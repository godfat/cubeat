#ifndef _SHOOTING_CUBES_VIEW_SPRITEMOVIE_
#define _SHOOTING_CUBES_VIEW_SPRITEMOVIE_

#include "view/Sprite.hpp"
#include "all_fwd.hpp"
#include <string>

class AVIVideo;

namespace psc { namespace view {

class SpriteMovie : public Sprite
{
public:
    typedef std::tr1::shared_ptr< SpriteMovie > pointer_type;
    static pointer_type create(std::string const& path,
                               pObject const&     parent,
                               int const& w = 100,
                               int const& h = 100,
                               bool const& center = false)
    {
        return utils::ObjectPool< SpriteMovie >::create(path, center)->init(parent, w, h);
    }

    SpriteMovie(std::string const& name, bool const& center)
        : Sprite(name, center) { avi = 0; }

    virtual SpriteMovie* clone() const { return 0; }
    virtual SpriteMovie& redraw();
    virtual SpriteMovie& play();
    virtual SpriteMovie& stop();
    virtual SpriteMovie& restart();
    virtual SpriteMovie& setFPS(int const&);
    virtual SpriteMovie& setCurrentFrame(int const&);
    virtual int getCurrentFrame() const;
    virtual bool isEnd() const;

    virtual ~SpriteMovie();

protected:
    pointer_type init(pObject const& parent, int const& ,int const&);

protected:
    AVIVideo* avi;

};

} //view
} //psc


#endif // _SHOOTING_CUBES_VIEW_SPRITEMOVIE_
