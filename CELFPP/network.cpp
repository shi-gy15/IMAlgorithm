#include"network.h"
#include"import.h"
#include"header.h"
#include "SimpleHANP.h"

Network::Network(string data_path)
{
	cout<<"loading network..."<<endl;
	this->weighted = false;
	this->data_path= "data/"+data_path+".txt";
	this->result_path= "result/"+data_path;
    sumWeight = 0;
	this->GetData();
    calcSumWeight();
    fixLT();
	//this->shorest_path_done = false;
}

void Network::calcSumWeight() {
    for (auto edge : edges) {
        sumWeight += edge->weight;
    }
}

void Network::fixLT() {
    for (auto rel : edges) {
        rel->weight /= rel->end->inAdjacencies.size();
        //cout << rel->weight << " ";
        if (rel->weight <= 0.0001)
            cout << rel->weight << " " << rel->end->inAdjacencies.size() << endl;
    }
}
void Network::GetData()
{
	GetNumbers(this->data_path, this->n, this->m);
	cout<<"dataset:"<<this->data_path<<", nodes:"<<this->n<<", edges:"<<this->m<<endl;
	
	Node* node=NULL;
	for(size_t i=0; i<=this->n; i++)
	{
		node = new Node(i);
		this->nodes.push_back(node);
		if(i%1000==0) cout<<i<<endl;
	}
    //nodes[0] = NULL;
	cout<<"relationships !"<<endl;
	GetAdjList(this->data_path, this->nodes, this->edges, this->weighted);
	cout<<"relationships built!"<<endl;
}

void Network::detect(int com, int mvs) {
    this->comtype = com;
    int max_T = 10;
    float prefer_factor = 0.1;
    float decay_factor = 0.05;
    SimpleHANP* detection = new SimpleHANP(prefer_factor, decay_factor, max_T, mvs);
    //GetMethod(dect, &detection);
    detection->detectionMain(this);
    delete detection;
}

Network::~Network()
{
	cout<<"delete network..."<<endl;

	for(int i=0;i<=this->n;i++)
	{
		delete this->nodes[i];
		this->nodes[i]=NULL;
	}
	
	for(size_t i=0;i<this->edges.size();i++)
	{
		delete this->edges[i];
		this->edges[i]=NULL;
	}
	for(size_t i=0;i<this->communities.size();i++)
	{
		delete this->communities[i];
		this->communities[i]=NULL;
	}

}