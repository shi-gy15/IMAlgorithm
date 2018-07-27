#include "IMAlgorithm.h"

void SimpleGreedySolver::solve() {
    IMBase::solve();
    // each time select nodes with greatest marginal gain
    size_t maxmg = 0;
    Node *maxnode = NULL;
    size_t curSSize = 0;
    size_t gain = 0;
    bool first = true;
    for (int t = 0; t < k; ++t) {
        curSSize = spreadSize(S);
        if (curSSize >= network->n) {
            //cout << "End on iteration " << t << endl;
            break;
        }
        maxmg = 0;
        maxnode = NULL;
        for (auto node : network->nodes) {
            if (first) {
                first = false;
                continue;
            }
            if (S.find(node) == S.end()) {
                S.insert(node);
                gain = spreadSize(S) - curSSize;
                S.erase(node);
                if (gain > maxmg) {
                    maxmg = gain;
                    maxnode = node;
                }
            }
        }
        if (maxnode) {
            S.insert(maxnode);
        }
        /*else {
        cout << "Error: no node added in greedy iteration." << endl;
        }*/
    }
}

void SimpleGreedySolver::show(ostream& os, bool detail) {
    os << "In simple greedy: ";
    IMBase::show(os, detail);
}