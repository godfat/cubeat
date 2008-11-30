
#ifndef _SHOOTING_CUBES_UTILS_OBJECT_POOL_
#define _SHOOTING_CUBES_UTILS_OBJECT_POOL_

#include <boost/pool/object_pool.hpp>
#include <loki/Singleton.h>

namespace psc{

template <class T>
class ObjectPool{
public:
    typedef typename T::pointer_type pointer_type;
    typedef Loki::SingletonHolder<boost::object_pool<T>, Loki::CreateUsingNew, Loki::DeletableSingleton> pool_type;

public:
    static pointer_type create(){
        return pointer_type(pool_type::Instance().construct(), Deleter());
    }
    static void destroy_all(){
        Loki::DeletableSingleton< boost::object_pool<T> >::GracefulDelete();
    }
private:
    static bool is_from(T* t){
        return pool_type::Instance().is_from(t);
    }
    static void destroy(T* t){
        pool_type::Instance().destroy(t);
    }

    friend class Deleter;
    class Deleter{
    public:
        typedef void result_type;
        typedef T* argument_type;

    public:
        void operator()(argument_type p){
            if( ObjectPool<T>::is_from(p) )
                ObjectPool<T>::destroy(p);
        }
    };
};

} // end of namespace

#endif
