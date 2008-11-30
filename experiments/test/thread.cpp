
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

boost::mutex m;

struct f{
    f(int d): d_(d){}
    void operator()(){
        for(int i=0; i<10; ++i){
            boost::mutex::scoped_lock lock(m);
            std::cout << d_ << std::endl;
        }
    }
private:
    int d_;
};

int main(){
    boost::thread t1(f(1)), t2(f(2));
    t1.join(); t2.join();
}
