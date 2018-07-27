#include <iostream>
#include <vector>
using namespace std;

template <class T>
class A {
public:
    T * pt;
};

int main() {
    A<int> x;
    //A<A> y;
    A<A<A<A<A<int>*>>>> z;
}