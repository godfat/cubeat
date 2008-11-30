
// http://lists.boost.org/boost-users/2005/01/9521.php
/*
From: Peter Dimov (pdimov_at_[hidden])
Date: 2005-01-17 06:13:01

In reply to: Niklas Wiberg: "[Boost-users] shared_from_this and custom deleters"

Niklas Wiberg wrote: 
> Hi, 
> I have the following situation: 
> In the constructor body of class A, I need to call functions that 
> take a shared_ptr<A> as a parameter. These functions do not store the 
> shared_ptr<A>, but in some situations a weak_ptr<A>. 
> There is not yet any shared_ptr owning the A instance, since this is 
> during A construction... 
> 
> At first, I thought using a shared_ptr<A> tmpThis(this, NullDeleter()) 
> was a good idea. 
> But when I later on (when another shared_ptr actually owns the A 
> instance) want to use shared_from_this on A, I get an exception since 
> the use_count of the internal weak_ptr is based on the first temporary 
> shared_ptr that I used during construction (with the NullDeleter). 
> 
> Am I correct in my assumption that the internal weak_ptr in 
> enable_shared_from_this is initialized by the first shared_ptr and 
> that will prevent any later use of shared_from_this when I have 
> another shared_ptr (with a default deleter) owning the element?

In the current implementation, every shared_ptr constructed with a pointer 
to the object will overwrite the internal weak_ptr in 
enable_shared_from_this, so the last one wins. But this is an implementation 
detail; if you decide to port your code to a TR1 implementation of 
shared_ptr, it may or may not work. 

> I guess that a static create function would solve the problem, but 
> changing that would be quite a big undertaking in this particular case 
> so I thought I'd check if someone knows a simple solution to this... 

The static create function is the simple solution. ;-) 

> I added a small example code snipped below... 

Your example works for me. No exception is thrown. But: 

> void foo(shared_ptr<A> a); 
> void otherFunction(shared_ptr<A> a); 
> 
> class A : public enable_shared_from_this<A> { 
> A() { 
> // Can't use shared_from_this since no shared_ptr owns this yet 
> shared_ptr<A> thisTmp(this, NullDeleter()); 
> foo(thisTmp); 
> } 

If foo only stores a weak_ptr, it will expire immediately at the end of the 
constructor, so I'm not sure what are you trying to accomplish.
*/

#include <tr1/memory>
#include <iostream>

class Object{};

template <class T>
class Deleter{
public:
    typedef void result_type;
    typedef T* argument_type;

public:
    void operator()(argument_type p){std::cout<<"del"<<std::endl;}
};

class Test: public Object/*, private std::tr1::enable_shared_from_this<Test>*/{
public:
    std::tr1::shared_ptr<Test> f(){
        // return shared_from_this();
        return std::tr1::shared_ptr<Test>(this, Deleter<Test>());
    }
};

int main(){
    Test t;

    std::cout << "start" << std::endl;
    std::tr1::shared_ptr<Test> XD = t.f();
    // std::tr1::shared_ptr<Test> haha(&t, Deleter<Test>());
    std::tr1::shared_ptr<Test> haha = XD;
    std::cout << "end" << std::endl;
    // haha.reset();
    std::cout << "XD" << std::endl;
    // XD.reset();
    // std::cout << (std::tr1::get_deleter<Test>(haha) == std::tr1::get_deleter<Test>(XD)) << std::endl;
}
