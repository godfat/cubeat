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

    void LeftBtnClick (view::pSprite& p){ act_->setTexture("char1/test"); }
    void RightBtnClick(view::pSprite& p){ act_->setTexture("char2/test"); }
    void EnterFocus(view::pSprite& p, int a, int b);
    void LeaveFocus(view::pSprite& p, int a, int b);
    void ShowPanel(view::pSprite& p);
    void HidePanel(view::pSprite& p);
    void ShowChar(view::pSprite& p);
    void HideChar(view::pSprite& p);

    void ShowPos(view::pSprite& p);

private:
    presenter::pStage stage_;
    view::pScene      scene_;

    lua_State* L_;

    view::pSprite       bg_;
    view::pSprite       act_;
    view::pSprite       btn_change_char_;
    view::pSprite       btn_visible_panel_;
    view::pSprite       btn_visible_char_;
    view::pSpriteText   text_;

    view::pSprite       btn_test_cursor_;
    view::pSpriteText   text_cursor_;
};
} //psc

#endif // _SHOOTING_CUBES_TEST_UI_
