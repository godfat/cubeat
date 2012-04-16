#ifndef _SHOOTING_CUBES_TEST_TEST_BASIC_ENET_MAIN_
#define _SHOOTING_CUBES_TEST_TEST_BASIC_ENET_MAIN_

#include <boost/tr1/memory.hpp>

namespace psc {

namespace presenter {
class Stage;
typedef std::tr1::shared_ptr<Stage> pStage;
}

namespace test {

class TestBasicENet{
public:
    TestBasicENet();
    void cycle();

private:
    presenter::pStage stage_;
};

} //test
} //psc

#endif //_SHOOTING_CUBES_TEST_TEST_BASIC_ENET_MAIN_

