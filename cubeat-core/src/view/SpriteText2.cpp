
#include "view/SpriteText2.hpp"
#include "view/Scene.hpp"
#include "view/SFX.hpp"
#include "utils/to_s.hpp"
#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"
#include "Conf.hpp"

#include <boost/lexical_cast.hpp>
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

pSpriteText2
SpriteText2::init(std::string const& text, pObject const& parent)
{
    if( text.size() < 1 ) {
        Object::init(parent);
        return std::tr1::static_pointer_cast<SpriteText2>( shared_from_this() );
    }
    setupSceneAndManager(parent);

    setupMeshAndNode(thismesh_, body_, parent->body(), dimension2df(100, 100), center_, name_);
    SMaterial mat = create_std_material_for_sprite();
    mat.DiffuseColor = SColor(0, 255, 255, 255);
    body_->getMaterial(0) = mat;

    createText( text );

    pSpriteText2 self = static_pointer_cast<SpriteText2>( shared_from_this() );
    scene()->addPickMapping( body_, self );
    return self;
}

void SpriteText2::createText(std::string const& text)
{
    changeText( text );
}

void SpriteText2::clearText()
{
    for( unsigned int i = 0; i < letter_node_.size(); ++i ) {
        letter_node_outline_[i]->remove(); // outline follows the main node, must be removed first
        letter_node_[i]->remove(); //we call remove and it will be dropped.
    }
    letter_node_outline_.clear();
    letter_node_.clear();
}

SpriteText2& SpriteText2::changeText(std::string const& new_text)
{
    if( text_ == new_text )
        return *this;

    // This specialized class only used premade sprites for texts now, so hardcodingly blocking all non-suitable applications:
    for( size_t i = 0; i < new_text.size(); ++i ) {
        if ( new_text[i] != '.' ) {
            if( new_text[i] > '9' || new_text[i] < '0' ) {
                printf("SpriteText2: bad string for current application: %s\n", new_text.c_str());
                return *this;
            }
        }
    }

    video::IVideoDriver* driver = smgr_->getVideoDriver();
    std::string full_path("rc/texture/sptxt/");

    if( new_text.size() != text_.size() ) {
        clearText();
        text_ = new_text;
        // 2012 new scheme, now we just use the ustring from irr::core,
        // however, a char const* will make it think it's arbitrary byte stream and it will try to
        // identify it using BOM, which is undesirable (we don't use BOM in anyway)
        // so, the reinterpret_cast from char const* to unsigned char const* is necessary.

        size_.Width = 128 * text_.size(); // Temporary and treat them as monospaced words now.
        size_.Height= 128;

        for( size_t i = 0; i < text_.size(); ++i) {
            IMesh* tmp_mesh;
            ISceneNode* temp_letter_node;
            ISceneNode* temp_letter_node_outline;

            setupMeshAndNode(tmp_mesh, temp_letter_node, body_, dimension2df(128, 128), false, "");
            tmp_mesh->drop();
            setupMeshAndNode(tmp_mesh, temp_letter_node_outline, temp_letter_node, dimension2df(128, 128), false, "");
            tmp_mesh->drop();

            int startx = 0, starty = 0;
            if( center_ ) {
                startx -= static_cast<int>(text_.size())*128 / 2;
                starty += 128 / 2; // y-inversed!
            }

            vector3df node_pos(startx + static_cast<int>(i) * 128, starty, 0);
            vector3df outline_pos(0, 0, -5);

            temp_letter_node->setPosition(node_pos);
            temp_letter_node_outline->setPosition(outline_pos);

            SMaterial mat = create_std_material_for_sprite();
            temp_letter_node->getMaterial(0) = mat;
            temp_letter_node_outline->getMaterial(0) = mat;

            std::string texture_name;
            if( text_[i] == '.' ) {
                texture_name = full_path + "dot";
            } else {
                texture_name = full_path + text_[i];
            }

            temp_letter_node->setMaterialTexture(0, driver->getTexture( (texture_name + ".png" ).c_str() ));
            temp_letter_node_outline->setMaterialTexture(0, driver->getTexture( (texture_name + "o.png" ).c_str() ));

            temp_letter_node->setIsDebugObject(true);
            temp_letter_node_outline->setIsDebugObject(true);

            letter_node_.push_back( temp_letter_node );
            letter_node_outline_.push_back( temp_letter_node_outline );
            //when debug text nodes un-comment this:
            //letter_node_[i]->setDebugDataVisible(EDS_BBOX);
            printf("SpriteText2: changeText totally new string\n");
        }
    } else { // when the length of the SpriteText2 didn't change, which is most what happens with this class' use case
        text_ = new_text;

        for( size_t i = 0; i < text_.size(); ++i ) {

            std::string texture_name;
            if( text_[i] == '.' ) {
                texture_name = full_path + "dot";
            } else {
                texture_name = full_path + text_[i];
            }

            letter_node_[i]->setMaterialTexture(0, driver->getTexture( (texture_name + ".png").c_str() ) );
            letter_node_outline_[i]->setMaterialTexture(0, driver->getTexture( (texture_name + "o.png").c_str() ) );
        }
    }

    if( body_ )
        Size2D::set(body_, vec2(size_.Width, size_.Height));

    return *this;
}

SpriteText2& SpriteText2::showDouble(double num, unsigned int precision)
{
    // convert the number to string and truncate the unwanted precision
    std::string str( boost::lexical_cast<std::string>(num) );
    std::string res;

    size_t i = 0;
    for( ; i < str.size() && str[i] != '.' ; ++i );

    if( i < str.size() ) {
        size_t j = i + 1;
        for( ; j - i < precision && j < str.size()-1 ; ++j );
        res = str.substr(0, j+1);

        for( ; j - i < precision ; ++j ) {
            res += "0";
        }
    } else {
        res += str.substr(0, i) + ".";
        for( int j = 0; j < precision; ++j ) {
            res += "0";
        }
    }

    changeText( res );
    return *this;
}

SpriteText2& SpriteText2::setCenterAligned(bool const& center)
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


// Note: outline animation must be separate from this API, otherwise too confusing
void SpriteText2::startTween()
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

void SpriteText2::startTween_outline()
{
    if( anim_queue_.size() > 0 ) {
        AnimatorBase* anim = anim_queue_.front();
        anim_queue_.pop_front();
        std::time_t time = scene()->getTime();
        anim->updateStartTime( time );
        AT::ATEnum e = static_cast<AT::ATEnum>(anim->getType());
        clearTween_outline( e ); //del all animators of same type

        for( unsigned int i = 0; i < letter_node_outline_.size(); ++i ) {
            if( i == 0 )
                letter_node_outline_[0]->addAnimator( anim );
            else {
                AnimatorBase* a = anim->cloneWithoutCallback();
                a->updateStartTime( time );
                letter_node_outline_[i]->addAnimator( a );
                a->drop();
            }
            //the problem here is, we have to make sure only "one of the letters"
            //have the animator with callback. since the callback semantic is
            //"do something once when this sprite's animation ends", NOT
            //"do something as many times as the number of letters when animation ends."
        }
        anim->drop();
    }
}

SpriteText2& SpriteText2::glow_animation()
{
    std::string full_path("sptxt/");

    for( size_t i = 0; i < text_.size(); ++i) {

        std::string texture_name;
        if( text_[i] == '.' ) {
            texture_name = full_path + "doto";
        } else {
            texture_name = full_path + text_[i] + "o";
        }

        int startx = static_cast<int>(i) * 128 + 64 , starty = 0;
        if( center_ ) {
            startx -= static_cast<int>(text_.size())*128 / 2;
        }

        pSprite temp = Sprite::create(texture_name, shared_from_this(), 128, 128, true);
        temp->set<Pos2D>( vec2(startx, starty) ).setDepth(-5).setPickable(false);
        temp->set<ColorDiffuseVec3>( vec3(255, 255, 0) );
        temp->tween<OQuad, Scale>( vec3(1,1,1), vec3(1.5, 1.5, 1), 200u );
        temp->tween<Linear, Alpha>( 255, 0, 200u );

        SFX::i().hold( temp, 250u );
    }

    return *this;
}

SpriteText2& SpriteText2::clearAllTween()
{
    clearAllQueuedTween();
    body_->removeAnimators();

    BOOST_FOREACH(ISceneNode* it, letter_node_)
        it->removeAnimators();

    BOOST_FOREACH(ISceneNode* it, letter_node_outline_)
        it->removeAnimators();

    return *this;
}


// Note: outline animation must be separate from this API, otherwise too confusing
SpriteText2& SpriteText2::clearTween(AT::ATEnum const& eType)
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

SpriteText2& SpriteText2::clearTween_outline(AT::ATEnum const& eType)
{
    BOOST_FOREACH(ISceneNode* it, letter_node_outline_) {
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

SpriteText2::~SpriteText2()
{
    clearText();
}
