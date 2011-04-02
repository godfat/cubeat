#ifndef _SHOOTING_CUBES_UTILS_GAUSS_LEGENDRE_INTEGRATION_
#define _SHOOTING_CUBES_UTILS_GAUSS_LEGENDRE_INTEGRATION_

// Gauss-Legendre Numerical Integration.
// Reference and thanks to:
//     Jim Armstrong, Singularity (www.algorithmist.net)
//
// 2008.3.15 by arch.jslin (archilifelin@yahoo.com.tw)
// the code originally comes from the actionscript Singularity Library by
// Jim Armstrong. I ported the Gauss-Legendre Integration code to C++.

#include <boost/tr1/functional.hpp>

#include <utility>
#include <algorithm>

#include <cstddef>

namespace psc {
namespace utils {
class GaussLegendre{
public:
    static float integrate(std::tr1::function<float(float)>const& integrand,
                           float start,
                           float end,
                           std::size_t const& num_of_points)
    {
        if( start == end ) return 0.f;
        if( start > end )  std::swap(start, end);
        std::size_t n = std::min( std::max( static_cast<std::size_t>(2), num_of_points ), MAX_INTEGRATION_POINTS );
        std::size_t l = (n==2) ? 0 : n*(n-1)/2 - 1;
        float sum = 0.f;

        float mul = 0.5f*( end - start );
        float ab2 = 0.5f*( start + end );
        for( std::size_t i=0; i<n; ++i )
            sum += integrand( ab2 + mul*t_table_[l+i] ) * weight_[l+i];

        return mul*sum;
    }

private:
    static std::size_t const MAX_INTEGRATION_POINTS;
    static std::size_t const SIZE;
    static double const t_table_[];  //different progression percentage for integration
    static double const weight_[];   //different weight for different progression percentage points.
};

} //psc
} //utils

#endif //_SHOOTING_CUBES_UTILS_GAUSS_LEGENDRE_INTEGRATION_
