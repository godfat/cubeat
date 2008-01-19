
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

Cube& Cube::setOwnerHit( HitCallback const& ownerhit )
{
    owner_hit_ = ownerhit;
    return *this;
}

Cube& Cube::setEnemyHit( HitCallback const& enemyhit )
{
    enemy_hit_ = enemyhit;
    return *this;
}

void Cube::ownerHit()
{
    if (owner_hit_) owner_hit_();
}

void Cube::enemyHit()
{
    if (enemy_hit_) enemy_hit_();
}
