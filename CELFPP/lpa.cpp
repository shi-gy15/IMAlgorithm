#include"lpa.h"

LPA::LPA(int max_T){
	this->max_T=max_T;
}

void LPA::Proceed()
{
	cout<<"LPA started..."<<endl;
	srand(time(0));
	
	t_p0 = clock();
	this->CD_FirstAllocate();
	t_p1 = clock();
	cout<<"initialization done!"<<endl;

	this->GetSequence();
	for(int i=0; i<this->max_T; i++)
	{
		cout<<i<<endl;
		this->CD_Select();
		this->CD_Fluctuate();
	}
	t_p2 = clock();
	cout<<"transformation done!"<<endl;

	this->CD_Extract();
	t_p3_b = clock();
	cout<<"construction done!"<<endl;

	cout<<"LPA finished!"<<endl;
}

void LPA::CD_Select()
{
	list<int> ll;
	for(int i=1;i<=this->n;i++)
		ll.push_back(i);
	
	std::list <int> ::iterator pos ; 
	for(int i=0;i<this->n;i++)
	{
		if(i%1000==0)
		{
			cout<<i<<"SE"<<endl;
		}
		int t=rand()%ll.size();
		pos= ll.begin();
		for(int j=0;j<t;j++)
			pos++;
		
		this->sequence[i]=*pos;
		ll.remove(this->sequence[i]);
	}
}

void LPA::CD_Fluctuate()
{
	Node* node;
	for(int j=0; j<this->n; j++)
	{
		if(j%1000==0)
		{
			cout<<j<<"FL"<<endl;
		}
		node = this->nodes[sequence[j]];
		if(!node->relationships.empty()){
			node->cid = this->UpdateLabel(node->relationships);
		}
	}
}

void LPA::CD_Extract()
{
	ComsMap::iterator i_c;
	for(int i=1;i<=this->n;i++)
	{
		i_c=this->coms_map.find(nodes[i]->cid);
		if(i_c==this->coms_map.end())
		{
			list<Node*> members;
			members.push_back(this->nodes[i]);
			this->coms_map.insert(ComPair(this->nodes[i]->cid,members));
		}
		else
			i_c->second.push_back(this->nodes[i]);
	}
}

void LPA::GetSequence()
{
	this->sequence=new int[this->n];
	for(int i=0;i<this->n;i++)
		sequence[i]=i+1;
}


int LPA::CountLabel(map<int,float>& count)
{
	vector<pair<int,float> > vec;
	for(pair<int,float> p : count)
		vec.push_back(p);
	
	sort(vec.begin(), vec.end(), cmp_float);
	
	vector<int> candidates;
	
	float max_f=vec[0].second;
	for(pair<int,float> p : vec)
	{
		if(p.second!=max_f)
			break;
		candidates.push_back(p.first);
	}

	int pos=0;
	if(!candidates.empty())
		pos=rand()%candidates.size();

	return candidates[pos];
}

int LPA::UpdateLabel(set<Relationship*>& rels)
{
	map<int,float> count;// map of <cid,frequency>
	map<int,float>::iterator i_f;

	// count frequency of each label 
	float f;
	int cid;
	for(Relationship* rel :rels)
	{
		cid=rel->endNode->cid;
		f=rel->weight;// weight
		
		i_f=count.find(cid);
		if(i_f!=count.end())
			i_f->second+=f;
		else
			count.insert(pair<int,float>(cid,f));
	}

	return this->CountLabel(count);
}

LPA::~LPA()
{
	cout<<"detele lpa..."<<endl;
	delete[] sequence;
}