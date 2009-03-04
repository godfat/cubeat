#ifndef _SHOOTING_CUBES_MODEL_AI_COMMAND_
#define _SHOOTING_CUBES_MODEL_AI_COMMAND_

#include "utils/ObjectPool.hpp"
#include "ButtonEnum.hpp"
#include <tr1/memory>
#include <utility>

/* 2009.3 by arch.jslin
   This is a class for providing a unified & extensible interface
   between class AIBrain and class AIPlayer.
*/

namespace psc {
namespace model {

class AICommand
{
public:
    struct BtnID{enum BID{NONE, TRIG_1, TRIG_2, WEP_1, WEP_2, WEP_3, HASTE};};

    typedef std::tr1::shared_ptr<AICommand>             pointer_type;
    typedef std::tr1::weak_ptr<AICommand>               wpointer_type;
    typedef std::tr1::shared_ptr< std::pair<int, int> > pPosition;
    typedef std::tr1::shared_ptr< BtnID::BID >          pButton;

    static pointer_type create(int weight, int delay, int priority = 10) {
        return utils::ObjectPool<AICommand>::create(weight, delay, priority);
    }

    static pointer_type create() {
        return utils::ObjectPool<AICommand>::create();
    }

    AICommand():
        weight_(0), delay_(0), priority_(10), btn_press_(false)
    {}

    AICommand(int weight, int delay, int priority):
        weight_(weight), delay_(delay), priority_(priority), btn_press_(false)
    {}

    AICommand& move_to(int x = -1, int y = -1);
    AICommand& press  (BtnID::BID = BtnID::NONE);
    AICommand& release(BtnID::BID = BtnID::NONE);

    //shortcuts
    AICommand& press_haste();
    AICommand& press_trig1();
    AICommand& press_trig2();
    AICommand& press_wep1();
    AICommand& press_wep2();
    AICommand& press_wep3();
    //shortcuts
    AICommand& release_haste();
    AICommand& release_trig1();
    AICommand& release_trig2();
    AICommand& release_wep1();
    AICommand& release_wep2();
    AICommand& release_wep3();
    //shortcuts
    AICommand& normal_shot(int x = -1, int y = -1);
    AICommand& weapon_shot(int x = -1, int y = -1);

    AICommand& weight(int w)   { weight_ = w;   return *this; }
    AICommand& delay(int d)    { delay_  = d;   return *this; }
    AICommand& priority(int p) { priority_ = p; return *this; }

    pPosition pos()      const { return pos_; }
    pButton   btn()      const { return btn_; }
    int       weight()   const { return weight_; }
    int       delay()    const { return delay_; }
    int       priority() const { return priority_; }
    bool      press_btn()const { return btn_press_; }

private:

    pPosition pos_;
    pButton   btn_;
    int weight_;
    int delay_;
    int priority_;
    bool btn_press_;
};

typedef AICommand::pointer_type pAICommand;
typedef AICommand::wpointer_type wpAICommand;

} //model
} //psc

#endif //_SHOOTING_CUBES_MODEL_AI_COMMAND_
