
#include "view/SpriteText.hpp"
#include "view/Scene.hpp"
#include "utils/to_s.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"

#include <sstream>
#include <algorithm> //for the ugly copy

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using namespace easing;
using namespace accessor;
using utils::to_s;
using std::tr1::static_pointer_cast;

pSpriteText
SpriteText::init(std::string const& text, std::string const& font_path,
                 int const& size, data::Color const& color, pObject const& parent)
{
    if( text.size() < 1 ) {
        Object::init(parent);
        return std::tr1::static_pointer_cast<SpriteText>( shared_from_this() );
    }
    setupSceneAndManager(parent);
    fsize_ = size;
    fpath_ = font_path;

    setupMeshAndNode(thismesh_, body_, parent, dimension2df(100, 100), center_, name_);

    SMaterial mat = create_std_material_for_sprite();
    mat.DiffuseColor = SColor( color.rgb() );
    body_->getMaterial(0) = mat;

    createText( text, font_path, size );

    pSpriteText self = static_pointer_cast<SpriteText>( shared_from_this() );
    scene()->addPickMapping( body_, self );
    return self;
}

void SpriteText::createText(std::string const& text, std::string const& font_path, int const& size)
{
    gui::IGUIEnvironment* gui = IrrDevice::i().d()->getGUIEnvironment();
    //This is rather stupid. we have multiple SceneManager, but only one GUIEnvironment,
    //so smgr_->getGUIEnvironment() is NULL !? ...well, guess I have to live with that.

    std::ostringstream oss;
    oss << "rc/fonts/" << font_path << ".ttf";

    ttfont_ = gui->getFont(oss.str().c_str(),size);

    ttfont_->setAntiAlias(true);
    ttfont_->setTransparent(true);

    changeText( text );
}

void SpriteText::generateLetter(char const& c, char const& last_c, int& current_xpos)
                                //current_xpos is modifable
{
    wchar_t wc      = std::cin.widen(c); //not toliet ...
    wchar_t last_wc = std::cin.widen(last_c);
    std::string texture_name = fpath_+"_"+to_s(fsize_)+"_";
    texture_name += c;

    ITexture*    current_tex = ttfont_->getTextureFromText(&wc, texture_name.c_str());
    IMesh*       current_mesh;
    ISceneNode*  current_node;
    dimension2d<u32> letter_size = ttfont_->getDimension(&wc);

    SMaterial mat = create_std_material_for_sprite();
    mat.DiffuseColor = body_->getMaterial(0).DiffuseColor;
    mat.DiffuseColor.setAlpha(255);
    mat.setTexture(0, current_tex);

    vec2 orig = center_ ? vec2(-size_.Width/2, size_.Height/2) : vec2(0,0);
    orig.X += current_xpos;

    setupMeshAndNode(current_mesh, current_node, shared_from_this(),
                     dimension2df(letter_size.Width, letter_size.Height) );
    current_node->setPosition( vec3(orig.X, orig.Y, 0) );
    Size2D::set( current_node, vec2(letter_size.Width, letter_size.Height) );
    current_node->getMaterial(0) = mat;
    current_node->setIsDebugObject(true);

    adjust_texcoord_for_hand_made_texture(current_mesh, letter_size.Width, letter_size.Height);

    letter_mesh_.push_back( current_mesh );
    letter_node_.push_back( current_node );

    current_xpos += letter_size.Width + ttfont_->getKerningWidth(&wc, &last_wc);
}

void SpriteText::clearText()
{
    for( unsigned int i = 0; i < letter_node_.size(); ++i ) {
        letter_node_[i]->remove(); //we call remove and it will be dropped.
        letter_mesh_[i]->drop();
    }
    letter_node_.clear();
    letter_mesh_.clear();
}

SpriteText& SpriteText::changeText(std::string const& new_text)
{
    if( text_ == new_text )
        return *this;

    clearText();
    text_ = new_text;
    std::wstring wtext(new_text.length(),L' ');
    std::copy(new_text.begin(), new_text.end(), wtext.begin());
    //so hard to convert between wchar and char ....

    dimension2d<u32> size_int = ttfont_->getDimension(wtext.c_str());
    size_.Width = size_int.Width;
    size_.Height= size_int.Height;

    int current_xpos = 0; //This will be modified by generateLetter()
    for( unsigned int i = 0; i < new_text.size(); ++i ) {
        char c = new_text[i];
        char lc= (i > 0) ? new_text[i-1] : '\0';
        generateLetter(c, lc, current_xpos);
    }

    if( body_ )
        Size2D::set(body_, vec2(size_.Width, size_.Height));

    return *this;
}

SpriteText& SpriteText::showNumber(int num, unsigned int digit)
{
    std::string str = to_s(num);
    while( str.size() < digit ) {
        str = "0" + str;
    }
    changeText( str );
    return *this;
}

SpriteText& SpriteText::setCenterAligned(bool const& center)
{
    if( center_ == center ) return *this;
    Sprite::setCenterAligned(center);

    if( center_ ) {
        BOOST_FOREACH(ISceneNode* it, letter_node_) {
            vec3 orig;
            Pos3D::get(it, orig);
            orig.X -= size_.Width/2;
            orig.Y += size_.Height/2;
            Pos3D::set(it, orig);
        }
    }
    else {
        BOOST_FOREACH(ISceneNode* it, letter_node_) {
            vec3 orig;
            Pos3D::get(it, orig);
            orig.X += size_.Width/2;
            orig.Y -= size_.Height/2;
            Pos3D::set(it, orig);
        }
    }
    return *this;
}

void SpriteText::startTween()
{
    if( anim_queue_.size() > 0 ) {
        AnimatorBase* anim = anim_queue_.front();
        anim_queue_.pop_front();
        anim->updateStartTime();
        AT::ATEnum e = static_cast<AT::ATEnum>(anim->getType());
        clearTween( e ); //del all animators of same type

        if( AT::isMatrixTransformationValue( e ) )
            body_->addAnimator( anim );
        else {
            for( unsigned int i=0; i < letter_node_.size(); ++i ) {
                if( i == 0 )
                    letter_node_[0]->addAnimator( anim );
                else {
                    AnimatorBase* a = anim->cloneWithoutCallback();
                    letter_node_[i]->addAnimator( a );
                    a->drop();
                }
                //the problem here is, we have to make sure only "one of the letters"
                //have the animator with callback. since the callback semantic is
                //"do something once when this sprite's animation ends", NOT
                //"do something as many times as the number of letters when animation ends."
            }
        }
        anim->drop();
    }
}

SpriteText& SpriteText::clearAllTween()
{
    clearAllQueuedTween();
    body_->removeAnimators();

    BOOST_FOREACH(ISceneNode* it, letter_node_)
        it->removeAnimators();

    return *this;
}

SpriteText& SpriteText::clearTween(AT::ATEnum const& eType)
{
    if( AT::isMatrixTransformationValue(eType) )
        Object::clearTween(eType);
    else {
        BOOST_FOREACH(ISceneNode* it, letter_node_) {
            typedef core::list< ISceneNodeAnimator* > IrrAnimList;
            IrrAnimList const& alist = it->getAnimators();
            for( IrrAnimList::ConstIterator a = alist.begin(), end = alist.end();
                 a != end; ++a ) {
                if( static_cast<int>((*a)->getType()) == static_cast<int>(eType) ) {
                    smgr_->addToAnimatorDeletionQueue(*a, it);
                }
            }
        }
    }
    return *this;
}

SpriteText::~SpriteText()
{
    clearText();
}
