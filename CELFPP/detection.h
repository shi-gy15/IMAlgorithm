#ifndef CD_DETECTION_H
#define CD_DETECTION_H

#include"node.h"
#include"network.h"
#include "SimpleNetwork.h"

typedef pair<int,list<Node*> > ComPair; // community<cid,members>
typedef vector<ComPair> ComsVec; 
typedef map<int,list<Node*> > ComsMap;
typedef set<Node*> Outs;
typedef set<Node*> Hubs;

using namespace std;

class Detection{

public:
	int method;
	string data_path;

	int n,m;
	vector<Node*> nodes;
	vector<Edge*> rels;
	
	bool weighted;
	int mvs; //minimal valid size

	ComsVec coms_vec;

protected:
	int frequency;

	ComsMap coms_map;
	Hubs hubs;
	Outs outliers;

	map<int,int> size_map; //distribution
	
	clock_t t_p0, t_p1, t_p2, t_p3_a, t_p3_b;
	double time_init, time_iter, time_draft, time_ext;

public:

	Detection();

	void GetNetwork(Network* network);

	virtual void Proceed(); // go to detection 

	void Show(int k);  // show top-k communities, -1 means "all"
	void Record(int k, string record_path);
	
	void GetTimes(string record_path);
	void GetDistribution(string record_path);

	virtual ~Detection();

    // my
    //vector<Node*> snodes;
    //vector<Edge*> sedges;
    //void getSimpleNetwork(SimpleNetwork* network);
    //void detectionMain(SimpleNetwork* network);
    //void getSimpleCommunity(SimpleNetwork* network);

protected:

	void CD_PreCompute() {}
	void CD_FirstAllocate() {}
	void CD_Select() {}
	void CD_Fluctuate() {}
	void CD_Update() {}
    void CD_Collect() {}
	void CD_MultiLevelDraft() {}
	void CD_Extract();

public:
	void CD_Erase();
	void CD_Order();
	void CD_OrderMembers();
	void CD_GetCommunity(Network* network);
	void CD_GetOutliers();
	void Detection_main(Network* network);
};

#endif