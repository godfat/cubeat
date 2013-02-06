// Isolate these to a specific translation unit.
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>
#include <boost/tr1/tuple.hpp>
#include <boost/tr1/functional.hpp>

// Mark some classes that you want to be restorable
// can include headers or use forward declarations.
//#include "Map.hpp"
//#include "Cube.hpp"

#include "ctrl/TimerDispatcher.hpp"

namespace psc {
namespace utils {

namespace details {

template<typename CharOrSPtr, size_t EleSize = sizeof(CharOrSPtr)>
class SpecializedPool {
    typedef boost::singleton_pool<CharOrSPtr, EleSize> this_pool;
    typedef typename this_pool::pool_type pool_type;

public:
    static void backup(int frame_number) {
        backups_.push_back( pool_type() );      // make sure the place holder (tmp pool) is destructed right away
        this_pool::clone_to( backups_.back() ); // So that it won't call destructor when tmp pool is out of scope.
        tracked_frame_number_ = frame_number;

        // dump things larger than the backup window:
        if( backups_.size() > MAX_SIZE_OF_BACKUP ) {
            //printf("Backup window full.. popping (specialized_pool)\n");
            backups_.pop_front();
        }
        //printf("tracked_frame_number_ = %d, backup_size = %d (specialized pool)\n", tracked_frame_number_, backups_.size());
    }

    static void restore(int frame_number) {
        int num_of_frames_to_rollback = tracked_frame_number_ - frame_number + 1;
        for( ; num_of_frames_to_rollback > 0 ; --num_of_frames_to_rollback ) {
            if( num_of_frames_to_rollback == 1 ) {
                pool_type & backup = backups_.back();
                this_pool::restore(backup);
            }
            backups_.pop_back();
        }
    }

private:
    static std::deque<pool_type> backups_;
    static int tracked_frame_number_;
};

template<typename CharOrSPtr, size_t EleSize>
std::deque<typename SpecializedPool<CharOrSPtr, EleSize>::pool_type> SpecializedPool<CharOrSPtr, EleSize>::backups_;

template<typename CharOrSPtr, size_t EleSize>
int SpecializedPool<CharOrSPtr, EleSize>::tracked_frame_number_ = 0;

} // details

// And some unified restore // backup implementation here.

void pools_backup(int frame_number)
{
//    ObjectPoolRestorable<model::Cube>::backup(frame_number);
//    ObjectPoolRestorable<model::Map>::backup(frame_number);
//
    // used for basic_string => pooled string, or other possible uses
//    details::SpecializedPool<char>::backup(frame_number);

    // used for shared_ptrs stored in STL containers
//    details::SpecializedPool<std::tr1::shared_ptr<void> >::backup(frame_number);
    details::SpecializedPool<ctrl::TimerDispatcher::Timer, sizeof(ctrl::TimerDispatcher::Timer)+(sizeof(void*)*2)>::backup(frame_number);
}

void pools_restore(int frame_number)
{
//    ObjectPoolRestorable<model::Cube>::restore(frame_number);
//    ObjectPoolRestorable<model::Map>::restore(frame_number);
//
    // used for basic_string => pooled string, or other possible uses
//    details::SpecializedPool<char>::restore(frame_number);

    // used for shared_ptrs stored in STL containers
//    details::SpecializedPool<std::tr1::shared_ptr<void> >::restore(frame_number);
    details::SpecializedPool<ctrl::TimerDispatcher::Timer, sizeof(ctrl::TimerDispatcher::Timer)+(sizeof(void*)*2)>::restore(frame_number);
}

} // utils
} // psc
