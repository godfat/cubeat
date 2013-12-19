#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "CallbackDelegate.hpp"
#include "utils/dictionary.hpp"

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
                               int const& w = -1,
                               int const& h = -1,
                               bool const& center = false)
    {
        return utils::ObjectPool< Sprite >::create(name, center)->init(parent, w, h);
    }

    static pointer_type create(pObject const& parent,
                               int const& w = -1,
                               int const& h = -1,
                               bool const& center = false)
    {
        return utils::ObjectPool< Sprite >::create("unnamed", center)->init(parent, w, h);
    }

    Sprite(std::string const& name, bool const& center);

    template <class Accessor>
    Sprite& set(typename Accessor::value_type const& value) {
        Accessor::set(body_, value);
        return *this;
    }

    virtual Sprite& setCenterAligned(bool const&);
    virtual Sprite& setDepth(float);
    virtual Sprite& setTexture(std::string const& path);
    virtual Sprite& moveTo(int,int);
    virtual Sprite& moveTween(int,int,int,std::tr1::function<void()> cb = 0, int delay = 0);
    virtual Sprite& textureFlipH();
    virtual Sprite& textureFlipV();
    virtual Sprite& lazyFixAlphaArtifact();
    virtual Sprite& setBlending(std::string const& mt_name);

    //normally you don't want to do this.
    virtual Sprite& removeTexture();

    virtual bool const& isCenterAligned() const { return center_; }

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
    irr::video::SMaterial create_std_material_for_sprite();
    void setupMeshAndNode(irr::scene::IMesh*&, irr::scene::ISceneNode*&, pObject const&,
                          irr::core::dimension2df const&, bool const& center = false,
                          std::string const& debug_name = "sprite_mesh" );
    void adjust_texcoord_for_hand_made_texture(irr::scene::IMesh const*, int const&, int const&);

    void setupTextureSingleAndSize(int const&, int const&);
    void setupTextureFromSheetAndSize(size_t const&, int const&, int const&);
    size_t locateSheetNumber(std::string const&) const;
    void adjustSheetUV(utils::map_any const&);

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
    irr::scene::IMesh* thismesh_;
    size_t sheet_index_;

    //A shared base plane to provide simple mesh for sprite usage
    static irr::scene::IMesh* sprite_plane_ptr_;
    static irr::scene::SMesh  sprite_plane_;

    //A shared spritesheet data
    static std::vector< utils::map_any > spritesheets_;
};

} //view
} //psc

#endif
