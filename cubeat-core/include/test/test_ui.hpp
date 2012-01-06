#ifndef _SHOOTING_CUBES_TEST_UI_
#define _SHOOTING_CUBES_TEST_UI_

#include <boost/tr1/memory.hpp>

//for lua scripting, and this has to be global.. not in any namespaces
struct lua_State;

namespace psc {
namespace view {
class Scene;
class Sprite;
class SpriteText;
typedef std::tr1::shared_ptr<Scene>  pScene;
typedef std::tr1::shared_ptr<Sprite> pSprite;
typedef std::tr1::shared_ptr<SpriteText> pSpriteText;
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

    void LeftBtnClick(view::pSprite& p) { act_->setTexture("char1/test"); }
    void RightBtnClick(view::pSprite& p) { act_->setTexture("char2/test"); }

private:
    presenter::pStage stage_;
    view::pScene      scene_;
    view::pSprite     test_sprite_;

    lua_State* L_;

    view::pSprite   act_;
    view::pSprite   btn_;
};
} //psc

#endif // _SHOOTING_CUBES_TEST_UI_
