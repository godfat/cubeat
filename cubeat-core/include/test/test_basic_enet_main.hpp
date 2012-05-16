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

    void on_connected(std::string const& msg);
    void on_matched(std::string const& msg);
    void on_disconnected(std::string const& msg);
    void on_received(std::string const& msg);

    void update_cursor_pos();
    void initiate_matching();

private:
    void on_mov(int, int);

    presenter::pStage stage_;
    int type_;
};

} //test
} //psc

#endif //_SHOOTING_CUBES_TEST_TEST_BASIC_ENET_MAIN_

