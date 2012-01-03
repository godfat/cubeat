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

    void onMoveRight(view::pSprite& p);
    void onMoveLeft(view::pSprite& p);
    void onSelectChar1(view::pSprite& p);
    void onSelectChar2(view::pSprite& p);
    void onNoRed(view::pSprite& p);
    void onFullRed(view::pSprite& p);
    void onHalfAlpha(view::pSprite& p);
    void onFullAlpha(view::pSprite& p);
    void onPressSprite(view::pSprite& p);

private:
    presenter::pStage stage_;
    view::pScene      scene_;
    view::pSprite     test_sprite_;

    view::pSprite   sprite_;
    view::pSprite   button_01_;
    view::pSprite   button_02_;
    view::pSprite   button_03_;
    view::pSprite   button_04_;
    view::pSprite   button_05_;
    view::pSprite   button_06_;
    view::pSprite   button_07_;
    view::pSprite   button_08_;
    view::pSprite   button_09_;
    view::pSprite   button_10_;
    view::pSprite   button_11_;
    view::pSprite   button_12_;

    lua_State* L_;
};
} //psc

#endif // _SHOOTING_CUBES_TEST_UI_
