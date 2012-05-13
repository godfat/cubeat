#ifndef _SHOOTING_CUBES_PRESENTER_PLAYERVIEW_
#define _SHOOTING_CUBES_PRESENTER_PLAYERVIEW_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "data/BasicViewTypes.hpp"
#include "all_fwd.hpp"

#include <string>

namespace psc {

namespace ctrl {
class Input;
}

namespace presenter {

class PlayerView : public std::tr1::enable_shared_from_this<PlayerView>
{
public:
    enum STATE { STAND, ATTACK, HIT, NONE };
    enum FACE  { NORMAL, GOOD, BAD };

public:
    typedef std::tr1::shared_ptr<PlayerView> pointer_type;
    static pointer_type create(std::string const& path,
                               view::pObject const& parent,
                               vec2 const& pos) {
        return utils::ObjectPool<PlayerView>::create()->init(path, parent, pos);
    }

    PlayerView();
    ~PlayerView();

    //int getState() const { return conf_.I("current_state"); }
    int getState() const { return static_cast<int>(current_state_); }
    PlayerView& flipPosition();
    PlayerView& switchCharacterState(STATE const&);
    PlayerView& switchCharacterFace(FACE const&);
    PlayerView& clearFaceState();
    PlayerView& setMap(presenter::wpMap m) { map_ = m; return *this; }
    PlayerView& setInput(ctrl::Input* i)   { input_ = i; return *this; }
    view::pMenu& getView() { return character_; }

    void cycle();

protected:
    pointer_type init(std::string const&, view::pObject const&, vec2 const&);
    void faceUpdate();

protected:
    utils::map_any conf_;
    view::pMenu character_;
    presenter::wpMap map_;
    ctrl::Input* input_;

    int last_garbage_;
    int last_attack_;
    STATE current_state_;
    FACE  current_face_;
    int face_pos_idx_;

    struct FaceState {
        std::string tex;
        int gdeco;
        int bdeco;
    };

    struct CharState {
        std::string anim;
        int face_visible;
        std::string sound;
    };

    std::vector<FaceState> faces_;
    std::vector<CharState> states_;
    std::vector<vec2> face_pos_;
};

}// presenter
}// psc

#endif // _SHOOTING_CUBES_PRESENTER_PLAYERVIEW_
