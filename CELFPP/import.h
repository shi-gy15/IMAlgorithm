#ifndef IMPORT_H
#define IMPORT_H

#include"network.h"



void GetNumbers(string data_path, size_t& n, size_t& m);
void GetNumbers(string data_path, int& n, int& m);
	
void GetAdjList(string data_path,vector<Node*>& nodes,vector<Edge*>& rels,bool weighted);



#endif