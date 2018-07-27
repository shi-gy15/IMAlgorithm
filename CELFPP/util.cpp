#include"util.h"

using namespace std;

int cmp_float(const pair<int,float>& x, const pair<int,float>& y)
{
	return x.second > y.second;
}

int cmp_int(const pair<int,int>& x, const pair<int,int>& y)
{  
	return x.second > y.second;  
} 

string time()
{
	time_t tt = time(NULL);
	tm* t= localtime(&tt);
	cout<<t->tm_year + 1900<<t->tm_mon+ 1<<t->tm_mday<<t->tm_hour<<t->tm_min<<t->tm_sec<<endl;
	stringstream ss;
	ss<<t->tm_year + 1900<<"-"<<t->tm_mon+ 1<<"-"<<t->tm_mday<<"-"<<t->tm_hour<<"-"<<t->tm_min<<"-"<<t->tm_sec;
	return ss.str();	
}

void swp(int& i,int& j)
{
	int tmp=i;
	i=j;
	j=tmp;
}

void int2string(int src,string* dest)
{
	stringstream ss;
	ss<<src;
	ss>>(*dest);
}


