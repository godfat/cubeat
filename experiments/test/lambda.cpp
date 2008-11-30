
#include <boost/lambda/lambda.hpp>
#include <tr1/functional>
#include <iostream>

void gg(){std::cout<<"gg"<<std::endl;}

int main(){
    using namespace boost;
    using namespace lambda;

    std::tr1::function<void()> f = (std::cout << constant("f\n"));
    f();
    std::tr1::function<void()> g = gg;
    g();
    std::tr1::function<void()> h = &gg;
    h();
}
