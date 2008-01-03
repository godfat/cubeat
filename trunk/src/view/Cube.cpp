
#include "view/Cube.hpp"
#include "view/Map.hpp"
#include "IrrDevice.hpp"

//test code
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;

Cube* Cube::clone() const
{
    Cube* obj = new Cube(*this);
    return obj;
}

void Cube::init(pMap& parent)
{
    SceneObject::init(parent);

    parent->addCube( std::tr1::static_pointer_cast<Cube>(shared_from_this()) );
}

void Cube::ownerHit()
{
    std::cout << "HAHAHA.\n";
}

void Cube::enemyHit()
{
    std::cout << "HEHEHE.\n";
}
