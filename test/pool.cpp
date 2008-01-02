
#include "utils/ObjectPool.hpp"
#include <iostream>
#include <tr1/memory>

using namespace std;

class Test/*: public tr1::enable_shared_from_this<Test> */{
public:
    typedef tr1::shared_ptr<Test> pointer_type;
    static pointer_type create(int i){
        pointer_type result = psc::ObjectPool<Test>::create(i);
        // result->init(i);
        return result;
    }
    Test(int i){init(i*2);}
    Test(){cout << "c'tor" << endl;}
    ~Test(){cout << "d'tor: " << i_ << endl;}
    Test& go(){ cout << "go" << endl; return *this; }

private:
    void /* pointer_type */ init(int i){
        i_ = i;
        cout << "init: " << i_ << endl;
        // return shared_from_this();
    }
    int i_;
};

typedef Test::pointer_type pTest;

/* output would be:
c'tor
init: 1
a
c'tor
init: 2
b
d'tor: 2
c'tor
init: 3
go
go
go
d'tor: 1
d'tor: 3
c
*/

int main(){
    pTest t1 = Test::create(1);
    cout << 'a' << endl;
    {
        pTest t2 = Test::create(2);
        cout << 'b' << endl;
    }
    pTest t3 = Test::create(3);
    t3->go().go().go();
    psc::ObjectPool<Test>::destroy_all();
    cout << 'c' << endl;
}
