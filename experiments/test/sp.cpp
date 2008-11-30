
#include <iostream>
#include <tr1/memory>

int main(){
    std::cout << sizeof(std::tr1::shared_ptr<int>) << std::endl;
}
