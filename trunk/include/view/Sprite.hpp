#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace psc { namespace view {

class SceneView;

class Sprite : public Object, public std::tr1::enable_shared_from_this<Sprite>
{
public:
    typedef std::tr1::shared_ptr< Sprite > pointer_type;
    static pointer_type create(pObject const& parent) {
        pointer_type p = psc::ObjectPool< Sprite >::create();
        p->init(parent);
        return p;
    }

    virtual Sprite* clone() const;
    virtual Sprite& moveTo(int,int);
    virtual Sprite& setDepth(float);
    virtual Sprite& moveTween(int,int,int,std::tr1::function<void()> cb = 0, int delay = 0);

    virtual ~Sprite();

protected:
    void init(pObject const&);

};

typedef Sprite::pointer_type pSprite;
typedef std::tr1::weak_ptr<Sprite> wpSprite;

} //view
} //psc

#endif
