
#include "view/SpriteMovie.hpp"
#include "view/Scene.hpp"
#include "private/AVIVideo.hpp"
#include "IrrDevice.hpp"
#include "Conf.hpp"

#include <sstream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using std::tr1::static_pointer_cast;

pSpriteMovie SpriteMovie::init(pObject const& parent, int const& w, int const& h)
{
    setupSceneAndManager(parent);

    size_.Width = w;
    size_.Height = h;

    avi = new AVIVideo(Conf::i().expand(name_.c_str()));

    /////////////// I'll keep this part for future reference /////////////////
    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setTexture(0, avi->getTexture());
    mat.MaterialType = video::EMT_ONETEXTURE_BLEND;

    mat.MaterialTypeParam =
        video::pack_texureBlendFunc(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA, EMFN_MODULATE_1X);

    mat.DiffuseColor.set(255,255,255,255);
    /////////////// End of this part /////////////////////////////////////////

    setupMeshAndNode(thismesh_, body_, parent->body(), size_, center_, name_);
    body_->getMaterial(0) = mat;

    dimension2di avi_actual_size = avi->getSize();
    adjust_texcoord_for_hand_made_texture( thismesh_, avi_actual_size.Width, avi_actual_size.Height );

    pSpriteMovie self = static_pointer_cast<SpriteMovie>( shared_from_this() );
    scene()->addPickMapping( body_, self );
    return self;
}

SpriteMovie& SpriteMovie::redraw()  { avi->redraw(); return *this; }
SpriteMovie& SpriteMovie::play()    { avi->play(); return *this; }
SpriteMovie& SpriteMovie::stop()    { avi->stop(); return *this; }
SpriteMovie& SpriteMovie::restart() { avi->restart(); return *this; }
SpriteMovie& SpriteMovie::setFPS(int const& fps)        { avi->setFPS(fps); return *this; }
SpriteMovie& SpriteMovie::setCurrentFrame(int const& n) { avi->setCurrentFrame(n); return *this; }
int SpriteMovie::getCurrentFrame() const { return avi->getCurrentFrame(); }
bool SpriteMovie::isEnd() const          { return avi->isEnd(); }

SpriteMovie::~SpriteMovie()
{
    if( avi )
        delete avi;
}
