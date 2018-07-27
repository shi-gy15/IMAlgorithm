#ifndef LPA_H
#define LPA_H

#include"detection.h"

class LPA : public Detection{

private:

	int max_T;
	int* sequence;

public:
	
	LPA(int max_T);

	void Proceed();

	void CD_Select(); //shuffle nodes 
	void CD_Fluctuate(); // update labels
	void CD_Extract();

	~LPA();

private:
	
	void GetSequence();
	int UpdateLabel(set<Edge*>& rels); 
	int CountLabel(map<int,float>& count);

};

#endif