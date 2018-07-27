#pragma once

#include "util.h"
#include "network.h"
#include "IMBase.h"


struct Mgcmp {
    bool operator() (Node* const node1, Node* const node2) const {
        return node1->info.mg1 > node2->info.mg1 || (node1->info.mg1 == node2->info.mg1 && node1 < node2);
    }
};

typedef set<Node*, Mgcmp> heap;

class CELFSolver : public IMBase {
public:
    Node *lastSeed;
    Node *curBest;
    heap Q;
public:
    CELFSolver(Network *vnet, int vk, float vth)
        : IMBase(vnet, vk, vth) {
        //cout << "build success, threshold = " << threshold << endl;
    }
    void solve();
    void init();
    void reset();
    void show(ostream& os, bool detail);
};

class SimpleGreedySolver : public IMBase {
public:
    SimpleGreedySolver(Network *vnet, int vk, float vth)
        : IMBase(vnet, vk, vth) {}
    void solve();
    void show(ostream& os, bool detail);
};

class AMICSSolver : public IMBase {
public:
    AMICSSolver(Network *vnet, int vk, float vth)
        : IMBase(vnet, vk, vth) {

    }
    void calcCommunity();
    void eraseNeighbours(Node* node);
    void solve();
    void reset();
    void show(ostream& os, bool detail);
};