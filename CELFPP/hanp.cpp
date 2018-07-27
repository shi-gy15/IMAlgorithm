#include"hanp.h"

HANP::HANP(float prefer_factor, float decay_factor, int max_T)
{
	this->prefer_factor=prefer_factor;
	this->decay_factor=decay_factor;
	this->max_T=max_T;
}

void HANP::Proceed()
{
	cout<<"HANP started..."<<endl;
	srand(time(0));
	this->CD_Start();
	t_p0=clock();

	this->CD_FirstAllocate();
	t_p1=clock();
	cout<<"initialization done!"<<endl;
	
	this->GetSequence();
	for(int i=0;i<this->max_T;i++)
	{
		cout<<i<<"hanp"<<endl;
		//this->CD_Select();
		this->CD_Fluctuate();
	}
	t_p2=clock();
	cout<<"transformation done!"<<endl;
	
	this->CD_Extract();
	t_p3_b=clock();
	cout<<"construction done!"<<endl;
	
	cout<<"HANP finished!"<<endl;
}

void HANP::CD_Start()
{
	for(int i=0;i<=this->n;i++)
	{
		this->nodes[i]->score=1;
		this->nodes[i]->degree=this->nodes[i]->neighbours.size();
	}
}

void HANP::CD_Select()
{
	list<int> ll;
	for(int i=1;i<=this->n;i++)
		ll.push_back(i);
	
	std::list <int> ::iterator pos ; 
	for(int i=0;i<this->n;i++)
	{
		int t=rand()%ll.size();
		pos= ll.begin();
		for(int j=0;j<t;j++)
			pos++;
		
		this->sequence[i]=*pos;
		ll.remove(this->sequence[i]);
	}
}

void HANP::CD_Fluctuate()
{
	Node* node;
	parallel_for(1,this->n,[&](int j)
	//for(int j=0;j<this->n;j++)
	{
		node=this->nodes[j];
		if(!node->relationships.empty())
		{
			int old_label=node->communityId;
			node->communityId=this->UpdateLabel(node->relationships);
			this->CD_Update(node,old_label);
		}
	});
}

void HANP::CD_Update(Node* node,int old)
{
	int id=node->id;
	int communityId=node->communityId;
	float s,max_s=INT_MIN;
	for(Node* nbr : node->neighbours)
	{
		if(nbr->communityId==communityId){
			s=this->nodes[nbr->id]->score;
			if(s>max_s)
				max_s=s;
		}
	}
	if(communityId!=old) // to avoid negative feedback loop
		max_s-=this->decay_factor;

	this->nodes[id]->score=max_s;
}

void HANP::CD_Extract()
{
	ComsMap::iterator i_c;
	for(int i=1;i<=this->n;i++)
	{
		i_c=this->coms_map.find(nodes[i]->communityId);
		if(i_c==this->coms_map.end())//if not in a community
		{
			list<Node*> members;
			members.push_back(this->nodes[i]);
			this->coms_map.insert(ComPair(this->nodes[i]->communityId,members));//save to map,key is communityId
		}
		else
			i_c->second.push_back(this->nodes[i]);//already,add
	}
}

void HANP::GetSequence()
{
	this->sequence=new int[this->n];
	for(int i=0;i<this->n;i++)
		sequence[i]=i+1;
}

int HANP::CountLabel(map<int,float>& count)
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

int HANP::UpdateLabel(set<Edge*>& rels)
{
	map<int,float> count;// map of <communityId,frequency>
	map<int,float>::iterator i_f;

	// count frequency of each label: si*fi^a*w(r)
	float f;
	int id,communityId;
	Node* nbr;
	for(Edge* rel :rels)
	{
		id=rel->end->id;
		communityId=rel->end->communityId;
		nbr=this->nodes[id];

		// score(id)*degree(id)^a*weight(rel)
		f=nbr->score * pow(nbr->degree,this->prefer_factor) * pow(rel->weight,this->prefer_factor);

		i_f=count.find(communityId);
		if(i_f!=count.end())
			i_f->second+=f;
		else
			count.insert(pair<int,float>(communityId,f));
	}

	return this->CountLabel(count);
}

HANP::~HANP()
{
	cout<<"detele hanp..."<<endl;
	delete[] sequence;
}

