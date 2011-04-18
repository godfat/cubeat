
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

#include "utils/GaussLegendre.hpp"

using namespace psc;
using namespace utils;

size_t const GaussLegendre::MAX_INTEGRATION_POINTS = 8;
size_t const GaussLegendre::SIZE =
    (GaussLegendre::MAX_INTEGRATION_POINTS + 2)*(GaussLegendre::MAX_INTEGRATION_POINTS - 1)/2;

double const GaussLegendre::t_table_[] =
{ -0.5773502692, 0.5773502692,    // N=2
  -0.7745966692, 0.7745966692, 0,     //N=3
  -0.8611363116, 0.8611363116, -0.3399810436, 0.3399810436, //N=4
  -0.9061798459, 0.9061798459, -0.5384693101, 0.5384693101, 0.0000000000, //N=5
  -0.9324695142, 0.9324695142, -0.6612093865, 0.6612093865, -0.2386191861, 0.2386191861, //N=6
  -0.9491079123, 0.9491079123, -0.7415311856, 0.7415311856, -0.4058451514, 0.4058451514, 0.0000000000, //N=7
  -0.9602898565, 0.9602898565, -0.7966664774, 0.7966664774, -0.5255324099, 0.5255324099,-0.1834346425, 0.1834346425 //N=8
};
double const GaussLegendre::weight_[] =
{
  1, 1, // N=2
  0.5555555556, 0.5555555556, 0.8888888888, //N=3
  0.3478548451, 0.3478548451, 0.6521451549, 0.6521451549, //N=4
  0.2369268851, 0.2369268851, 0.4786286705, 0.4786286705, 0.5688888888, //N=5
  0.1713244924, 0.1713244924, 0.3607615730, 0.3607615730, 0.4679139346, 0.4679139346, //N=6
  0.1294849662, 0.1294849662, 0.2797053915, 0.2797053915, 0.3818300505, 0.3818300505, 0.4179591837, //N=7
  0.1012285363, 0.1012285363, 0.2223810345, 0.2223810345, 0.3137066459, 0.3137066459, 0.3626837834, 0.3626837834 //N=8
};
