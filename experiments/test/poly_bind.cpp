
#include <iostream>
#include <tr1/memory>
#include <tr1/functional>

struct Base{
    virtual void poly(){ std::cout << "Base" << std::endl; }
    virtual ~Base(){}
};

struct Derived: public Base{
    void poly(){ std::cout << "Derived" << std::endl; }
};

int main(){
    std::tr1::shared_ptr<Base> pb(new Derived);
    std::tr1::bind(&Base::poly, pb)();
}
