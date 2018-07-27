#ifndef NETWORK_H
#define NETWORK_H

#include"node.h"
#include"community.h"


using namespace std;

class Network{

public:
	string data_path;
	string result_path;

    vector<Node*> nodes;
    vector<Edge*> edges;
    vector<Community*> communities;
    bool weighted;
    size_t m, n;

	int comtype;
    float sumWeight;
	//set<NodePair*> mypairs;
	//set<CoPair*> compairs;
	//double time0;

	//float avg_degree;
	//float density;
	//int components;

	//double rate1;
	//double rate2;

	//int** distance;
	//bool shorest_path_done;

	//bool flag;

public:	
	Network(string data_path);
	
	void GetData();

    void fixLT();
    void calcSumWeight();

	//void GetAvgDegree();
	//void GetGlobalDensity();
	//void GetConnectComponents();

	//void Init();
	//void InitShortDistance();
	//void InitCountPath();
	//void InitBetwenness();
	//void InitCommunityShortDistance();

	//void CommunityShortDistance();

	//void Brandes();
	//void clear();

	//void SetCountPath(Node* n1,Node* n2);
	//void CountPath(Node* n1,Node* n2);

	//void AddRel(int n1,int n2,float weight);
	//void DelRel(int n1,int n2);

	//void AddBet(Node* n1,Node* n2,float weight);
	//void DelBet(Node* n1,Node* n2,float weight);
	//void ResetShortDistance();

	//void Test();
	//void TestAdd(int dect,int com);
	//void TestDel(int dect,int com);

    void detect(int com, int mvs);

	~Network();
};

#endif