
#include "presenter/Object.hpp"
#include "App.hpp"
#include <iostream>

using namespace psc;
using namespace presenter;
using std::tr1::bind;
using namespace std::tr1::placeholders;

Object::Object()
    :loading_complete_(0)
{
    loading_complete_ = bind(&App::setLoading, &App::i(), _1);
}

Object::~Object()
{
}

void Object::init()
{
}

void Object::precondition()
{
}

void Object::cycle()
{
}
