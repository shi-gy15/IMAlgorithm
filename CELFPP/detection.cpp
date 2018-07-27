#include"node.h"
#include"detection.h"
#include"import.h"

Detection::Detection()
{
	cout<<"create detection..."<<endl;
}

void Detection::Proceed()
{
}

void Detection::GetNetwork(Network* network)
{
	this->data_path = network->data_path;
	this->weighted = network->weighted;
	this->n = network->n;
	this->m = network->m;
	this->nodes = network->nodes;
	this->rels = network->edges;
}

void Detection::CD_FirstAllocate()
{
	for(int i=0;i<=this->n;i++) 
		this->nodes[i]->communityId=this->nodes[i]->id;
}

void Detection::CD_Erase()
{
	if(this->mvs<=1)
		return;

	// erase small communities
	int size_before=this->coms_map.size();
	
	ComsMap::iterator i;
	for (i = coms_map.begin(); i != coms_map.end();)
	{  
		if(i->second.size()<this->mvs)	
		{
			for(Node* n : i->second)
				this->outliers.insert(n);
			coms_map.erase(i++);
		}
		else
			i++;
	}
	cout<<"before erase:"<<size_before<<"\tafter erase:"<<coms_map.size()<<endl;
}

void Detection::CD_Extract()
{
	// tag communityId
	ComsMap::iterator i_m;
	for(i_m=this->coms_map.begin();i_m!=this->coms_map.end();i_m++)
	{
		int communityId=(*i_m).first;
		for(Node* node : (*i_m).second)
			node->communityId=communityId;
	}
}

int cmp_csize(const ComPair& x, const ComPair& y)
{  
	return x.second.size() > y.second.size();  
} 

void Detection::CD_Order()
{
	ComsMap::iterator i;
	for (i=this->coms_map.begin();i!=this->coms_map.end();i++)
		this->coms_vec.push_back(ComPair(i->first,i->second));  
	
	sort(this->coms_vec.begin(),this->coms_vec.end(),cmp_csize); 
	cout<<"communities ordered!"<<endl;
}

void Detection::CD_OrderMembers(){
	
	ComsMap::iterator i;
	for (i=this->coms_map.begin();i!=this->coms_map.end();i++)
		i->second.sort(Node());
	cout<<"memebers ordered!"<<endl;
}

void Detection::CD_GetCommunity(Network* network){
	
	int i=0;
	Community* community=NULL;
	community = new Community(0);
	//community->id=0;
	network->communities.push_back(community);
	int all=this->coms_vec.size()+1;
	for(ComPair c : this->coms_vec)
	{   i++;
		if(i%1000==0)
		{
			cout<<i<<"of"<<all<<endl;
		}
		community = new Community(i);
		//community->id=i;
		for(Node* n : c.second)
		{
			n->communityId = i;
			community->nodes.push_back(n);
		}
		network->communities.push_back(community);
	}
	
	community = new Community(all);
	//community->id=all;
	//cout<<community->id<<endl;
	//cout<<network->com.size()<<endl;
	for(Node* out : this->outliers)
	{
		i++;
		if(i%100==0)
		{
			cout<<i<<"of"<<all<<endl;
		}
		
		out->communityId= all;
		community->nodes.push_back(out);
		
	}
	if(community->nodes.size()>0)
	{
		network->communities.push_back(community);
	}
	//cout<<network->com.size()<<endl;
	for(Community* community:network->communities)
	{
		community->short_distance_min.resize(network->communities.size(),INT_MAX);
		community->short_distance_max.resize(network->communities.size(),0);
		community->node_distance1.resize(network->n+1,INT_MAX);
		community->node_distance2.resize(network->n+1,INT_MAX);
	}
}

void Detection::CD_GetOutliers()
{
	Node* node;
	for(int i=1;i<=this->n;i++)
	{
		node = this->nodes[i];
		if(node->communityId==-1)
			this->outliers.insert(node);
	}
	cout<<"outliers collected!"<<endl;
}

void Detection::Show(int k)
{
	cout<<"detected communities:"<<this->coms_vec.size()<<endl;
	int i=0;
	for(ComPair c : this->coms_vec)
	{   i++;
		if(k>0 && i>k)
				break;
		cout<<i<<"\tsize:"<<c.second.size()<<"\t{";
		/*for(Node* n : c.second)
		{
			cout<<n->id<<",";
		}*/
		cout<<"}"<<endl;
	}
	cout<<"detected outliers:"<<this->outliers.size()<<endl;
	for(Node* out : this->outliers)
	{
		cout<<out->id<<",";
	}
	if(!this->outliers.empty())
		cout<<endl;
} 

void Detection::Record(int k,string record_path)
{ 
	ofstream ofs;
	ofs.open(record_path,ostream::app);
	// write in file
	ofs<<endl<<"method:"<<this->method<<", dataset:"<<this->data_path<<endl;
	ofs<<"detected communities:"<<this->coms_vec.size()<<endl;
	int i=0;
	for(ComPair c : this->coms_vec)
	{   i++;
		if(k>0 && i>k)
				break;
		ofs<<i<<"\tsize:"<<c.second.size()<<"\t{";
		for(Node* n : c.second)
		{
			ofs<<n->id<<",";
		}
		ofs<<"}"<<endl;
	}
	ofs<<"detected outliers:"<<this->outliers.size()<<endl;
	for(Node* out :  this->outliers)
		ofs<<out->id<<",";
	if(!this->outliers.empty())
		ofs<<endl;
	ofs.close(); 
	ofs.clear(); 
	cout<<"result recorded!"<<endl;
}

void Detection::GetDistribution(string record_path)
{
	map<int,int>::iterator i_m;
	int size;
	for(ComPair c : this->coms_vec)
	{
		size = c.second.size();
		i_m = this->size_map.find(size);
		if(i_m != this->size_map.end())
			this->size_map[size]++;
		else 
			this->size_map.insert(make_pair(size,1));
	}
	// write in file
	ofstream ofs;
	ofs.open(record_path,ostream::app);
	//ofs<<endl<<"method:"<<this->method<<", dataset:"<<this->data_path<<endl;
	ofs<<"community distribution:"<<endl;
	ofs<<"size"<<"\t"<<"number"<<endl;
	for(pair<int,int> p : this->size_map)
	{
		ofs<<p.first<<"\t"<<p.second<<endl;
		//cout<<"size:"<<p.first<<"\tn:"<<p.second<<endl;
	}
	ofs.close(); 
	ofs.clear(); 
	cout<<"distribution recorded!"<<endl;
}

void Detection::GetTimes(string record_path)
{
	this->time_init = (double)(t_p1-t_p0)/CLOCKS_PER_SEC;
	this->time_iter = (double)(t_p2-t_p1)/CLOCKS_PER_SEC;
	this->time_draft = (double)(t_p3_a-t_p2)/CLOCKS_PER_SEC;
	this->time_ext = (double)(t_p3_b-t_p2)/CLOCKS_PER_SEC;
	
	cout<<"initial time:"<<this->time_init<<"s"<<endl;
	cout<<"iteration time:"<<this->time_iter<<"s"<<endl;
	cout<<"collect&multi-draft time:"<<this->time_draft<<"s"<<endl;
	cout<<"extract time:"<<this->time_ext<<"s"<<endl;

	// write in file
	ofstream ofs;
	ofs.open(record_path,ostream::app);// add model
	//ofs<<endl<<"method:"<<this->method<<", dataset:"<<this->data_path<<endl;
	ofs<<"initial time:"<<this->time_init<<"s"<<endl;
	ofs<<"iteration time:"<<this->time_iter<<"s"<<endl;
	ofs<<"collect&multi-draft time:"<<this->time_draft<<"s"<<endl;
	ofs<<"extract time:"<<this->time_ext<<"s"<<endl;
	ofs.close(); 
	ofs.clear(); 
	cout<<"processing time recorded!"<<endl;
}

void Detection::Detection_main(Network* network)
{
	this->mvs = 2;
	this->GetNetwork(network);
	this->Proceed();
	this->CD_GetOutliers();
	this->CD_Erase();
	this->CD_Order();
	//this->CD_OrderMembers();
	this->CD_GetCommunity(network);
	this->Show(-1);
	//string record_path =  "result1/"+time()+".txt";
	//this->Record(-1,record_path);
	//this->GetTimes(record_path);
	//this->GetDistribution(record_path);
}

Detection::~Detection()
{
	cout<<"delete detection..."<<endl;
}


