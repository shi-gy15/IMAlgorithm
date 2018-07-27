#include"header.h"

void GetMethod(int method, Detection** d)
{
	int max_T;
	float prefer_factor,decay_factor;

	switch(method)
	{
	case 1:
		max_T=6;
		*d = new LPA(max_T);
		break;
	case 2:
		prefer_factor=0.1;
		decay_factor=0.05;
		max_T=10;
		*d = new HANP(prefer_factor,decay_factor,max_T);
		break;
	default:
		break;
	}
	(*d)->method = method;
}