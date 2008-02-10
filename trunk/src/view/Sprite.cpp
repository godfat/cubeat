
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "EventDispatcher.hpp"
#include "Button.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "CustomAnimator.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace easing;
using namespace accessor;

using namespace psc;
using namespace view;
using std::tr1::function;


CallbackDelegate& CallbackDelegate::operator =(std::tr1::function<void(pSprite&)> const& cb)
{
    ctrl::EventDispatcher::i().subscribe_obj_event(cb, subscribed_btn_, owner_.lock());
    return *this;
}

CallbackDelegate& CallbackDelegate::setButton(ctrl::Button const* btn)
{
    subscribed_btn_ = btn;
    return *this;
}

void CallbackDelegate::setOwner(pSprite const& owner)
{
    owner_ = owner;
}

////////////////////////////////////////////////////////////////////////////////////

Sprite::Sprite(std::string const& name, bool const& center)
    :Object(name), center_(center), center_aligned_plane_(0), upperleft_aligned_plane_(0)
{
    upperleft_aligned_plane_ = smgr_->getMesh( "rc/model/plane_orig.x" );
}

/*
    TODO:
    still some problem with material settings(alpha). I'll fix that later.
*/

pSprite Sprite::init(pObject const& parent, int const& w, int const& h)
{
    if( name_.size() < 1 ) {
        Object::init(parent);
        return shared_from_this();
    }
    std::ostringstream oss;
    oss << "rc/texture/" << name_ << ".png";

    video::IVideoDriver* driver = smgr_->getVideoDriver();

    size_.Width  = w;
    size_.Height = h;

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setTexture(0, driver->getTexture(oss.str().c_str()));

    mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    mat.DiffuseColor.set(255,255,255,255);

    setupMeshBase(parent);
    body_->getMaterial(0) = mat;

    press_.setOwner( shared_from_this() );
    scene_ = parent->scene();

    return shared_from_this();
}

void Sprite::setupMeshBase(pObject const& parent)
{
    IMeshManipulator* mani = smgr_->getMeshManipulator();
    thismesh_ = mani->createMeshCopy( upperleft_aligned_plane_ );
    mani->scaleMesh( thismesh_, vector3df(size_.Width / 100, size_.Height / 100, 1) );

    if( center_ ) {
        matrix4 mat; mat.setTranslation( vector3df(-size_.Width/2, size_.Height/2, 0) );
        mani->transformMesh( thismesh_, mat );
    }

    body_ = smgr_->addMeshSceneNode( thismesh_, parent->body(), -1, vector3df(0,0,-50) );
    body_->setName( name_.c_str() );
}

void Sprite::adjust_texcoord_for_hand_made_texture(int const& w, int const& h)
{
    //texture ratio adjustment because the texture is sized at power of 2.
    //have to do this for hand made texture like Movie Texture or Text Texture.
    float x = 1, y = 1;
    while( x < w ){x*=2;} float tex_coord_ratio_x = w / x;
    while( y < h ){y*=2;} float tex_coord_ratio_y = h / y;

    S3DVertex* ptr = (S3DVertex*)thismesh_->getMeshBuffer(0)->getVertices();
    ptr[0].TCoords.Y = tex_coord_ratio_y;
    ptr[1].TCoords.X = tex_coord_ratio_x; ptr[1].TCoords.Y = tex_coord_ratio_y;
    ptr[3].TCoords.X = tex_coord_ratio_x;
}

Sprite& Sprite::setDepth(float d)
{
    vector2df pos2d = get<Pos2D>();
    set<Pos3D>(vector3df(pos2d.X, pos2d.Y, d));
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
    set<Pos2D>(vector2df(x, -y));
    return *this;
}

Sprite& Sprite::moveTween(int x, int y, int delay_ms, function<void()> cb, int delay)
{
    vector2df newpos = vector2df(x, -y);
    tween<Linear, Pos2D>(newpos, delay_ms, false, cb, delay);
    return *this;
}

Sprite* Sprite::clone() const
{
    Sprite* obj = new Sprite(*this);
    return obj;
}

CallbackDelegate& Sprite::onPress(ctrl::Button const* btn)
{
    return press_.setButton(btn);
}

Sprite::~Sprite()
{
}
