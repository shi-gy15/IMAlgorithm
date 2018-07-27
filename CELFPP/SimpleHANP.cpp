#include "SimpleHANP.h"

SimpleHANP::SimpleHANP() {
    
}

void SimpleHANP::getNetwork(Network* network) {
    weighted = network->weighted;
    n = network->n;
    m = network->m;
    nodes = network->nodes;
    edges = network->edges;
}

void SimpleHANP::getCommunity(Network* network) {
    size_t i = 0;
    Community* community = NULL;
    community = new Community(0);
    network->communities.push_back(community);
    size_t all = coms_vec.size() + 1;
    for (ComPair c : coms_vec) {
        i++;
        if (i % 1000 == 0)
            cout << i << "of" << all << endl;
        
        community = new Community(i);
        for (Node* n : c.second) {
            n->communityId = i;
            community->nodes.push_back(n);
        }
        network->communities.push_back(community);
    }

    community = new Community(all);
    //cout<<community->id<<endl;
    //cout<<network->com.size()<<endl;
    for (Node* out : outliers) {
        i++;
        if (i % 100 == 0)
            cout << i << "of" << all << endl;

        out->communityId = all;
        community->nodes.push_back(out);

    }
    if (!community->nodes.empty()) {
        network->communities.push_back(community);
    }
    //cout<<network->com.size()<<endl;
    for (Community* community : network->communities)
    {
        community->short_distance_min.resize(network->communities.size(), INT_MAX);
        community->short_distance_max.resize(network->communities.size(), 0);
        community->node_distance1.resize(network->n + 1, INT_MAX);
        community->node_distance2.resize(network->n + 1, INT_MAX);
    }
}

void SimpleHANP::proceed() {
    cout << "HANP started." << endl;
    srand(time(0));
    start();
    t_p0 = clock();
    firstAllocate();
    t_p1 = clock();
    cout << "HANP Initialization done!" << endl;
    getSequence();

    for (int i = 0; i < max_T; i++) {
        cout << i << " hanp" << endl;
        fluctuate();
    }

    t_p2 = clock();
    cout << "Transformation done!" << endl;
    extract();
    t_p3_b = clock();
    cout << "Construction done!" << endl;
    cout << "HANP finished!" << endl;
}

void SimpleHANP::start() {
    for (size_t i = 0; i <= n; i++) {
        nodes[i]->score = 1;
        nodes[i]->degree = nodes[i]->outAdjacencies.size();
    }
}

void SimpleHANP::firstAllocate() {
    for (size_t i = 0; i <= n; i++)
        nodes[i]->communityId = nodes[i]->id;
}

void SimpleHANP::getSequence() {
    sequence = new size_t[n];
    for (size_t i = 0; i < n; i++) {
        sequence[i] = i + 1;
    }
}

void SimpleHANP::fluctuate() {
    Node *node;
    size_t oldLabel;
    parallel_for(1U, n, [&](size_t j) {
        node = nodes[j];
        if (!node->relationships.empty()) {
            oldLabel = node->communityId;
            node->communityId = updateLabel(node->relationships);
            update(node, oldLabel);
        }
    });
}

size_t SimpleHANP::updateLabel(set<Edge*>& rels) {
    map<int, float> count;// map of <cid,frequency>
    map<int, float>::iterator i_f;

    // count frequency of each label: si*fi^a*w(r)
    float f;
    int id, cid;
    Node* nbr;
    for (Edge* rel : rels)
    {
        id = rel->end->id;
        cid = rel->end->communityId;
        nbr = this->nodes[id];

        // score(id)*degree(id)^a*weight(rel)
        f = nbr->score * pow(nbr->degree, this->prefer_factor) * pow(rel->weight, this->prefer_factor);

        i_f = count.find(cid);
        if (i_f != count.end())
            i_f->second += f;
        else
            count.insert(pair<int, float>(cid, f));
    }

    return countLabel(count);
}

void SimpleHANP::update(Node* node, size_t old) {
    int id = node->id;
    int cid = node->communityId;
    float s, max_s = INT_MIN;
    for (auto nbr : node->outAdjacencies)
    {
        if (nbr.first->communityId == cid) {
            s = nodes[nbr.first->id]->score;
            if (s>max_s)
                max_s = s;
        }
    }
    if (cid != old) // to avoid negative feedback loop
        max_s -= this->decay_factor;

    this->nodes[id]->score = max_s;
}

int SimpleHANP::countLabel(map<int, float>& count) {
    vector<pair<int, float>> vec;
    for (pair<int, float> p : count)
        vec.push_back(p);

    sort(vec.begin(), vec.end(), cmp_float);

    vector<int> candidates;

    float max_f = vec[0].second;
    for (pair<int, float> p : vec)
    {
        if (p.second != max_f)
            break;
        candidates.push_back(p.first);
    }

    int pos = 0;
    if (!candidates.empty())
        pos = rand() % candidates.size();

    return candidates[pos];
}

void SimpleHANP::extract() {
    ComsMap::iterator i_c;
    for (int i = 1; i <= this->n; i++)
    {
        i_c = this->coms_map.find(nodes[i]->communityId);
        if (i_c == this->coms_map.end())//if not in a community
        {
            list<Node*> members;
            members.push_back(this->nodes[i]);
            this->coms_map.insert(ComPair(this->nodes[i]->communityId, members));//save to map,key is cid
        }
        else
            i_c->second.push_back(this->nodes[i]);//already,add
    }
}

void SimpleHANP::getOutliers() {
    Node* node;
    for (int i = 1; i <= this->n; i++)
    {
        node = this->nodes[i];
        if (node->communityId == Node::noCommunity)
            this->outliers.insert(node);
    }
    cout << "outliers collected!" << endl;
}

void SimpleHANP::erase() {
    if (this->mvs <= 1)
        return;

    // erase small communities
    int size_before = this->coms_map.size();

    ComsMap::iterator i;
    for (i = coms_map.begin(); i != coms_map.end();)
    {
        if (i->second.size()<this->mvs)
        {
            for (Node* n : i->second)
                this->outliers.insert(n);
            coms_map.erase(i++);
        }
        else
            i++;
    }
    cout << "before erase:" << size_before << "\tafter erase:" << coms_map.size() << endl;
}

int cmp_csize(const ComPair& x, const ComPair& y)
{
    return x.second.size() > y.second.size();
}

void SimpleHANP::order() {
    ComsMap::iterator i;
    for (i = this->coms_map.begin(); i != this->coms_map.end(); i++)
        this->coms_vec.push_back(ComPair(i->first, i->second));

    sort(this->coms_vec.begin(), this->coms_vec.end(), cmp_csize);
    cout << "communities ordered!" << endl;
}

void SimpleHANP::show(int k) {
    cout << "detected communities:" << this->coms_vec.size() << endl;
    int i = 0;
    for (ComPair c : this->coms_vec)
    {
        i++;
        if (k>0 && i>k)
            break;
        cout << i << "\tsize:" << c.second.size() << "\t{";
        /*for(Node* n : c.second)
        {
        cout<<n->id<<",";
        }*/
        cout << "}" << endl;
    }
    cout << "detected outliers:" << this->outliers.size() << endl;
    for (Node* out : this->outliers)
    {
        cout << out->id << ",";
    }
    if (!this->outliers.empty())
        cout << endl;
}

void SimpleHANP::detectionMain(Network* network) {
    mvs = 2;
    getNetwork(network);
    proceed();
    getOutliers();
    erase();
    order();
    getCommunity(network);
    show(10);
}