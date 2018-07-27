#include "IMAlgorithm.h"

void AMICSSolver::solve() {
    calcCommunity();
    Node* maxNode = NULL;
    size_t maxSize = 0;
    size_t curSize = 0;
    for (int t = 0; t < k; ++t) {
        maxNode = NULL;
        maxSize = 0;
        for (auto node : network->nodes) {
            if (!node->amics.covered) {
                curSize = node->amics.connectedCommunities.size();
                if (!maxNode ||
                    curSize > maxSize ||
                    (curSize == maxSize && node->outAdjacencies.size() > maxNode->outAdjacencies.size()) ||
                    node->id <= maxNode->id) {
                    maxNode = node;
                    maxSize = curSize;
                    eraseNeighbours(node);
                }
            }
        }
        if (maxNode) {
            S.insert(maxNode);
        }
        else {
            cout << "Error: no node added in AMICS " << t << "." << endl;
        }
    }
}

void AMICSSolver::calcCommunity() {
    unordered_map<size_t, int>::iterator itf;
    for (auto node : network->nodes) {
        if (node->communityId != Node::noCommunity) {
            for (auto prev : node->inAdjacencies) {
                unordered_map<size_t, int> &cmap = prev.first->amics.connectedCommunities;
                itf = cmap.find(node->communityId);
                if (itf == cmap.end()) {
                    cmap[node->communityId] = 1;
                }
                else {
                    cmap[node->communityId]++;
                }
            }
        }
    }
}

void AMICSSolver::eraseNeighbours(Node* node) {
    unordered_map<size_t, int>::iterator itf;
    node->amics.covered = true;

    for (auto eraseNode : node->outAdjacencies) {
        for (auto prev : eraseNode.first->inAdjacencies) {
            // if prev has not been covered yet, and not in present eraseNode set
            if (!prev.first->amics.covered && node->outAdjacencies.find(prev.first) == node->outAdjacencies.end()) {
                unordered_map<size_t, int> &cmap = prev.first->amics.connectedCommunities;
                itf = cmap.find(eraseNode.first->communityId);
                if (itf != cmap.end()) {
                    itf->second--;
                    if (itf->second <= 0) {
                        cmap.erase(itf);
                    }
                }
            }
            eraseNode.first->amics.covered = true;
        }
    }
}

void AMICSSolver::reset() {
    IMBase::reset();
    for (auto node : network->nodes) {
        node->amics.clear();
    }
}

void AMICSSolver::show(ostream& os, bool detail) {
    os << "In AMICS: ";
    IMBase::show(os, detail);
}
