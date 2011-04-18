#ifndef _SHOOTING_CUBES_UTILS_GAUSS_LEGENDRE_INTEGRATION_
#define _SHOOTING_CUBES_UTILS_GAUSS_LEGENDRE_INTEGRATION_

// Originally on 2008.3.15, and modified on 2011.4.18:
//
// This part of the program is about Gauss-Legendre Numerical Integration.
// This part of the program is a derived work of the Flash ActionScript 3
// "Singularity Library", written by Jim Armstrong (www.algorithmist.net)
//
// The original copyright notice is presented here unmodified:
//////////////////////////////////////////////////////////////////////////
// Gauss.as - Gauss-Legendre Numerical Integration.
//
// copyright (c) 2006-2007, Jim Armstrong.  All Rights Reserved.
//
// This software program is supplied 'as is' without any warranty, express, implied,
// or otherwise, including without limitation all warranties of merchantability or fitness
// for a particular purpose.  Jim Armstrong shall not be liable for any special
// incidental, or consequential damages, including, without limitation, lost
// revenues, lost profits, or loss of prospective economic advantage, resulting
// from the use or misuse of this software program.
//
// Programmed by:  Jim Armstrong, Singularity (www.algorithmist.net)
//////////////////////////////////////////////////////////////////////////
//
// Ported to C++ on 2008.3.15 by arch.jslin (arch.jslin@gmail.com) to be
// used in Irrlicht environment. Any derived work from this piece of code
// must maintain this copyright notice and must not be resold in the form
// of source code.

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
