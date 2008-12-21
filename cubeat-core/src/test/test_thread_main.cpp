
#include "presenter/Stage.hpp"
#include "EventDispatcher.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"

#include <tr1/functional> // for bind
#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp>

using namespace psc;

class TestThread{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    TestThread(){
        //temporary Scene:

        utils::map_any stage = utils::map_any::construct( utils::fetchConfig("config/test_stage.zzml") );
        stage_ = presenter::Stage::create( stage.S("test_stage") );
        stage_->playBGM();

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 1), 3000, -1);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&presenter::Stage::hitGroup, stage_.get(), 2), 6000, -1);

//  setup data and flag:
        data_needed_to_be_calculated_ = 0;
        is_calculating_ = false;

    let_us_calculate();

//  persistently probing data:
        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestThread::probe_data, this), 500, -1);
    }

    void let_us_calculate() {
//  using thread:
        std::cout << "start to calculate...." << std::endl;
        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestThread::start_thread, this), 2000);

//  not using thread:
//        ctrl::EventDispatcher::i().subscribe_timer(
//            std::tr1::bind(&TestThread::do_thread_work, this), 2000);
    }

    void start_thread() {
        if( !is_calculating_ ) {
            pthread_ = boost::shared_ptr<boost::thread>(
                new boost::thread(std::tr1::bind(&TestThread::do_thread_work, this)));
        } else { //if the calculation isn't done yet, wait for a little more time
            ctrl::EventDispatcher::i().subscribe_timer(
                std::tr1::bind(&TestThread::start_thread, this), 1000);
        }
    }

    void do_thread_work() {
        //boost::mutex::scoped_lock lk(mutex_);
        is_calculating_ = true;
        std::cout << "Hi!." << std::endl;
        for( volatile int i = 0; i < 200000000; i++ ) {
            data_needed_to_be_calculated_ += 1;
            if( i % 20000000 == 0 ) std::cout << "calculating..." << std::endl;
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        std::cout << "done calculating." << std::endl;
        is_calculating_ = false;
        if( data_needed_to_be_calculated_ < 1000000000 )
            let_us_calculate();
        else
            stop_calculating();
    }

    void stop_calculating() {
        if( pthread_ ) {
            std::cout << "The calculating is done, thread joined." << std::endl;
            pthread_->join();
        }
    }

    void probe_data() {
        std::cout << "report data: " << data_needed_to_be_calculated_ << std::endl;
    }

    void cycle(){
        stage_->cycle();
    }

private:
    int data_needed_to_be_calculated_;
    bool is_calculating_;

    boost::mutex  mutex_;
    boost::shared_ptr<boost::thread> pthread_;
    presenter::pStage stage_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    TestThread tester;
    return psc::App::i().run(std::tr1::bind(&TestThread::cycle, &tester));
}
