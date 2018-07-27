#ifndef HANP_H
#define HANP_H

#include"detection.h"
#include"lpa.h"

class HANP : public Detection{
private:
	int max_T;

	float prefer_factor;
	float decay_factor;

	int* sequence;

public:
	HANP(float prefer_factor, float decay_factor, int max_T);
	
	void Proceed();

	void CD_Start();
	void CD_Select(); 
	void CD_Fluctuate();
	void CD_Update(Node* node, int old);
	void CD_Extract();
	
	~HANP();

private:

	void GetSequence();
	int UpdateLabel(set<Edge*>& rels);
	int CountLabel(map<int,float>& count);

};

#endif