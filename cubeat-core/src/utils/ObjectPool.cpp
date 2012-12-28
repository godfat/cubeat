// Isolate these to a specific translation unit.
#include <boost/tr1/memory.hpp>
#include "utils/ObjectPool.hpp"

// Mark some classes that you want to be restorable
// can include headers or use forward declarations.
//#include "Map.hpp"
//#include "Cube.hpp"

namespace psc {
namespace utils {

// And some unified restore // backup implementation here.

void pools_backup(int frame_number)
{
//    ObjectPoolRestorable<model::Cube>::backup(frame_number);
//    ObjectPoolRestorable<model::Map>::backup(frame_number);
//
    // used for basic_string => pooled string, or other possible uses
    details::SpecializedPool<char>::backup(frame_number);

    // used for shared_ptrs stored in STL containers
    details::SpecializedPool<std::tr1::shared_ptr<void> >::backup(frame_number);
}

void pools_restore(int frame_number)
{
//    ObjectPoolRestorable<model::Cube>::restore(frame_number);
//    ObjectPoolRestorable<model::Map>::restore(frame_number);
//
    // used for basic_string => pooled string, or other possible uses
    details::SpecializedPool<char>::restore(frame_number);

    // used for shared_ptrs stored in STL containers
    details::SpecializedPool<std::tr1::shared_ptr<void> >::restore(frame_number);
}

} // utils
} // psc
