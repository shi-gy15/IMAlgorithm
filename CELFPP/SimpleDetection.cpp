#include "detection.h"

//void Detection::getSimpleNetwork(SimpleNetwork* network) {
//    weighted = network->weighted;
//    n = network->n;
//    m = network->m;
//    snodes = network->nodes;
//    sedges = network->edges;
//}
//
//void Detection::getSimpleCommunity(SimpleNetwork* network) {
//
//    int i = 0;
//    SimpleCommunity* community = NULL;
//    community = new SimpleCommunity();
//    community->communityId = 0;
//    network->communities.push_back(community);
//    int all = this->coms_vec.size() + 1;
//    for (sComPair c : this->coms_vec)
//    {
//        i++;
//        if (i % 1000 == 0)
//        {
//            cout << i << "of" << all << endl;
//        }
//        community = new SimpleCommunity();
//        community->communityId = i;
//        for (SNode* n : c.second)
//        {
//            n->cid = i;
//            community->nodes.push_back(n);
//        }
//        network->com.push_back(community);
//    }
//
//    community = new Community;
//    community->id = all;
//    //cout<<community->id<<endl;
//    //cout<<network->com.size()<<endl;
//    for (Node* out : this->outliers)
//    {
//        i++;
//        if (i % 100 == 0)
//        {
//            cout << i << "of" << all << endl;
//        }
//
//        out->cid = all;
//        community->myNode.push_back(out);
//
//    }
//    if (community->myNode.size()>0)
//    {
//        network->com.push_back(community);
//    }
//    //cout<<network->com.size()<<endl;
//    for (Community* community : network->com)
//    {
//        community->short_distance_min.resize(network->com.size(), INT_MAX);
//        community->short_distance_max.resize(network->com.size(), 0);
//        community->node_distance1.resize(network->n + 1, INT_MAX);
//        community->node_distance2.resize(network->n + 1, INT_MAX);
//    }
//}
//
//
//void Detection::detectionMain(SimpleNetwork* network) {
//    mvs = 2;
//    getSimpleNetwork(network);
//    Proceed();
//    CD_GetOutliers();
//    CD_Erase();
//    CD_Order();
//    CD_GetCommunity(network);
//    Show(-1);
//}