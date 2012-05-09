#include "view/Scene.hpp"
#include "presenter/game/Puzzle.hpp"

using namespace psc;
using namespace view;
using namespace presenter;
using namespace game;

extern "C"{
#include "script/ui/puzzle/bindings.h"
}

pScene* get_puzzle_scene(Puzzle* self) {
    pScene* s = new pScene;
    *s = self->getUIScene();
    return s;
}
