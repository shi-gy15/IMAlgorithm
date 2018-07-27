#ifndef NODE_H
#define NODE_H

#include"util.h"

using namespace std;

class Edge;
class Node;

struct NodeInfo {
    int mg1;
    Node *prevBest;
    int mg2;
    int flag;
    bool active;
    NodeInfo() : mg1(0), prevBest(NULL), mg2(0), flag(0), active(false) {}
    void clear() {
        mg1 = 0;
        mg2 = 0;
        prevBest = NULL;
        flag = 0;
        active = false;
    }
};

struct AMICS {
    //int connectedCommunities;
    unordered_map<size_t, int> connectedCommunities;
    bool covered;
    AMICS() : covered(false) {}
    void clear() {
        connectedCommunities.clear();
        covered = false;
    }
};

class Node{
public:
    static const size_t noCommunity = 4294967295U;
    size_t id;
    size_t communityId;
	
	set<Edge*> relationships;
	//set<Node*> neighbours;
	//set<Node*> pre_neighbours;
    unordered_map<Node*, Edge*> inAdjacencies, outAdjacencies;
    /*unordered_map<Node*, Edge*> ;*/
	//vector<int> short_distance;
	//vector<int> short_count;
	//vector<bool> short_flag;
	//double betwenness;

    // !hanp
    float degree; // cnm & hanp & kmf & radicchi
    float score; // hanp

    // end

	set<int> cids; // topleaders
	float leadership; 
	int role; // 2:leader; 1:member; 0:hub; -1:outlier(default)
	int mark; // Radicchi
	int graph_id;
	bool move; // kmf
	
	float activeness; // clpa
	float AS; // gclus
	int _cid;
	long long bigcid; 

	int dist; // for BFS shortest path

    // CELFPP
    NodeInfo info;

    // AMICS
    AMICS amics;

    //.
public:
    Node() {}
    Node(size_t vid)
        : id(vid), communityId(noCommunity) {}
	bool operator()(const Node* n1,const Node* n2)
	{
		return n1->id < n2->id;    
	}

	bool operator == (const Node &node) const
	{
		return id == node.id;
	}

	bool operator != (const Node &node) const
	{
		return id != node.id;
	}
};

class Edge{
public:
	Node* start;
	Node* end;	

	float weight;
	
	bool move; // kmf & radicchi
	int triangle; // kmf
    float cosine;  // gclus   
	float RS;
	float CCS;  
	float q; // cnm
	float cluster; // radicchi

	Edge(){}
	Edge(Node* vstart, Node* vend, float _q)
        : start(vstart), end(vend), q(_q) {}
};

//class Node {
//public:
//    static const size_t noCommunity = 4294967295U;
//    size_t id;
//    size_t communityId;
//    unordered_map<Node*, Edge*> inAdjacencies, outAdjacencies;
//public:
//    Node() {}
//    Node(size_t vid)
//        : id(vid), communityId(noCommunity) {}
//    bool operator()(const Node* n1, const Node* n2) {
//        return n1->id < n2->id;
//    }
//};

//class Edge {
//public:
//    Node * start, *end;
//    double weight;
//public:
//    Edge(Node *vstart, Node *vend, double vw)
//        : start(vstart), end(vend), weight(vw) {}
//};

#endif