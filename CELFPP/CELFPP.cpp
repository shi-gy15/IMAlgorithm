
#include "IMAlgorithm.h"

void CELFSolver::init() {
    nodeset test;
    for (auto node : network->nodes) {
        test.clear();
        test.insert(node);
        node->info.mg1 = spreadSize(test);
        node->info.prevBest = curBest;
        if (curBest) {
            test.insert(curBest);
            node->info.mg2 = spreadSize(test);
        }
        else
            node->info.mg2 = node->info.mg1;
        node->info.flag = 0;
        Q.insert(node);
        curBest = *Q.begin();
        //cout << node << " ";
    }

    
    /*for (auto node : Q) {
        if (node->info.mg1 != 1)
            cout << node->info.mg1 << " ";
    }*/
    
    
    /*cout << endl;
    cout << "Init finish." << endl;*/
}

void CELFSolver::reset() {
    IMBase::reset();
    Q.clear();
    bool first = true;
    for (auto node : network->nodes) {
        if (first) {
            first = false;
            continue;
        }
        node->info.clear();
    }
}

void CELFSolver::solve() {
    IMBase::solve();
    //fixLT();
    init();
    Node *cur;
    while (S.size() < k) {
        cur = *Q.begin();
        Q.erase(cur);
        if (cur->info.flag == S.size()) {
            S.insert(cur);
            lastSeed = cur;
            continue;
        }
        else if (cur->info.prevBest == lastSeed) {
            cur->info.mg1 = cur->info.mg2;
        }
        else {
            nodeset test(S);
            test.insert(curBest);
            cur->info.mg1 = marginalGain(cur, S);
            cur->info.prevBest = curBest;
            cur->info.mg2 = marginalGain(cur, test);
        }
        cur->info.flag = S.size();
        if (cur->info.mg1 > curBest->info.mg1)
            curBest = cur;
        Q.insert(cur);
    }
}

void CELFSolver::show(ostream& os, bool detail) {
    os << "In CELF: ";
    IMBase::show(os, detail);
}
