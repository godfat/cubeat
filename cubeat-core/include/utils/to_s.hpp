
#ifndef _SHOOTING_CUBES_UTILS_TO_S_
#define _SHOOTING_CUBES_UTILS_TO_S_

#include <boost/lexical_cast.hpp>
#include <string>

namespace psc{ namespace utils{

template <class T>
inline std::string to_s(T t) { return boost::lexical_cast<std::string>(t); }

}} // end of namespace

#endif
