#pragma once

#include"node.h"
#include"network.h"
#include "SimpleNetwork.h"

typedef pair<int, list<Node*> > ComPair; // community<cid,members>
typedef vector<ComPair> ComsVec;
typedef map<int, list<Node*> > ComsMap;
typedef set<Node*> Outs;
typedef set<Node*> Hubs;

class SimpleHANP {
private:
    int max_T;
    float prefer_factor;
    float decay_factor;
    size_t* sequence;

public:
    int method;
    string data_path;

    size_t n, m;
    vector<Node*> nodes;
    vector<Edge*> edges;

    bool weighted;
    int mvs; //minimal valid size

    ComsVec coms_vec;

protected:
    int frequency;

    ComsMap coms_map;
    Hubs hubs;
    Outs outliers;

    map<int, int> size_map; //distribution

    clock_t t_p0, t_p1, t_p2, t_p3_a, t_p3_b;
    double time_init, time_iter, time_draft, time_ext;

public:
    SimpleHANP();
    SimpleHANP(float vprefer_factor, float vdecay_factor, int vmax_T, int vmvs)
        : prefer_factor(vprefer_factor), decay_factor(vdecay_factor), max_T(vmax_T), mvs(vmvs) {

    }

    ~SimpleHANP() {}
    void getNetwork(Network* network);
    void proceed();
    void show(int k);
    //void record(int k, string record_path);
    //void getTimes(string record_path);
    //void getDistribution(string record_path);
    void getCommunity(Network* network);
    void start();
    void firstAllocate();
    void getSequence();
    void fluctuate();
    size_t updateLabel(set<Edge*>& rels);
    void update(Node* node, size_t old);
    int countLabel(map<int, float>& count);
    void extract();
    void getOutliers();
    void erase();
    void order();
    void detectionMain(Network* network);
};
