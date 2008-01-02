
#include "view/Cube.hpp"
#include "view/Map.hpp"
#include "IrrDevice.hpp"

//test code
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace view;

Cube* Cube::clone() const
{
    Cube* obj = new Cube(*this);
    return obj;
}

void Cube::init(Map* parent)
{
    SceneObject::init(parent);

    //ugly test code
    parent->addCube( this );
}

void Cube::ownerHit()
{
    std::cout << "HAHAHA.\n";
}

void Cube::enemyHit()
{
    std::cout << "HEHEHE.\n";
}
