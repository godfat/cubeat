
/* 2007.11.9
   Most things remain unchanged.

  These included file are going to be modified:
  1. Square.hpp
*/

#ifndef _SHOOTING_CUBE_FACTORY_HPP_
#define _SHOOTING_CUBE_FACTORY_HPP_

// states
#include "Waiting.hpp"
#include "Dropping.hpp"
#include "Dying.hpp"
#include "Exploding.hpp"

// others
#include "Square.hpp"
#include "Chain.hpp"

#include <loki/AbstractFactory.h>
#include <loki/Sequence.h> // used to present product list
#include <loki/Singleton.h> // used to build AbstractFactory

#include <boost/bind.hpp> // used by find_if

#include <algorithm> // used by ObjectPool, for find_if
#include <boost/ptr_container/ptr_vector.hpp> // used by ObjectPool
#include <vector> // used by ObjectPool

#include "Debugger.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

namespace{

//added
//const int INIT_POOL_SIZE = 128;

//NOTE:
//Can we know what type of object is resizing its pool??
//Or, How do we create pool for each type of object individually?

template <
    class T,
    class StoringContainer = std::vector<T*>,
    class ViewingContainer = boost::ptr_vector<T, boost::view_clone_allocator>
>
class ObjectPoolContainer{
public:
    typedef StoringContainer storing_container_type;
    typedef ViewingContainer viewing_container_type;

    typedef viewing_container_type container_type;

    typedef typename viewing_container_type::iterator
        iterator;
    typedef typename viewing_container_type::const_iterator
        const_iterator;
    typedef typename viewing_container_type::reverse_iterator
        reverse_iterator;
    typedef typename viewing_container_type::const_reverse_iterator
        const_reverse_iterator;
    typedef typename viewing_container_type::value_type
        value_type;
    typedef typename viewing_container_type::reference
        reference;
    typedef typename viewing_container_type::const_reference
        const_reference;
    typedef typename viewing_container_type::size_type
        size_type;

public:
    ObjectPoolContainer(size_type init_size){
        this->enlarge(init_size);
    }
    ~ObjectPoolContainer(){
        for(typename storing_container_type::iterator i = storing_.begin(), iend = storing_.end();
            i != iend; ++i)
        {
            delete [] *i;
        }
    }

    iterator begin(){ return viewing_.begin(); }
    const_iterator begin() const{ return viewing_.begin(); }

    iterator end(){ return viewing_.end(); }
    const_iterator end() const{ return viewing_.end(); }

    reverse_iterator rbegin(){ return viewing_.rbegin(); }
    const_reverse_iterator rbegin() const{ return viewing_.rbegin(); }

    reverse_iterator rend(){ return viewing_.rend(); }
    const_reverse_iterator rend() const{ return viewing_.rend(); }

    size_type size() const{ return viewing_.size(); }
    void enlarge(size_type amounts){
        storing_.push_back(new T[amounts]);
        viewing_.insert(viewing_.end(), storing_.back(), storing_.back()+amounts);
    }

private:
    storing_container_type storing_;
    viewing_container_type viewing_;
};

template <
    class T,
    int init_size = 128,
    int enlarging_rate = 1,
    class C = ObjectPoolContainer<T>
>
class ObjectPool{
public:
    typedef T* pointer_type;
    typedef C container_type;
    typedef typename container_type::size_type size_type;

public:
    ObjectPool(): data_(init_size), top_(data_.rbegin())
    {
		//for(int i=0; i<init_size; ++i)
			//data_.push_back(new T);
	}
    ObjectPool(size_type size): data_(size), top_(data_.rbegin())
    {
		//for(int i=0; i<size; ++i)
			//data_.push_back(new T);
	}

    T* make(){
        find_next(top_);
        if( top_ != data_.rend() )
            return got_it();
        else if( find_next(data_.rbegin()) != data_.rend() )
            return got_it();
        else
            return make_more(), make();
    }

    #ifndef NDEBUG
    size_type report() const{
        std::cout << "ObjectPool: " << typeid(T).name() << ": size: "
                  << data_.size() << std::endl;
        return data_.size();
    }
    #endif

private:
    void make_more(){
        data_.enlarge(static_cast<size_type>(data_.size()*enlarging_rate));
        top_ = data_.rbegin();
    }

    typedef typename container_type::reverse_iterator r_iter;
    r_iter find_next(r_iter first){
        return top_ = find_if(first, data_.rend(), boost::bind(&T::dead, _1));
    }

    T* got_it(){
        return &top_++->PooledObject::template init<T>();
    }

private:
    container_type data_;
    r_iter top_;
};



template <class ConcreteProduct, class Base>
class ObjectPoolFactoryUnit: public Base{
    typedef typename Base::ProductList BaseProductList;
	ObjectPool<ConcreteProduct> object_pool_;

protected:
    typedef typename BaseProductList::Tail ProductList;

public:
    typedef typename BaseProductList::Head AbstractProduct;
    ConcreteProduct* DoCreate(Loki::Type2Type<AbstractProduct>){ return object_pool_.make(); }
    typename ObjectPool<ConcreteProduct>::size_type do_report(Loki::Type2Type<AbstractProduct>){
        return object_pool_.report();
    }
};



#ifndef NDEBUG
template <class T>
class ReportingAFactoryUnit{
public:
    virtual T* DoCreate(Loki::Type2Type<T>) = 0;
    virtual size_t do_report(Loki::Type2Type<T>) = 0;
    virtual ~ReportingAFactoryUnit(){}
};

template <class TList>
class ReportingAFactory: public Loki::AbstractFactory<TList, ReportingAFactoryUnit>{
public:
    template <class T> size_t report(){
        ReportingAFactoryUnit<T>& unit = *this;
        return unit.do_report(Loki::Type2Type<T>());
    }
};
#endif


typedef Loki::Seq<Waiting, Dropping, Dying, Exploding, Chain, Square>::Type Products;
#ifndef NDEBUG
typedef Loki::ConcreteFactory<ReportingAFactory<Products>, ObjectPoolFactoryUnit> CFactory;
#else
typedef Loki::ConcreteFactory<Loki::AbstractFactory<Products>, ObjectPoolFactoryUnit> CFactory;
#endif

} // local namespace

typedef Loki::SingletonHolder<CFactory> SF;


#endif
