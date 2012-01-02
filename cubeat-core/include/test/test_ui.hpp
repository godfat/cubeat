#ifndef _SHOOTING_CUBES_TEST_UI_
#define _SHOOTING_CUBES_TEST_UI_

#include <boost/tr1/memory.hpp>

//for lua scripting, and this has to be global.. not in any namespaces
struct lua_State;

namespace psc {
namespace view {
class Scene;
class Sprite;
typedef std::tr1::shared_ptr<Scene>  pScene;
typedef std::tr1::shared_ptr<Sprite> pSprite;
} //view
namespace presenter {
class Stage;
typedef std::tr1::shared_ptr<Stage>  pStage;
} //presenter

class TestUI{
public:
    TestUI();
    ~TestUI();

    void init();

    void cycle();
    view::pScene getUIScene() { return scene_; }

private:
    presenter::pStage stage_;
    view::pScene      scene_;
    view::pSprite     test_sprite_;

    lua_State* L_;
};
} //psc

#endif // _SHOOTING_CUBES_TEST_UI_
