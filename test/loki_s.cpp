
#include <loki/Singleton.h>
#include <iostream>

using namespace std;

class Test{
public:
    Test(){cout << "c'tor" << endl;}
    ~Test(){cout << "d'tor" << endl;}
};

int main(){
    cout << '1' << endl;
    Loki::SingletonHolder<Test, Loki::CreateUsingNew, Loki::DeletableSingleton>::Instance();
    cout << '2' << endl;
    Loki::DeletableSingleton<Test>::GracefulDelete();
    cout << '3' << endl;
    Loki::SingletonHolder<Test, Loki::CreateUsingNew, Loki::DeletableSingleton>::Instance();
    cout << '4' << endl;
}
