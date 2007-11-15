
/* 2007.11.9
   Nothing changed.
*/

#ifndef _SHOOTING_CUBE_POOLED_OBJECT_HPP_
#define _SHOOTING_CUBE_POOLED_OBJECT_HPP_

class PooledObject{
public:
    PooledObject(): dead_(true){}
    void suicide(){ dead_ = true; this->do_suicide(); }

    // methods below only for ObjectPool to call
    bool dead() const{ return dead_; }
    template <class Derived>
    Derived& init(){ dead_ = false; this->do_init(); return static_cast<Derived&>(*this); }
    virtual ~PooledObject(){}

private:
    virtual void do_init() = 0;
    virtual void do_suicide() = 0;

private:
    bool dead_;
};

#endif
