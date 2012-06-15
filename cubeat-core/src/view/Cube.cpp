
#include "view/Cube.hpp"
#include "view/Map.hpp"
#include "view/Scene.hpp"
#include "IrrDevice.hpp"

//test code
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using std::tr1::static_pointer_cast;

pCube Cube::init(pMap& parent)
{
    SceneObject::init(parent);

    pCube self = static_pointer_cast<Cube>( shared_from_this() );
    parent->addCube( self );
    return self;
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
