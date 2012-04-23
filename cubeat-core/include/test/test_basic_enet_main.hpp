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
    TestBasicENet(int type);
    ~TestBasicENet();
    void cycle();

    void on_connected();
    void on_matched();
    void on_disconnected();
    void on_received();

private:
    presenter::pStage stage_;
    int type_;
};

} //test
} //psc

#endif //_SHOOTING_CUBES_TEST_TEST_BASIC_ENET_MAIN_

