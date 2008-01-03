
#include <tr1/memory>
#include <iostream>
#include "utils/ObjectPool.hpp"
using namespace std;
using namespace tr1;

class MapM;

class MapP: public enable_shared_from_this<MapP>{
public:
    typedef shared_ptr<MapP> pointer_type;

public:
    static pointer_type create(){
        pointer_type r = psc::ObjectPool<MapP>::create();
        r->init();
        return r;
    }

public:
    MapP(){cout<<"MapP"<<endl;}
    ~MapP(){cout<<"~MapP"<<endl;}

private:
    void init();

private:
    shared_ptr<MapM> mapm_;
};

class MapM{
public:
    typedef shared_ptr<MapM> pointer_type;

public:
    static pointer_type create(shared_ptr<MapP> p){
        pointer_type r = psc::ObjectPool<MapM>::create();
        r->init(p);
        return r;
    }

public:
    MapM(){cout<<"MapM"<<endl;}
    ~MapM(){cout<<"~MapM"<<endl;}

private:
    void init(shared_ptr<MapP> owner){
        owner_ = owner;
        cout<<"MapM init"<<endl;
    }

private:
    weak_ptr<MapP> owner_;
    // DON'T USE shared_ptr !!!! it would crash !!
    // shared_ptr<MapP> owner_;
};

void MapP::init(){ mapm_ = MapM::create(shared_from_this()); cout<<"MapP init"<<endl; }

int main(){
    shared_ptr<MapP> mapp(MapP::create());
}
