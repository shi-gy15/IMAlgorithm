#include "IMBase.h"

float IMBase::influence(Node* node, string type) {
    float infl = 0;
    int num = 0;
    for (auto &neighbour : node->inAdjacencies) {
        if (neighbour.first->info.active) {
            //infl += node->inAdjacencies[neighbour]->weight;
            infl += neighbour.second->weight;
            num++;
        }
    }
    /*if (infl > 1)
        cout << infl << " ";*/
    if (type == "edge")
        return infl;
    else if (type == "num")
        return num;
}



size_t IMBase::spreadSize(nodeset& seeds) {
    seeds.erase(NULL);
    nodeset curActiveNodes(seeds);
    nodeset candidates;
    nodeset allActiveNodes(seeds);

    // init
    for (auto seed : seeds) {
        seed->info.active = true;
    }

    // iterate
    while (!curActiveNodes.empty()) {
        for (auto seed : curActiveNodes) {
            for (auto succ : seed->outAdjacencies) {
                if (!succ.first->info.active)
                    candidates.insert(succ.first);
            }
        }
        curActiveNodes.clear();
        for (auto cand : candidates) {
            if (influence(cand, "edge") >= threshold) {
            //if (influence(cand, "num") / cand->inAdjacencies.size() >= threshold) {
                //cout << "[" << influence(cand) << " " << cand->pre_neighbours.size() << "]" << endl;
                curActiveNodes.insert(cand);
            }
        }
        candidates.clear();
        for (auto activeNode : curActiveNodes) {
            activeNode->info.active = true;
            allActiveNodes.insert(activeNode);
        }
    }

    // reset
    for (auto activeNode : allActiveNodes) {
        activeNode->info.active = false;
    }

    // test
    /*for (auto node : network->nodes) {
        if (node && node->info.active)
            cout << "err!" << endl;
    }*/
    return allActiveNodes.size();
}

int IMBase::marginalGain(Node* node, nodeset& seeds) {
    nodeset test(seeds);
    test.insert(node);
    return spreadSize(test) - spreadSize(seeds);
}

void IMBase::solve() {
    solveStartTime = clock();
}

void IMBase::reset() {
    S.clear();
    solveStartTime = 0;
}

void IMBase::show(ostream& os, bool detail) {
    os << "Time: " << (clock() - solveStartTime) / CLOCKS_PER_SEC << ", ";
    os << "Influence: " << spreadSize(S) << endl;
    int count = 0;
    if (detail) {
        nodeset test;
        for (auto seed : S) {
            test.clear();
            test.insert(seed);
            ++count;
            os << count << ". Node " << seed->id << ", with neighbours " << seed->outAdjacencies.size();
            os << ", spread " << spreadSize(test) << endl;
        }
        os << endl;
    }
}