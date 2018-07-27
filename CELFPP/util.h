#ifndef UTIL_H
#define UTIL_H

#include<fstream> 
#include<iostream> 
#include<sstream>
#include<iterator>     
#include<string>
#include<cstdlib>
#include<ctime>
#include<list>
#include<set>
#include<unordered_set>
#include<map>
#include<unordered_map>
#include<vector>
#include<queue>
#include<algorithm>
#include <ppl.h>

using namespace std;
using namespace concurrency;

string time();

int cmp_float(const pair<int,float>& x, const pair<int,float>& y);

int cmp_int(const pair<int,int>& x, const pair<int,int>& y);

void int2string(int src,string* dest);

void swp(int& i,int& j);

#endif