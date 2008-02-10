
#include "view/SpriteMovie.hpp"
#include "private/AVIVideo.hpp"
#include "IrrDevice.hpp"

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
    size_.Width = w;
    size_.Height = h;

    avi = new AVIVideo(name_.c_str());

    SMaterial mat_;

    mat_.setFlag(video::EMF_LIGHTING, true);
    mat_.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat_.setTexture(0, avi->getTexture());
    mat_.MaterialType = video::EMT_ONETEXTURE_BLEND;

    mat_.MaterialTypeParam =
        video::pack_texureBlendFunc(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA, EMFN_MODULATE_1X);

    mat_.DiffuseColor.set(255,255,255,255);

    setupMeshBase(parent);
    body_->getMaterial(0) = mat_;

    dimension2di avi_actual_size = avi->getSize();
    adjust_texcoord_for_hand_made_texture( avi_actual_size.Width, avi_actual_size.Height );

    press_.setOwner( static_pointer_cast<SpriteMovie>(shared_from_this()) );
    scene_ = parent->scene();

    return static_pointer_cast<SpriteMovie>(shared_from_this());
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
