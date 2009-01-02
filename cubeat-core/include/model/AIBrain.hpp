#ifndef _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_
#define _SHOOTING_CUBES_MODEL_ARTIFICIAL_INTELLIGENCE_BRAIN_

#include <tr1/memory>

namespace psc {

namespace ctrl {
class AIPlayer;
typedef std::tr1::shared_ptr<AIPlayer> pAIPlayer;
typedef std::tr1::weak_ptr<AIPlayer> wpAIPlayer;
}

namespace model {

class AIBrain
{
public:
    AIBrain(ctrl::pAIPlayer const&);
    void think();

    bool isThinking() const { return is_thinking_; }
    int  getData()    const { return data_; }

private:
    ctrl::wpAIPlayer owner_;
    int  data_;
    bool is_thinking_;
};

} //model
} //psc

#endif //_SHOOTING_CUBES_CTRL_ARTIFICIAL_INTELLIGENCE_BRAIN_
