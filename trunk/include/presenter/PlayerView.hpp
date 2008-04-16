#ifndef _SHOOTING_CUBES_PRESENTER_PLAYERVIEW_
#define _SHOOTING_CUBES_PRESENTER_PLAYERVIEW_

#include "presenter/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "utils/dictionary.hpp"
#include "data/BasicViewTypes.hpp"
#include "all_fwd.hpp"

#include <string>

namespace psc {
namespace presenter {

class PlayerView : public Object, public std::tr1::enable_shared_from_this<PlayerView>
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

    int getState() const { return conf_.I("current_state"); }
    PlayerView& flipPosition();
    PlayerView& switchCharacterState(STATE const&);
    PlayerView& switchCharacterFace(FACE const&);
    PlayerView& clearFaceState();
    PlayerView& setMap(presenter::wpMap m) { map_ = m; return *this; }

    void cycle();

protected:
    pointer_type init(std::string const&, view::pObject const&, vec2 const&);
    void faceUpdate();

protected:
    utils::map_any conf_;
    view::pMenu character_;
    presenter::wpMap map_;

    int last_garbage_;
};

}// presenter
}// psc

#endif // _SHOOTING_CUBES_PRESENTER_PLAYERVIEW_
