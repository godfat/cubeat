
#ifndef _SHOOTING_CUBES_MENU_VIEW_
#define _SHOOTING_CUBES_MENU_VIEW_

#include "view/Object.hpp"

namespace view {

class Menu : public Object
{
public:
    Menu(){}
    virtual void init(Object const*);

    virtual Menu* clone() const;

    virtual ~Menu(){}

protected:

};

} //view

#endif
