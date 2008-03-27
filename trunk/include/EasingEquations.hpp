
#ifndef _SHOOTING_CUBES_EASING_EQUATIONS_
#define _SHOOTING_CUBES_EASING_EQUATIONS_

/**
 * C++ Port by Johnson Lin(a.k.a arch.jslin) to combine with
 * Irrlicht Animator system
 *
 * last updated: 2008.3
 * fixed some calculation which might invoke undefined behavior in C++.
 * (In Java and Flash this is not a problem, but in C++ IT IS A PROBLEM.)
 *
 * All equations are now working. there are some newly defined equations as well.
 */

/**
 * Equations
 * Main equations for the Tweener class
 *
 * @author		Zeh Fernando, Nate Chatellier
 * @version		1.0.2
 */

/*
Disclaimer for Robert Penner's Easing Equations license:

TERMS OF USE - EASING EQUATIONS

Open source under the BSD License.

Copyright ?2001 Robert Penner
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of contributors may be used
      to endorse or promote products derived from this software without specific
      prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ISceneNode.h"
#include "data/BasicViewTypes.hpp"

namespace psc {
namespace easing {

    // ==================================================================================================================================
	// TWEENING EQUATIONS functions -----------------------------------------------------------------------------------------------------
	// (the original equations are Robert Penner's work as mentioned on the disclaimer)

	/**
	 * Easing equation function for a simple linear tweening, with no easing.
	 *
	 * @param t		Current time (in frames or seconds).
	 * @param b		Starting value.
	 * @param c		Change needed in value.
	 * @param d		Expected easing duration (in frames or seconds).
	 * @return		The correct value.
	 */
    template<class T>
    class Linear
    {
    public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            return static_cast<T>( c*t/d + b );
        }
    };

	/**
	 * Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class IQuad
    {
    public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t/=d;
			return c*t*t + b;
		}
    };

	/**
	 * Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
	 */
    template<class T>
    class OQuad
    {
    public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t/=d;
            t*=(t-2);
			return -c *t + b;
		}
    };

	/**
	 * Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOQuad
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if ((t/=d/2) < 1) return c/2*t*t + b;
			--t;
			return -c/2 * (t*(t-2) - 1) + b;
		}
    };

	/**
	 * Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OIQuad
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t < d/2) return OQuad<T>::calculate(t*2, b, c/2, d, node);
			return IQuad<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
    };

	/**
	 * Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class ICubic
    {
    public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t/=d;
			return c*t*t*t + b;
		}
    };

	/**
	 * Easing equation function for a cubic (t^3) easing out: decelerating from zero velocity.
	 */
    template<class T>
    class OCubic
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t=(t/d)-1;
			return c*(t*t*t + 1) + b;
		}
    };

	/**
	 * Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOCubic
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if ((t/=d/2) < 1) return c/2*t*t*t + b;
			t-=2;
			return c/2*(t*t*t + 2) + b;
		}
    };

	/**
	 * Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OICubic
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            if (t < d/2) return OCubic<T>::calculate(t*2, b, c/2, d, node);
			return ICubic<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
    };

	/**
	 * Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class IQuart
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t/=d;
			return c*t*t*t*t + b;
		}
    };

	/**
	 * Easing equation function for a quartic (t^4) easing out: decelerating from zero velocity.
	 */
    template<class T>
    class OQuart
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t=(t/d)-1;
			return -c * (t*t*t*t - 1) + b;
		}
    };

	/**
	 * Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOQuart
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
			t-=2;
			return -c/2 * (t*t*t*t - 2) + b;
		}
    };

	/**
	 * Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OIQuart
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t < d/2) return OQuart<T>::calculate(t*2, b, c/2, d, node);
			return IQuart<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
    };

	/**
	 * Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class IQuint
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t/=d;
			return c*t*t*t*t*t + b;
		}
    };

	/**
	 * Easing equation function for a quintic (t^5) easing out: decelerating from zero velocity.
	 */
    template<class T>
    class OQuint
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t=(t/d)-1;
			return c*(t*t*t*t*t + 1) + b;
		}
    };

	/**
	 * Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOQuint
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
			t-=2;
			return c/2*(t*t*t*t*t + 2) + b;
		}
    };

	/**
	 * Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OIQuint
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t < d/2) return OQuint<T>::calculate(t*2, b, c/2, d, node);
			return IQuint<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
    };

	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class ISine
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            return -c * cosf(t/d * (irr::core::PI/2)) + c + b;
		}
    };

    //Added by arch_jslin
	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing in: going through a sine cycle
     *                                                               (back to the original point)
	 */
    template<class T>
    class SineCirc
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            return static_cast<T>(c * (-cosf(t/d * (irr::core::PI*2))+1)/2 + b);
		}
    };

    //Added by arch_jslin
	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing in: going through a sine cycle
     *                                                               (back to the original point)
	 */
    template<class T>
    class SineCircX
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            return static_cast<T>(-c/2 * sinf(t/d * (irr::core::PI*2) + (node->getPosition().X / 50.0f)) + c/2 + b);
		}
    };

    //Added by arch_jslin
	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing in: going through a sine cycle
     *                                                               (back to the original point)
	 */
    template<class T>
    class SineCircY
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
           return static_cast<T>(-c/2 * sinf(t/d * (irr::core::PI*2) + (node->getPosition().Y / 50.0f)) + c/2 + b);
		}
    };

    //Added by arch_jslin
	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing in: going through a sine cycle
     *                                                               (back to the original point)
	 */
    template<class T>
    class SineCircZ
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            return static_cast<T>(-c/2 * sinf(t/d * (irr::core::PI*2) + (node->getPosition().Z / 50.0f)) + c/2 + b);
		}
    };

	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing out: decelerating from zero velocity.
	 */
    template<class T>
    class OSine
    {
    public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			return c * sinf(t/d * (irr::core::PI/2)) + b;
		}
    };

	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOSine
    {
    public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			return -c/2 * (cosf(irr::core::PI*t/d) - 1) + b;
		}
    };

	/**
	 * Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OISine
    {
    public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t < d/2) return OSine<T>::calculate(t*2, b, c/2, d, node);
			return ISine<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
    };

    /**
	 * Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class IExpo
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b - c * 0.001f;
		}
    };

    /**
	 * Easing equation function for an exponential (2^t) easing out: decelerating from zero velocity.
	 */
    template<class T>
    class OExpo
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			return (t==d) ? b+c : c * 1.001f * (-pow(2, -10 * t/d) + 1) + b;
		}
    };

	/**
	 * Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOExpo
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t==0) return b;
			if (t==d) return b+c;
			if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b - c * 0.0005f;
			return c/2 * 1.0005f * (-pow(2, -10 * --t) + 2) + b;
		}
    };

	/**
	 * Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OIExpo
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t < d/2) return OExpo<T>::calculate(t*2, b, c/2, d, node);
			return IExpo<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
    };

	/**
	 * Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class ICirc
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t/=d;
			return -c * (sqrt(1 - t*t) - 1) + b;
		}
    };

	/**
	 * Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating from zero velocity.
	 */
    template<class T>
    class OCirc
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            t=(t/d)-1;
			return c * sqrt(1 - t*t) + b;
		}
    };

	/**
	 * Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOCirc
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
			t-=2;
			return c/2 * (sqrt(1 - t*t) + 1) + b;
		}
    };

	/**
	 * Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OICirc
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t < d/2) return OCirc<T>::calculate(t*2, b, c/2, d, node);
			return ICirc<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
    };

//helper function
template <class T>
inline float length(T const& t) { return abs(t); }
template <>
inline float length<vec2>(vec2 const& t) { return t.getLength(); }
template <>
inline float length<vec3>(vec3 const& t) { return t.getLength(); }

	/**
	 * Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
     * @param a		Amplitude.
     * @param p		Period.
	 */
    template<class T>
    class IElastic
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           T a = T(), float p=.0f) //additional param.
        {
			if(t==0) return b;  if((t/=d)==1) return b+c;  if(p==.0f) p=d*.3;
			float s = .0f;
			float la = length(a), lc = length(c); bool sign = ((c/a) >= T()) ? true : false;
			if( la==.0f || la < lc ) { a=c; s=p/4; }
			else s = p / (2*irr::core::PI) * asinf (lc/la * (sign?1:-1));
			t-=1;
			return -(a*pow(2,10*t) * sinf( (t*d-s)*(2*irr::core::PI)/p )) + b;
		}
    };

	/**
	 * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating from zero velocity.
     * @param a		Amplitude.
     * @param p		Period.
	 */
    template<class T>
    class OElastic
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           T a = T(), float p=.0f) //additional param.
        {
			if(t==0) return b;  if((t/=d)==1) return b+c;  if(p==.0f) p=d*.3;
			float s = .0f;
			float la = length(a), lc = length(c); bool sign = ((c/a) >= T()) ? true : false;
			if( la==.0f || la < lc ) { a=c; s=p/4; }
			else s = p / (2*irr::core::PI) * asinf (lc/la * (sign?1:-1));
			return (a*pow(2,-10*t) * sinf( (t*d-s)*(2*irr::core::PI)/p ) + c + b);
		}
    };

	/**
	 * EEasing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
     * @param a		Amplitude.
     * @param p		Period.
	 */
    template<class T>
    class IOElastic
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           T a = T(), float p=.0f) //additional param.
        {
			if (t==0) return b;  if ((t/=d/2)==2) return b+c;  if(p==.0f) p=d*(.3*1.5);
			float s = .0f;
			float la = length(a), lc = length(c); bool sign = ((c/a) >= T()) ? true : false;
			if ( length(a)==.0f || length(a) < length(c) ) { a=c; s=p/4; }
			else s = p/(2*irr::core::PI) * asinf (lc/la * (sign?1:-1));
			t-=1;
			if (t < 1) return -.5*(a*pow(2,10*t) * sinf( (t*d-s)*(2*irr::core::PI)/p )) + b;
			return a*pow(2,-10*t) * sinf( (t*d-s)*(2*irr::core::PI)/p )*.5 + c + b;
		}
    };

	/**
	 * Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
     * @param a		Amplitude.
     * @param p		Period.
	 */
    template<class T>
    class OIElastic
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           T const& a = T(), float const& p=.0f) //additional param.
        {
			if (t < d/2) return OElastic<T>::calculate(t*2, b, c/2, d, node, a, p);
			return IElastic<T>::calculate((t*2)-d, b+c/2, c/2, d, node, a, p);
		}
    };

	/**
	 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
     * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot,
     *              and the default value of 1.70158 produces an overshoot of 10 percent).
	 */
    template<class T>
    class IBack
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           float s=.0f) //additional param.
        {
			if (s==.0f) s = 1.70158f;
			t/=d;
			return c*t*t*((s+1)*t - s) + b;
		}
    };

	/**
	 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating from zero velocity.
     * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot,
     *              and the default value of 1.70158 produces an overshoot of 10 percent).
	 */
    template<class T>
    class OBack
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           float s=.0f) //additional param.
        {
			if (s==.0f) s = 1.70158f;
			t=(t/d)-1;
			return c*(t*t*((s+1)*t + s) + 1) + b;
		}
    };

	/**
	 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
     * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot,
     *              and the default value of 1.70158 produces an overshoot of 10 percent).
	 */
    template<class T>
    class IOBack
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           float s=.0f) //additional param.
        {
			if (s==.0f) s = 1.70158f;
			s*=1.525;
			if ((t/=d/2) < 1) return c/2*(t*t*((s+1)*t - s)) + b;
			t-=2;
			return c/2*(t*t*((s+1)*t + s) + 2) + b;
		}
    };

	/**
	 *  Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OIBack
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0,
                           float s=.0f) //additional param.
        {
			if (t < d/2) return OBack<T>::calculate(t*2, b, c/2, d, node, s);
			return IBack<T>::calculate((t*2)-d, b+c/2, c/2, d, node, s);
		}
    };

	/**
	 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating from zero velocity.
	 */
    template<class T>
    class OBounce
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if ((t/=d) < (1/2.75f)) {
				return c*(7.5625f*t*t) + b;
			} else if (t < (2/2.75f)) {
			    t-=(1.5f/2.75f);
				return c*(7.5625f*t*t + .75f) + b;
			} else if (t < (2.5f/2.75f)) {
			    t-=(2.25f/2.75f);
				return c*(7.5625f*t*t + .9375f) + b;
			} else {
			    t-=(2.625f/2.75f);
				return c*(7.5625f*t*t + .984375f) + b;
			}
		}
    };

    /**
	 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
	 */
    template<class T>
    class IBounce
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			return c - OBounce<T>::calculate(d-t, T(), c, d, node) + b;
		}
    };

	/**
	 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
	 */
    template<class T>
    class IOBounce
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
			if (t < d/2) return IBounce<T>::calculate(t*2, T(), c, d, node) * .5 + b;
			else return OBounce<T>::calculate(t*2-d, T(), c, d, node) * .5 + c*.5 + b;
		}
    };

	/**
	 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
	 */
    template<class T>
    class OIBounce
    {
	public:
        static T calculate(float t, T const& b, T const& c, float const& d, irr::scene::ISceneNode* node=0) {
            if (t < d/2) return OBounce<T>::calculate(t*2, b, c/2, d, node);
			return IBounce<T>::calculate((t*2)-d, b+c/2, c/2, d, node);
		}
	};

} //easing
} //psc

#endif //_SHOOTING_CUBES_EASING_EQUATIONS_
