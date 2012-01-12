
#include "presenter/game/Puzzle.hpp"
#include <cstdio>

using namespace psc;

class TestPuzzle : public presenter::game::Puzzle
{
public:
    typedef std::tr1::shared_ptr<TestPuzzle> pointer_type;
    static pointer_type create(std::string const& c1p, std::string const& sc, int puzzle_level) {
        return utils::ObjectPool<TestPuzzle>::create()->init(c1p,sc,puzzle_level);
    }

    TestPuzzle() {}
    virtual ~TestPuzzle() {
        printf("TestPuzzle Game destructing ...\n");
    }

    void cycle() { //shadow Puzzle::cycle directly;
        presenter::game::Puzzle::cycle();
    }

protected:
    pointer_type init(std::string const& c1p, std::string const& sc, int puzzle_level) {
        presenter::game::Puzzle::init(c1p, sc, puzzle_level);
        return std::tr1::static_pointer_cast<TestPuzzle>(shared_from_this());
    }
};

typedef TestPuzzle::pointer_type pTestPuzzle;

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    pTestPuzzle tester = TestPuzzle::create(0, 0, 5);
    return psc::App::i().run(std::tr1::bind(&TestPuzzle::cycle, tester.get()));
}
