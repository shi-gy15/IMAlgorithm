#include"import.h"

void GetNumbers(string data_path,size_t& n,size_t& m)
{
	ifstream in(data_path);  
	if(in.is_open())
	{  
		string line;
		if(!in)exit(EXIT_FAILURE);
		string* str = new string[2];
		while(getline(in,line))
		{
			//cout<<line<<endl;
			if(line[0]!='#')
				break;
			
			if(line.substr(0,6)=="#Edges"||line.substr(0,6)=="#Nodes")
			{
				char *token=strtok(const_cast<char*>(line.c_str()),":");
				int i=0;
				while(token!=NULL)
				{
					str[i++]=token;
					token=strtok(NULL,":");
				}
				if(str[0]=="#Nodes")
					n=size_t(atoi(str[1].c_str()));
				if(str[0]=="#Edges")
					m=size_t(atoi(str[1].c_str()));
			}
		}
		delete[] str;
		in.clear();
		in.close();
	}
}

void GetNumbers(string data_path, int& n, int& m)
{
    ifstream in(data_path);
    if (in.is_open())
    {
        string line;
        if (!in)exit(EXIT_FAILURE);
        string* str = new string[2];
        while (getline(in, line))
        {
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
                    n = atoi(str[1].c_str());
                if (str[0] == "#Edges")
                    m = atoi(str[1].c_str());
            }
        }
        delete[] str;
        in.clear();
        in.close();
    }
}

void GetAdjList(string data_path,vector<Node*>& nodes,vector<Edge*>& rels,bool weighted)
{
	ifstream in(data_path);  
	string line;  
	int r=0;
	if(in.is_open())
	{
		if(!in)exit(EXIT_FAILURE); 
		int* num=new int[2];
		Edge* edge = NULL;
        Node *start, *end;
		float weight=1;
		int j=0;
		int mycount=0;
		while(getline(in,line))
		{  
			//cout<<line<<endl;
			if(line[0]!='#')
			{
				char *token=strtok(const_cast<char*>(line.c_str()),"\t");// 
				int i=0;
				while(token!=NULL)
				{
					if(i<2)
						num[i++]=atoi(token);
					else if(weighted==true)
						weight=atof(token);// weighted graph
					token=strtok(NULL,"\t");
				}
				//if(num[0]>1000) break;
				//if(num[1]>1000) continue;
				mycount++;
				//if(mycount%2==0) continue;
				if(mycount%1000==0) cout<<mycount<<endl;
                start = nodes[num[0]];
                end = nodes[num[1]];
                edge = new Edge(start, end, 0);
				edge->weight=weight;
				rels.push_back(edge);
				//start->relationships.insert(edge);
				//nodes[num[0]]->neighbours.insert(nodes[num[1]]);
				//nodes[num[1]]->pre_neighbours.insert(nodes[num[0]]);
				//nodes[num[0]]->outAdjacencies.insert(make_pair(nodes[num[1]],rel));
				//nodes[num[0]]->short_distance[num[1]]=weight;
				//nodes[num[1]]->relationships.insert(rel);
				//nodes[num[1]]->neighbours.insert(nodes[num[0]]);
				//nodes[num[0]]->pre_neighbours.insert(nodes[num[1]]);
				//nodes[num[1]]->inAdjacencies.insert(make_pair(nodes[num[0]],rel));
				//nodes[num[1]]->short_distance[num[0]]=weight;
                start->outAdjacencies[end] = edge;
                start->relationships.insert(edge);
                end->inAdjacencies[start] = edge;
                end->relationships.insert(edge);
			}
		}
		delete[] num;
		in.clear();
		in.close();
        //cout << "if " << nodes[0]->neighbours.size() << nodes[0]->pre_neighbours.size() << endl;
	}
}






