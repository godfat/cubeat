
#include "view/SpriteText.hpp"
#include "view/Scene.hpp"
#include "utils/to_s.hpp"
#include "Accessors.hpp"
#include "IrrDevice.hpp"

#include <sstream>
#include <algorithm> //for the ugly copy

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
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

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setFlag(video::EMF_NORMALIZE_NORMALS, true);

    mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    SColor col( color.rgb() );
    col.setAlpha( 0/*255*/ );
    mat.DiffuseColor = col;

    body_->getMaterial(0) = mat;

    createText( text, font_path, size );

    //adjust_texcoord_for_hand_made_texture(thismesh_, size_.Width, size_.Height);

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
    dimension2di letter_size = ttfont_->getDimension(&wc);

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setFlag(video::EMF_NORMALIZE_NORMALS, true);
    mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;
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

    std::cout << "Trace meshcache count: " << IrrDevice::i().d()->getVideoDriver()->getTextureCount() << "\n";
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

    dimension2di size_int = ttfont_->getDimension(wtext.c_str());
    size_.Width = size_int.Width;
    size_.Height= size_int.Height;

    int current_xpos = 0; //This will be modified by generateLetter()
    for( unsigned int i = 0; i < new_text.size(); ++i ) {
        char c = new_text[i];
        char lc= (i > 0) ? new_text[i-1] : '\0';
        generateLetter(c, lc, current_xpos);
    }

//    font_texture_ =
//        ttfont_->getTextureFromText(wtext.c_str(), (new_text+"_"+fpath_+"_"+to_s(fsize_)).c_str());

    if( body_ ) {
//        body_->setMaterialTexture(0, font_texture_);
        set<Size2D>( vec2(size_.Width, size_.Height) );
//        adjust_texcoord_for_hand_made_texture(thismesh_, size_.Width, size_.Height);
    }
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
    body_->setMaterialTexture(0, font_texture_);
    body_->getMaterial(0).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    body_->getMaterial(0).MaterialTypeParam = 0.01f;
    return *this;
}

SpriteText::~SpriteText()
{
    clearText();
}
