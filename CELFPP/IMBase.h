#pragma once

#include "util.h"
#include "network.h"

typedef unordered_set<Node*> nodeset;

class IMBase {
public:
    Network * network;
    int k;
    nodeset S;
    float threshold;
    double solveStartTime;
public:
    IMBase(Network *vnet, int vk, float vth)
        : network(vnet), k(vk), threshold(vth), solveStartTime(0) {
        //fixLT();
    }
    virtual void solve();
    float influence(Node* node, string type);
    //void spread(nodeset& seeds);
    size_t spreadSize(nodeset& seeds);
    int marginalGain(Node* node, nodeset& seeds);
    virtual void show(ostream& os, bool detail);
    virtual void reset();
};