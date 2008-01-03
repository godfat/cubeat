
#include "utils/ObjectPool.hpp"
#include <iostream>
#include <tr1/memory>

using namespace std;

class Test/*: public tr1::enable_shared_from_this<Test> */{
public:
    typedef tr1::shared_ptr<Test> pointer_type;
    static pointer_type create(int i1, int i2, int i3, int i4, int i5){
        return psc::ObjectPool<Test>::create(i1, i2, i3, i4, i5);
    }
    Test(int i1, int i2, int i3, int i4, int i5):
        i1_(i1), i2_(i2), i3_(i3), i4_(i4), i5_(i5)
    {cout << "c'tor: " << i1_ << i2_ << i3_ << i4_ << i5_ << endl;}
    ~Test(){cout << "d'tor: " << i1_ << i2_ << i3_ << i4_ << i5_ << endl;}
    Test& go(){ cout << "go" << endl; return *this; }

private:
    int i1_, i2_, i3_, i4_, i5_;
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
    pTest t1 = Test::create(1,2,3,4,5);
    cout << 'a' << endl;
    {
        pTest t2 = Test::create(2,3,4,5,6);
        cout << 'b' << endl;
    }
    pTest t3 = Test::create(3,4,5,6,7);
    t3->go().go().go();
    psc::ObjectPool<Test>::destroy_all();
    cout << 'c' << endl;
}
