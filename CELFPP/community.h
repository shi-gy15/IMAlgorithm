#ifndef COMMUNITY_H
#define COMMUNITY_H

#include"util.h"
#include"node.h"

using namespace std;

class Community {
public:
    size_t communityId;
    vector<Node*> nodes;
    vector<int> short_distance_min;
    vector<int> short_distance_max;
    vector<int> node_distance1;
    vector<int> node_distance2;
public:
    Community(size_t vcid)
        : communityId(vcid) {

    }
	bool operator()(const Community* n1,const Community* n2)
	{
		return n1->communityId < n2->communityId;
	}

	bool operator == (const Community &community) const
	{
		return communityId == community.communityId;
	}

	bool operator != (const Community &community) const
	{
		return communityId != community.communityId;
	}
};

//struct NodePair {
//	Node* n1;
//	Node* n2;
//	int type;
//};
//
//struct CoPair {
//	Community* c1;
//	Community* c2;
//};
#endif