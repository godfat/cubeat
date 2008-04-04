#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "CallbackDelegate.hpp"
#include "all_fwd.hpp"

namespace irr {
namespace scene {
class IMesh;
}
}

namespace psc {

namespace ctrl {
class Button;
class Crosshair;
}

namespace view {

/* TODO:
   This thing should be "upgrade" to Object level.
   It's quite critical to the view hierarchy.     */

class Sprite : public Object
{
public:
    typedef std::tr1::shared_ptr< Sprite > pointer_type;
    static pointer_type create(std::string const& name,
                               pObject const& parent,
                               int const& w = 100,
                               int const& h = 100,
                               bool const& center = false)
    {
        return utils::ObjectPool< Sprite >::create(name, center)->init(parent, w, h);
    }

    static pointer_type create(pObject const& parent,
                               int const& w = 100,
                               int const& h = 100,
                               bool const& center = false)
    {
        return utils::ObjectPool< Sprite >::create("unnamed", center)->init(parent, w, h);
    }

    Sprite(std::string const& name, bool const& center);

    virtual Sprite* clone() const;
    virtual Sprite& setCenterAligned(bool const&);
    virtual Sprite& setDepth(float);
    virtual Sprite& moveTo(int,int);
    virtual Sprite& moveTween(int,int,int,std::tr1::function<void()> cb = 0, int delay = 0);

    //Maybe this feature should move to another class and use multiple inheritance.
    ctrl::CallbackDelegate& onPress  (ctrl::Button const*);
    ctrl::CallbackDelegate& onRelease(ctrl::Button const*);
    ctrl::CallbackDelegate& onUp     (ctrl::Button const*);
    ctrl::CallbackDelegate& onDown   (ctrl::Button const*);
    ctrl::CallbackDelegate& onHit    (ctrl::Button const*);   //this will not be triggered by EventD.
    ctrl::FocusDelegate&    onEnterFocus(ctrl::Input const*);
    ctrl::FocusDelegate&    onLeaveFocus(ctrl::Input const*);
    ctrl::FocusDelegate&    onHoverIn(ctrl::Input const*);    //this will not be triggered by EventD.
    ctrl::FocusDelegate&    onHoverOut(ctrl::Input const*);   //this will not be triggered by EventD.
    void                    triggerHit(ctrl::Button const*);  //this will be used by view::Scene
    void                    triggerHoverIn(ctrl::Input const*);   //this will be used by view::Scene
    void                    triggerHoverOut(ctrl::Input const*);  //this will be used by view::Scene
    //End of maybe

    virtual ~Sprite();

protected:
    pointer_type init(pObject const&, int const&, int const&);
    void setupMeshBase(pObject const&);
    void adjust_texcoord_for_hand_made_texture(int const&, int const&);

    //Maybe this feature should move to another class and use multiple inheritance.
    inline ctrl::CallbackDelegate& onButtonEvent(ctrl::Button const*, ctrl::BSTATE const&);
    inline ctrl::CallbackDelegate& manualButtonEvent(ctrl::Button const*);
    inline ctrl::FocusDelegate&    onFocusEvent (ctrl::Input const*, ctrl::FSTATE const&);
    inline ctrl::FocusDelegate&    manualFocusEvent(ctrl::Input const*, ctrl::FSTATE const&);
    //End of maybe

protected:
    //Maybe this feature should move to another class and use multiple inheritance.
    std::list< ctrl::CallbackDelegate > delegates_;
    std::list< ctrl::FocusDelegate > delegates_focus_;
    std::list< ctrl::CallbackDelegate > delegates_manual_;
    std::list< ctrl::FocusDelegate > delegates_focus_manual_;
    //End of maybe

    bool center_;

    irr::core::dimension2df size_;
    irr::scene::IMesh* center_aligned_plane_;
    irr::scene::IMesh* upperleft_aligned_plane_;
    irr::scene::IMesh* thismesh_;
};

} //view
} //psc

#endif
