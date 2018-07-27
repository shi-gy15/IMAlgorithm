//#include"header.h"
#include "util.h"
#include "network.h"
#include "IMAlgorithm.h"


struct IntCmp {
    bool operator() (int* const a, int* const b) const {
        return *a > *b;
    }
};

int main()
{	
    /*int *a = new int(3);
    int *b = new int(3);
    set<int*, IntCmp> ts;
    ts.insert(a);
    ts.insert(b);


    vector<int> vts;

    double t1 = clock();
    vts.reserve(1000000);
    for (int i = 0; i < 1000000; ++i)
        vts.push_back(i);
    cout << (clock() - t1) / CLOCKS_PER_SEC;

    vector<int> vts2;
    double t2 = clock();
    for (int i = 0; i < 1000000; ++i)
        vts2.push_back(i);
    cout << (clock() - t2) / CLOCKS_PER_SEC;

    system("pause");*/


    string path = "2";
    Network *net = new Network(path);
    cout << "sum: " << net->sumWeight << endl;
    net->detect(1, 5);
    //network->clear();
    //network->Brandes();

    // some test
    /*cout << "Testing network" << endl;
    for (int i = 1; i <= net->n; ++i) {
        Node *node = net->nodes[i];
        if (node->pre_neighbours.empty()) {
            cout << "Node " << i << " has no pres" << endl;
        }
        if (node->neighbours.size() >= 15) {
            cout << "Node " << i << " has " << node->neighbours.size() << " succs" << endl;
        }
    }

    cout << "Testing finish." << endl;*/

    vector<int> ks;
    //ks.push_back(5);
    ks.push_back(10);
    /*ks.push_back(20);
    ks.push_back(30);
    ks.push_back(40);
    ks.push_back(50);
    ks.push_back(60);*/

    float r;
    cin >> r;
    vector<float> ths;
    //ths.push_back(0.1);
    ths.push_back(r);
    /*ths.push_back(0.3);
    ths.push_back(0.4);
    ths.push_back(0.5);
    ths.push_back(0.6);
    ths.push_back(0.7);
    ths.push_back(0.8);*/

    int k = 10;
    float th = 0.5;
    //cout << "Input threshold: " << endl;



    CELFSolver celfs(net, k, th);
    SimpleGreedySolver sgs(net, k, th);
    AMICSSolver as(net, k, th);

    vector<IMBase*> solvers;
    solvers.push_back(&celfs);
    solvers.push_back(&sgs);
    solvers.push_back(&as);

    //ofstream fout("result0725.txt", ios::out);

    for (int k : ks) {
        for (float th : ths) {
            cout << "k = " << k << ", threshold = " << th << endl;
            //fout << "k = " << k << ", threshold = " << th << endl;
            for (auto &solver : solvers) {
                solver->k = k;
                solver->threshold = th;
                solver->solve();
                solver->show(cout, true);
                solver->reset();
            }
        }
    }


    
    // origin
    /*
	string data_path="p2p-08"; 
	cin>>data_path;
	cout<<data_path<<endl;
	Network* network;
	srand(time(0));
	for(int i=1;i<2;i++)
	{
		network = new Network(data_path);	
		srand(time(0));
		clock_t t_p0,t_p1;
		double time_init;
		network->clear();
		t_p0 = clock();
		network->Brandes();
		t_p1 = clock();
		time_init=(double)(t_p1-t_p0)/CLOCKS_PER_SEC;
		cout<<time_init<<endl;
		//network->time0=time_init;
		cout<<"2333333and"<<i<<endl;
		if(i%2==0)
			network->TestAdd(2,i/2+1);
		else
			network->TestDel(2,i/2+1);
		delete network;
			
	}
	//delete network;

    */
	system("pause");
	return 0;
}