
#include "view/AnimatedSprite.hpp"
#include "view/Scene.hpp"
#include "view/detail/TextureAnimator.hpp"
#include "EasingEquations.hpp"
#include "IrrDevice.hpp"

#include <sstream>

using namespace irr;
using namespace io;    //for file system and file list
using namespace core;
using namespace scene;
using namespace video;

using namespace psc;
using namespace view;
using namespace easing;
using std::tr1::static_pointer_cast;

AnimatedSprite* AnimatedSprite::clone() const
{
    AnimatedSprite* obj = new AnimatedSprite(*this);
    return obj;
}

pAnimatedSprite AnimatedSprite::init(pObject const& parent, int const& w, int const& h)
{
    setupSceneAndManager(parent);

    size_.Width  = w;
    size_.Height = h;

    SMaterial mat;
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    mat.MaterialTypeParam = 0.01f;

    mat.DiffuseColor.set(255,255,255,255);

    setupMeshBase(parent);
    body_->getMaterial(0) = mat;

    pAnimatedSprite self = static_pointer_cast<AnimatedSprite>( shared_from_this() );
    scene()->addPickMapping( body_, self );

    loadAllAnime();

    return self;
}

AnimatedSprite& AnimatedSprite::addAnime(std::string const& anime_name, int total_frames)
{
    if( animations_.find(anime_name) != animations_.end() ) return *this; //don't add repetitively.

    video::IVideoDriver* driver = smgr_->getVideoDriver();
    driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY, true);

    Animation2D animation;
    for(int i=0; i < total_frames; ++i ) {
        std::ostringstream oss;
        oss << "rc/texture/" << name_ << "/" << anime_name << "/" << i << ".png";
        ITexture* tex = driver->getTexture( oss.str().c_str() );
        animation.push_back( tex );
    }
    animations_.insert( std::make_pair( anime_name, animation ) );
    return *this;
}

void AnimatedSprite::loadAllAnime()
{
    IFileSystem* fs = IrrDevice::i().d()->getFileSystem();
    std::string work_dir( fs->getWorkingDirectory() );
    std::string to_path( (work_dir + "/rc/texture/") + name_ );
    fs->changeWorkingDirectoryTo( to_path.c_str() );
    IFileList* flist = fs->createFileList();
    for( unsigned int i = 0; i < flist->getFileCount(); ++i ) {
        if( flist->isDirectory(i) ) {
            if( std::string( flist->getFileName(i) ) == std::string(".") ||
                std::string( flist->getFileName(i) ) == std::string("..") ) continue;

            std::string anim_path( flist->getFileName(i) );
            fs->changeWorkingDirectoryTo( ((to_path + "/")+anim_path).c_str() );
            IFileList* animlist = fs->createFileList();
            int count_frame = animlist->getFileCount();
            int num_of_dirs_to_skip = 0;
            for( unsigned int j = 0; j < animlist->getFileCount(); ++j )
                if( animlist->isDirectory(j) ) ++num_of_dirs_to_skip;

            fs->changeWorkingDirectoryTo( work_dir.c_str() );
            //must call this in root dir, for old client code compatibility.
            addAnime( anim_path, count_frame - num_of_dirs_to_skip );

            fs->changeWorkingDirectoryTo( to_path.c_str() );
        }
    }
    fs->changeWorkingDirectoryTo( work_dir.c_str() );
}

AnimatedSprite&
AnimatedSprite::playAnime(std::string const& anime_name, unsigned int const& duration, int const& loop,
                          std::tr1::function<void()> cb, int const& delayTime)
{
    if( !duration ) return *this;
    AnimatorBase* animator =
        new TextureAnimator<Linear>( smgr_, animations_[anime_name], duration, loop, cb, delayTime );
    anim_queue_.push_back( animator );
    startTween();
    return *this;
}

bool AnimatedSprite::isPlaying() const
{
    typedef core::list< ISceneNodeAnimator* > IrrAnimList;
    IrrAnimList const& alist = body_->getAnimators();
    for( IrrAnimList::ConstIterator it = alist.begin(), end = alist.end();
         it != end; ++it ) {
        if( static_cast<int>((*it)->getType()) == static_cast<int>(accessor::AT::TEXTURE) ) //work?
            return true;
    }
    return false;
}

AnimatedSprite::~AnimatedSprite()
{
}
