
#include "view/SpriteText.hpp"
#include "view/Scene.hpp"
#include "utils/to_s.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"
#include "Conf.hpp"

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

    oss << Conf::i().expand("rc/fonts/") << font_path << ".xtf";
    ttfont_ = gui->getFont(oss.str().c_str(), size);

    if( !ttfont_ ) { //if not otf, try ttf
        oss.str(std::string());
        oss << Conf::i().expand("rc/fonts/") << font_path << ".otf";
        ttfont_ = gui->getFont(oss.str().c_str(), size);
    }

    if( !ttfont_ ) { //if not otf, try xtf
        oss.str(std::string());
        oss << Conf::i().expand("rc/fonts/") << font_path << ".ttf";
        ttfont_ = gui->getFont(oss.str().c_str(), size);
    }

    ttfont_->setBatchLoadSize(1);
    ttfont_->setMaxPageTextureSize( dimension2du(512,512) );
    //I don't want massive caches, it actually can be as small as possible.

    changeText( text );
}

void SpriteText::clearText()
{
    for( unsigned int i = 0; i < letter_node_.size(); ++i ) {
        letter_node_[i]->remove(); //we call remove and it will be dropped.
    }
    letter_node_.clear();
}

SpriteText& SpriteText::changeText(std::string const& new_text)
{
    if( text_ == new_text )
        return *this;

    clearText();
    text_ = new_text;
    core::ustring utf8s(reinterpret_cast<const uchar8_t*>(new_text.c_str()));
    // 2012 new scheme, now we just use the ustring from irr::core,
    // however, a char const* will make it think it's arbitrary byte stream and it will try to
    // identify it using BOM, which is undesirable (we don't use BOM in anyway)
    // so, the reinterpret_cast from char const* to unsigned char const* is necessary.

    dimension2d<u32> size_int = ttfont_->getDimension(utf8s);

    size_.Width = size_int.Width;
    size_.Height= size_int.Height;

    SColor textcolor = body_->getMaterial(0).DiffuseColor;
    textcolor.setAlpha(255); //do not forget! SpriteText's alpha is ZERO! (because that's just a big white plane in 3D)
    array<ISceneNode*> irrArray =
        ttfont_->addTextSceneNode(utf8s, smgr_, body_, textcolor, center_);

    for( unsigned int i = 0; i < irrArray.size(); ++i) {
        letter_node_.push_back( irrArray[i] );  //irr::core::array and std::vector inconsistency... only way.. *sigh*
        //when debug text nodes un-comment this:
//        letter_node_[i]->setDebugDataVisible(EDS_BBOX);
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
        std::time_t time = scene()->getTime();
        anim->updateStartTime( time );
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
                    a->updateStartTime( time );
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
