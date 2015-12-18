
#include "IrrDevice.hpp"
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "Button.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "Conf.hpp"

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
std::vector< utils::map_any > Sprite::spritesheets_;

Sprite::Sprite(std::string const& name, bool const& center)
    :Object(name), center_(center), thismesh_(0), sheet_index_(0)
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
    if( spritesheets_.empty() ) {
        spritesheets_.push_back( Conf::i().config_of("spritesheet0") );
    }
}

pSprite Sprite::init(pObject const& parent, int const& w, int const& h)
{
    if( name_.size() < 1 ) {
        Object::init(parent);
        return std::tr1::static_pointer_cast<Sprite>(shared_from_this());
    }
    setupSceneAndManager(parent);

    setupMeshAndNode(thismesh_, body_, parent->body(), dimension2df(100, 100), center_, name_);

    size_t i = locateSheetNumber(name_);
    if( i < spritesheets_.size() ) {
        setupTextureFromSheetAndSize(i, w, h);
    } else {
        setupTextureSingleAndSize(w, h);
    }
    Size2D::set(body_, vec2(size_.Width, size_.Height));

    pSprite self = static_pointer_cast<Sprite>( shared_from_this() );
    scene()->addPickMapping( body_, self );

    return self;
}

size_t Sprite::locateSheetNumber(std::string const& str) const
{
    size_t i = 0;
    for( ; i < spritesheets_.size(); ++i ) {
        if( spritesheets_[i].M("data").exist( str ) )
            return i;
    }
    return i;
}

void Sprite::setupTextureFromSheetAndSize(size_t const& sheet_index, int const& w, int const& h)
{
    sheet_index_ = sheet_index;
    utils::map_any const& sheet = spritesheets_[sheet_index];

    std::ostringstream oss;
    oss << Conf::i().expand("rc/texture/") << sheet.S("image") << ".png";

    video::IVideoDriver* driver = smgr_->getVideoDriver();
    ITexture* tex = driver->getTexture(oss.str().c_str());

    utils::map_any const& tex_data = sheet.M("data").M(name_);

    if( w < 0 || h < 0 ) {
        size_ = dimension2df(tex_data.I("w"), tex_data.I("h"));
    } else {
        size_.Width  = w;
        size_.Height = h;
    }

    //adjustSheetUV(sheet);

    SMaterial mat = create_std_material_for_sprite();
    mat.setTexture(0, tex);
    body_->getMaterial(0) = mat;

    adjustSheetUV(sheet);
}

void Sprite::adjustSheetUV(utils::map_any const& sheet)
{
    utils::map_any const& tex_data = sheet.M("data").M(name_);
    double sheetwidth  = sheet.M("size").I("w");
    double sheetheight = sheet.M("size").I("h");
    if( thismesh_ ) {
        double orig_u = tex_data.I("x") / sheetwidth;
        double orig_v = tex_data.I("y") / sheetheight;
        double width_u  = tex_data.I("w") / sheetwidth;
        double height_v = tex_data.I("h") / sheetheight;

        body_->getMaterial(0).getTextureMatrix(0).setTextureTranslate(orig_u, orig_v);
        body_->getMaterial(0).getTextureMatrix(0).setTextureScale(width_u, height_v);
    }

}

void Sprite::setupTextureSingleAndSize(int const& w, int const& h)
{
    std::ostringstream oss;
    oss << Conf::i().expand("rc/texture/") << name_ << ".png";

    video::IVideoDriver* driver = smgr_->getVideoDriver();
    ITexture* tex = driver->getTexture(oss.str().c_str());

    if( w < 0 || h < 0 ) { //2012: added default case for ease.
        size_ = tex->getOriginalSize();
    } else {
        size_.Width  = w;
        size_.Height = h;
    }

    SMaterial mat = create_std_material_for_sprite();
    mat.setTexture(0, tex);
    body_->getMaterial(0) = mat;
}

SMaterial Sprite::create_std_material_for_sprite()
{
    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setFlag(video::EMF_NORMALIZE_NORMALS, true);

    //This is important for material usage in cubeat.
    //ColorMaterial is a added feature in Irrlicht 1.7 so that Vertex Color can be used as
    //Material colors when doing blending. But if you want the ordinary material color
    //to be used and not vertex color, this must be set to ECM_NONE.
    //I think it is a bad naming because you cannot understand the meaning by just looking
    //at the "ColorMaterial" name...
    mat.ColorMaterial = video::ECM_NONE;

    mat.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
    mat.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;

    mat.MaterialType = video::EMT_TRANSPARENT_MODULATE;
    mat.MaterialTypeParam = 0.01f;
    mat.DiffuseColor.set(255, 255, 255, 255);

    return mat;
}

void Sprite::setupMeshAndNode(IMesh*& out_mesh, ISceneNode*& out_node,
     ISceneNode* const& parent_body, dimension2df const& size, bool const& center,
     std::string const& debug_name)
{
    IMeshManipulator* mani = smgr_->getMeshManipulator();
    out_mesh = mani->createMeshCopy( sprite_plane_ptr_ );
    mani->scale( out_mesh, vector3df(size.Width, size.Height, 1) );

    if( center ) {
        matrix4 mat;
        mat.setTranslation( vector3df(-size.Width/2, size.Height/2, 0) );
        mani->transform( out_mesh, mat );
    }

    out_node = smgr_->addMeshSceneNode( out_mesh, parent_body, -1, vector3df(0,0,0) );
    out_node->setAutomaticCulling(EAC_OFF);
    out_node->setName( debug_name.c_str() );

    out_node->grab(); //added so its d'tor order is consistent with view::Object.
    //out_node->setDebugDataVisible(EDS_BBOX); //de-comment this when debugging.
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

Sprite& Sprite::setTexture(std::string const& path)
{
    video::IVideoDriver* driver = smgr_->getVideoDriver();
    size_t i = locateSheetNumber(path);

    if( i >= spritesheets_.size() ) {
        Object::setTexture(path);
        return *this;
    } else {
        name_ = path; // name is actually important here. because we depend on name_ to lookup into spritesheet_
        body_->setName(path.c_str());

        if( i != sheet_index_ ) {
            std::string full_path("rc/texture/");
            full_path += spritesheets_[i].S("image") + ".png";
            body_->setMaterialTexture(0, driver->getTexture(full_path.c_str()) );
            sheet_index_ = i;
        }
        adjustSheetUV(spritesheets_[i]);
    }
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
    mani->transform( thismesh_, mat );
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

Sprite& Sprite::lazyFixAlphaArtifact()
{
    set<ScaleWithUV>(vec2(0.995, 0.995));
    return *this;
}

Sprite& Sprite::removeTexture()
{
    IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    driver->removeTexture( body_->getMaterial(0).getTexture(0) );
    return *this;
}

Sprite& Sprite::setBlending(std::string const& mt_name)
{
    int mat_type = IrrDevice::i().getMaterialTypeEx(mt_name);
    if( mat_type > EMT_TRANSPARENT_MODULATE ) { // at least make sure it's not the basic material type
        body_->getMaterial(0).MaterialType = static_cast<E_MATERIAL_TYPE>(mat_type);
        body_->setMaterialTexture(1, scene()->getPostProcTexture());
    }
    else {
        body_->getMaterial(0).MaterialType = EMT_TRANSPARENT_MODULATE;
        body_->setMaterialTexture(1, 0);
    }
    return *this;
}

Sprite& Sprite::removeParticleChildren()
{
    core::list<ISceneNode*> children = body_->getChildren();
    for( core::list<ISceneNode*>::ConstIterator it = children.begin(), iend = children.end();
         it != iend; ++it ) {
        if( (*it)->getType() == ESNT_PARTICLE_SYSTEM ) {
            body_->removeChild(*it);
        }
    }
    return *this;
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
