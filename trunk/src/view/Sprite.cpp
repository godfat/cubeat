
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "Button.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"

#include <sstream>
#include <boost/foreach.hpp>
#include <algorithm>

using std::tr1::function;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using namespace easing;
using namespace accessor;
using std::tr1::static_pointer_cast;

IMesh* Sprite::sprite_plane_ptr_ = 0;
SMesh  Sprite::sprite_plane_;

Sprite::Sprite(std::string const& name, bool const& center)
    :Object(name), center_(center), thismesh_(0)
{
    if( !sprite_plane_ptr_ ) {
        S3DVertex vertices[4];
        u16 indices[6] = {0,2,3,3,1,0};

        vertices[0].Pos = vector3df(0,-1,0);
        vertices[1].Pos = vector3df(1,-1,0);
        vertices[2].Pos = vector3df(0, 0,0);
        vertices[3].Pos = vector3df(1, 0,0);

        vertices[0].Normal = vector3df(0,0,-1);
        vertices[1].Normal = vector3df(0,0,-1);
        vertices[2].Normal = vector3df(0,0,-1);
        vertices[3].Normal = vector3df(0,0,-1);

        vertices[0].TCoords = vector2df(0,1);
        vertices[1].TCoords = vector2df(1,1);
        vertices[2].TCoords = vector2df(0,0);
        vertices[3].TCoords = vector2df(1,0);

        SMeshBuffer* buf = new SMeshBuffer();
        buf->append(vertices, 4, indices, 6);

        sprite_plane_.addMeshBuffer( buf );
        sprite_plane_.recalculateBoundingBox();

        sprite_plane_ptr_ = &sprite_plane_;
        buf->drop(); //the addMeshBuffer method will grab it, so we can drop this ptr.
    }
}

pSprite Sprite::init(pObject const& parent, int const& w, int const& h)
{
    if( name_.size() < 1 ) {
        Object::init(parent);
        return std::tr1::static_pointer_cast<Sprite>(shared_from_this());
    }
    setupSceneAndManager(parent);

    std::ostringstream oss;
    oss << "rc/texture/" << name_ << ".png";

    video::IVideoDriver* driver = smgr_->getVideoDriver();

    size_.Width  = w;
    size_.Height = h;

    SMaterial mat = create_std_material_for_sprite();
    mat.setTexture(0, driver->getTexture(oss.str().c_str()));

    setupMeshAndNode(thismesh_, body_, parent, size_, center_, name_);

    body_->getMaterial(0) = mat;

    pSprite self = static_pointer_cast<Sprite>( shared_from_this() );
    scene()->addPickMapping( body_, self );

    return self;
}

SMaterial Sprite::create_std_material_for_sprite()
{
    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setFlag(video::EMF_NORMALIZE_NORMALS, true);
    mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;
    mat.DiffuseColor.set(255, 255, 255, 255);

    return mat;
}

void Sprite::setupMeshAndNode(IMesh*& out_mesh, ISceneNode*& out_node,
     pObject const& parent, dimension2df const& size, bool const& center,
     std::string const& debug_name)
{
    IMeshManipulator* mani = smgr_->getMeshManipulator();
    out_mesh = mani->createMeshCopy( sprite_plane_ptr_ );
    mani->scaleMesh( out_mesh, vector3df(size.Width, size.Height, 1) );

    if( center ) {
        matrix4 mat;
        mat.setTranslation( vector3df(-size.Width/2, size.Height/2, 0) );
        mani->transformMesh( out_mesh, mat );
    }

    out_node = smgr_->addMeshSceneNode( out_mesh, parent->body(), -1, vector3df(0,0,0) );
    out_node->setAutomaticCulling(EAC_OFF);
    out_node->setName( debug_name.c_str() );
}

void Sprite::adjust_texcoord_for_hand_made_texture(IMesh const* mesh, int const& w, int const& h)
{
    //texture ratio adjustment because the texture is sized at power of 2.
    //have to do this for hand made texture like Movie Texture or Text Texture.
    float x = 1, y = 1;
    while( x < w ){x*=2;} float tex_coord_ratio_x = w / x;
    while( y < h ){y*=2;} float tex_coord_ratio_y = h / y;

    S3DVertex* ptr = static_cast<S3DVertex*>(mesh->getMeshBuffer(0)->getVertices());
    ptr[0].TCoords.Y = tex_coord_ratio_y;
    ptr[1].TCoords.X = tex_coord_ratio_x; ptr[1].TCoords.Y = tex_coord_ratio_y;
    ptr[3].TCoords.X = tex_coord_ratio_x;
}

Sprite& Sprite::textureFlipH()
{
    if( thismesh_ ) {
        S3DVertex* ptr = static_cast<S3DVertex*>(thismesh_->getMeshBuffer(0)->getVertices());
        std::swap(ptr[0].TCoords, ptr[1].TCoords);
        std::swap(ptr[2].TCoords, ptr[3].TCoords);
    }
    return *this;
}

Sprite& Sprite::textureFlipV()
{
    if( thismesh_ ) {
        S3DVertex* ptr = static_cast<S3DVertex*>(thismesh_->getMeshBuffer(0)->getVertices());
        std::swap(ptr[0].TCoords, ptr[2].TCoords);
        std::swap(ptr[1].TCoords, ptr[3].TCoords);
    }
    return *this;
}

Sprite& Sprite::setDepth(float d)
{
    vector2df pos2d = get<Pos2D>();
    set<Pos3D>(vector3df(pos2d.X, -pos2d.Y, d));
    return *this;
}

Sprite& Sprite::setCenterAligned(bool const& center)
{
    if( center_ == center ) return *this;
    center_ = center;
    matrix4 mat;
    if( center_ )
        mat.setTranslation( vector3df(-size_.Width/2, size_.Height/2, 0) );
    else
        mat.setTranslation( vector3df(+size_.Width/2, -size_.Height/2, 0) );
    IMeshManipulator* mani = smgr_->getMeshManipulator();
    mani->transformMesh( thismesh_, mat );
    return *this;
}

Sprite& Sprite::moveTo(int x, int y)
{
    //body_->setPosition(vector3df(x-320.f+51.f, -y+240.f-21.f, body_->getPosition().Z));
    set<Pos2D>(vector2df(x, y));
    return *this;
}

Sprite& Sprite::moveTween(int x, int y, int duration, function<void()> cb, int delay)
{
    vector2df newpos = vector2df(x, y);
    tween<Linear, Pos2D>(newpos, duration, 0, cb, delay);
    return *this;
}

Sprite* Sprite::clone() const
{
    Sprite* obj = new Sprite(*this);
    return obj;
}


/// Maybe this feature should move to another class and use multiple inheritance.

ctrl::CallbackDelegate& Sprite::onPress(ctrl::Button const* btn) {
    return onButtonEvent(btn, ctrl::BTN_PRESS);
}

ctrl::CallbackDelegate& Sprite::onRelease(ctrl::Button const* btn) {
    return onButtonEvent(btn, ctrl::BTN_RELEASE);
}

ctrl::CallbackDelegate& Sprite::onDown(ctrl::Button const* btn) {
    return onButtonEvent(btn, ctrl::BTN_DOWN);
}

ctrl::CallbackDelegate& Sprite::onUp(ctrl::Button const* btn) {
    return onButtonEvent(btn, ctrl::BTN_UP);
}

ctrl::CallbackDelegate&
Sprite::onButtonEvent(ctrl::Button const* btn, ctrl::BSTATE const& state)
{
    BOOST_FOREACH(ctrl::CallbackDelegate& cd, delegates_)
        if( btn == cd.subscribed_btn() && state == cd.subscribed_state() )
            return cd;

    ctrl::CallbackDelegate cd(static_pointer_cast<Sprite>(shared_from_this()), btn, state);
    delegates_.push_back( cd );
    return delegates_.back();
}

/// MANUAL HIT ///
ctrl::CallbackDelegate& Sprite::onHit(ctrl::Button const* btn) {
    return manualButtonEvent(btn);
}

void Sprite::triggerHit(ctrl::Button const* btn) {
    BOOST_FOREACH(ctrl::CallbackDelegate& cdm, delegates_manual_)
        if( btn == cdm.subscribed_btn() )
            cdm.call_manually();
}

ctrl::CallbackDelegate& Sprite::manualButtonEvent(ctrl::Button const* btn)
{
    BOOST_FOREACH(ctrl::CallbackDelegate& cdm, delegates_manual_)
        if( btn == cdm.subscribed_btn() )
            return cdm;

    ctrl::CallbackDelegate
        cdm(static_pointer_cast<Sprite>(shared_from_this()), btn, ctrl::BTN_PRESS, true);
    delegates_manual_.push_back( cdm );                           //^^ useless
    return delegates_manual_.back();
}
/// END OF MANUAL HIT ///

ctrl::FocusDelegate& Sprite::onEnterFocus(ctrl::Input const* input) {
    return onFocusEvent(input, ctrl::FOCUS_ENTER);
}

ctrl::FocusDelegate& Sprite::onLeaveFocus(ctrl::Input const* input) {
    return onFocusEvent(input, ctrl::FOCUS_LEAVE);
}

ctrl::FocusDelegate&
Sprite::onFocusEvent(ctrl::Input const* input, ctrl::FSTATE const& state)
{
    BOOST_FOREACH(ctrl::FocusDelegate& fd, delegates_focus_)
        if( input == fd.subscribed_input() && state == fd.subscribed_state() )
            return fd;

    ctrl::FocusDelegate fd(static_pointer_cast<Sprite>(shared_from_this()), input, state);
    delegates_focus_.push_back( fd );
    return delegates_focus_.back();
}

/// MANUAL HOVER IN / OUT ///
ctrl::FocusDelegate& Sprite::onHoverIn(ctrl::Input const* input ) {
    return manualFocusEvent(input, ctrl::FOCUS_ENTER);
}

ctrl::FocusDelegate& Sprite::onHoverOut(ctrl::Input const* input ) {
    return manualFocusEvent(input, ctrl::FOCUS_LEAVE);
}

void Sprite::triggerHoverIn(ctrl::Input const* input) {
    BOOST_FOREACH(ctrl::FocusDelegate& fdm, delegates_focus_manual_)
        if( input == fdm.subscribed_input() && ctrl::FOCUS_ENTER == fdm.subscribed_state() )
            fdm.call_manually();
}

void Sprite::triggerHoverOut(ctrl::Input const* input) {
    BOOST_FOREACH(ctrl::FocusDelegate& fdm, delegates_focus_manual_)
        if( input == fdm.subscribed_input() && ctrl::FOCUS_LEAVE == fdm.subscribed_state() )
            fdm.call_manually();
}

ctrl::FocusDelegate&
Sprite::manualFocusEvent(ctrl::Input const* input, ctrl::FSTATE const& state)
{
    BOOST_FOREACH(ctrl::FocusDelegate& fdm, delegates_focus_manual_)
        if( input == fdm.subscribed_input() && state == fdm.subscribed_state() )
            return fdm;

    ctrl::FocusDelegate
        fdm(static_pointer_cast<Sprite>(shared_from_this()), input, state, true);
    delegates_focus_manual_.push_back( fdm );
    return delegates_focus_manual_.back();
}
/// END OF MANUAL HOVER IN / OUT ///

/// End of Maybe

Sprite::~Sprite()
{
    if( thismesh_ )
        thismesh_->drop();
}
