
#include <iostream>
using namespace std;

class Base{
public:
    Base(){}
    virtual Base* init() = 0;
};

class Derived: public Base{
public:
    Derived(){}
    virtual Derived* init(){ cout << "derived" << endl; return this; }
};

int main(){
    Base* b = (new Derived)->init();
    delete b;
}
