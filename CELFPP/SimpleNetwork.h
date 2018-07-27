#pragma once

#include "util.h"
#include "header.h"
//#include "community.h"


class BaseNode;
class BaseEdge;
class HanpNode;
class HanpEdge;

class BaseNode {
public:
    static const size_t noCommunity = 4294967295U;
    size_t id;
    size_t communityId;
    
public:
    BaseNode(size_t vid)
        : id(vid), communityId(noCommunity){
    
    }
    bool operator()(const BaseNode* n1, const BaseNode* n2) {
        return n1->id < n2->id;
    }
    bool operator == (const BaseNode &node) const {
        return id == node.id;
    }

    bool operator != (const BaseNode &node) const {
        return id != node.id;
    }
};

class BaseEdge {
public:
    double weight;
public:
    BaseEdge(double vw)
        : weight(vw) {

    }
};

class HanpNode : public BaseNode {
public:
    unordered_map<HanpNode*, HanpEdge*> inAdjacencies, outAdjacencies;
    set<HanpEdge*> relationships;
    double score;
    double degree;
public:
    HanpNode(size_t vid)
        : BaseNode(vid) {

    }
};

class HanpEdge : public BaseEdge {
public:
    HanpNode *start, *end;
public:
    HanpEdge(HanpNode *vstart, HanpNode *vend, double vw)
        : start(vstart), end(vend), BaseEdge(vw) {

    }
};


class HanpCommunity {
public:
    size_t communityId;
    vector<HanpNode*> nodes;
    vector<int> short_distance_min;
    vector<int> short_distance_max;
    vector<int> node_distance1;
    vector<int> node_distance2;
public:
    HanpCommunity(size_t vcid)
        : communityId(vcid) {

    }
};

class HanpNetwork {
public:
    vector<HanpNode*> nodes;
    vector<HanpEdge*> edges;
    vector<HanpCommunity*> communities;
    bool weighted;
    size_t m, n;

private:
    void loadFromFile(string path) {
        loadMeta(path);
        cout << "Dataset: " << path << ", nodes:" << n << ", edges:" << m << endl;
        nodes.reserve(n);
        HanpNode *node = NULL;
        for (size_t i = 0; i < n; ++i) {
            node = new HanpNode(i);
            nodes.push_back(node);
        }
        loadEdges(path);
        cout << "Network built" << endl;
    }
    void loadMeta(string path) {
        ifstream in(path);
        string line;
        string* str = new string[2];
        while (getline(in, line)) {
            //cout<<line<<endl;
            if (line[0] != '#')
                break;

            if (line.substr(0, 6) == "#Edges" || line.substr(0, 6) == "#Nodes")
            {
                char *token = strtok(const_cast<char*>(line.c_str()), ":");
                int i = 0;
                while (token != NULL)
                {
                    str[i++] = token;
                    token = strtok(NULL, ":");
                }
                if (str[0] == "#Nodes")
                    n = size_t(atoi(str[1].c_str()));
                if (str[0] == "#Edges")
                    m = size_t(atoi(str[1].c_str()));
            }
        }
        delete[] str;
        in.clear();
        in.close();
    }
    void loadEdges(string path) {
        ifstream in(path);
        string line;
        int r = 0;
        int* num = new int[2];
        HanpEdge *edge = NULL;
        HanpNode *start = NULL, *end = NULL;
        float weight = 1;
        int j = 0;
        int mycount = 0;
        while (getline(in, line))
        {
            //cout<<line<<endl;
            if (line[0] != '#')
            {
                char *token = strtok(const_cast<char*>(line.c_str()), "\t");// 
                int i = 0;
                while (token != NULL)
                {
                    if (i<2)
                        num[i++] = atoi(token);
                    else if (weighted == true)
                        weight = atof(token);// weighted graph
                    token = strtok(NULL, "\t");
                }
                mycount++;
                if (mycount % 1000 == 0)
                    cout << mycount << endl;
                start = nodes[num[0]];
                end = nodes[num[1]];
                edge = new HanpEdge(start, end, weight);
                edges.push_back(edge);
                start->outAdjacencies[end] = edge;
                start->relationships.insert(edge);
                end->inAdjacencies[start] = edge;
                end->relationships.insert(edge);
            }
        }
        delete[] num;
        in.clear();
        in.close();
    }
public:
    HanpNetwork(string dataPath)
        : m(0), n(0), weighted(false) {
        cout << "Loading network" << endl;
        loadFromFile(dataPath);
    }
    void detect(int dect, int com){}
};
