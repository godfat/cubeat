
#ifndef _CUBEAT_MODEL_SIMPLECUBE_
#define _CUBEAT_MODEL_SIMPLECUBE_

/* 2009.1 by arch.jslin
   This class is intended for AI calculations, and try to
   maxmize code-reusing between this class and the model::Cube class
*/

#ifndef NDEBUG
#include <iostream>
#endif

#include "data/Cube.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

namespace psc {

namespace data {
typedef Cube::pointer_type pCube;
}

namespace model {

class SimpleMap;
typedef std::tr1::shared_ptr<SimpleMap> pSimpleMap;
typedef std::tr1::weak_ptr<SimpleMap>  wpSimpleMap;

class Chain;
typedef std::tr1::shared_ptr<Chain> pChain;

class SimpleCube: public std::tr1::enable_shared_from_this<SimpleCube>
{
public:
    typedef std::tr1::shared_ptr<SimpleCube> pointer_type;
    typedef std::tr1::weak_ptr<SimpleCube>  wpointer_type;

    static pointer_type create(wpSimpleMap map, int x = 0, int y = 0, int color_id = 0) {
        return utils::ObjectPool<SimpleCube>::create(map, x, y, color_id)->init();
    }
    static pointer_type create(int x = 0, int y = 0, int color_id = 0) {
        return utils::ObjectPool<SimpleCube>::create(x, y, color_id)->init();
    }

    SimpleCube(wpSimpleMap map, int x = 0, int y = 0, int color_id = 0):
        data_(data::Cube::create(x, y, color_id)), has_grounded_(false),
        is_broken_(false), is_garbage_(false), is_new_garbage_(false),
        hp_(1), time_to_lose_chain_(0), map_(map), is_dead_(false)
    {}
    SimpleCube(int x = 0, int y = 0, int color_id = 0):
        data_(data::Cube::create(x, y, color_id)), has_grounded_(false),
        is_broken_(false), is_garbage_(false), is_new_garbage_(false),
        hp_(1), time_to_lose_chain_(0), map_(pSimpleMap()), is_dead_(false)
    {}

    ~SimpleCube(){ lose_chain(); }

    int x() const{ return data()->x(); }
    int y() const{ return data()->y(); }
    double real_y() const{ return data()->real_y(); }
    int color_id() const{ return data()->color_id(); }
    SimpleCube& x(int new_x){ data()->x(new_x); return *this; }
    SimpleCube& y(int new_y){ data()->y(new_y); return *this; }
    SimpleCube& real_y(double new_real_y){ data()->real_y(new_real_y); return *this; }
    SimpleCube& color_id(int new_color_id){ data()->color_id(new_color_id); return *this; }

    // differs from model::Cube
    void chain(pChain chain)   { chain_belonged_to_ = chain; }
    pChain const chain() const { return chain_belonged_to_; }
    void lose_chain()          { chain_belonged_to_.reset(); }
    void go_dying()            { is_dead_ = true; }
    bool cycle_and_die();

    // differs from model::Cube
    pSimpleMap map()            { return map_.lock(); }
    pSimpleMap const map() const{ return map_.lock(); }

    // used by MapUtils
    friend inline bool operator!=(SimpleCube const& lhs, SimpleCube const& rhs){
        return lhs.color_id() == -1 || rhs.color_id() == -1 ||
               lhs.color_id() != rhs.color_id();
    }
    // end

    bool is_garbage()   const{ return is_garbage_; }
    bool is_broken()    const{ return is_broken_; }
    bool is_new_garbage()const{return is_new_garbage_; }
	bool has_grounded() const{ return has_grounded_; }
	int  hp()           const{ return hp_; }
	void set_grounded() { has_grounded_ = true; new_garbage(false); }
	void new_garbage(bool const& f) { is_new_garbage_ = f; }

	// pure dummy. Have to use this in OneFading<T>. I couldn't find a way to avoid it.
	bool is_waiting() const{ return false; }


    data::pCube data() const{ return data_; }
    SimpleCube& data(data::pCube const& new_data){ data_ = new_data; return *this; }

    pointer_type dump_data_to_map(wpSimpleMap) const;

protected:
    data::pCube   data_;
    pChain        chain_belonged_to_;
    bool has_grounded_, is_broken_, is_garbage_, is_new_garbage_;
	int hp_;
	time_t time_to_lose_chain_;

private:
    pointer_type init() {
        return shared_from_this();
    }
    wpSimpleMap map_;
    bool is_dead_;
};

typedef SimpleCube::pointer_type pSimpleCube;
typedef SimpleCube::wpointer_type wpSimpleCube;

} //model
} //psc

#endif
