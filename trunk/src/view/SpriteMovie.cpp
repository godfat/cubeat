
#include "view/SpriteMovie.hpp"

#ifdef WIN32
#include "private/AVIVideo.hpp"
#endif

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

    SMaterial mat_;

    mat_.setFlag(video::EMF_LIGHTING, true);
    mat_.MaterialType = video::EMT_ONETEXTURE_BLEND;

    mat_.MaterialTypeParam =
        video::pack_texureBlendFunc(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA, EMFN_MODULATE_1X);

    mat_.DiffuseColor.set(255,255,255,255);

    setupMeshBase(parent);
    body_->getMaterial(0) = mat_;

    press_.setOwner( static_pointer_cast<SpriteMovie>(shared_from_this()) );
    scene_ = parent->scene();

    return static_pointer_cast<SpriteMovie>(shared_from_this());
}

SpriteMovie::~SpriteMovie()
{
    if ( avi )
        delete avi;
}
