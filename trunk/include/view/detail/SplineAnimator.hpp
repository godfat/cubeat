#ifndef _SHOOTING_CUBES_SPLINE_ANIMATOR_
#define _SHOOTING_CUBES_SPLINE_ANIMATOR_

// A Catmull-Rom Spline animator.
// Basically this animator has even higher complexity in both
// time and space than WaypointAnimator, so use it with caution.
// arch.jslin - archilifelin@yahoo.com.tw (2008.3.19)

// Reference and thanks to:
//     Jim Armstrong, Singularity (www.algorithmist.net)

// TODO:
// There are some potential bug about float number's equality.
// If other compiler handles float numbers too differently,
// some boundary-problem will happen. (will try to fix in the future.)
// and the current version has many redundant code that need to be cleaned up.

#include "Accessor_proto.hpp"
#include "view/detail/CustomAnimator.hpp"
#include "utils/GaussLegendre.hpp"

//for debug & warning purpose
#include <iostream>

#include <vector>

namespace irr{

namespace scene{

template <template <class> class Eq, class Acc>
class SplineAnimator : public CustomAnimator<Eq, psc::accessor::Accessor<float, Acc::TYPE> >
{
    typedef CustomAnimator
        <Eq, psc::accessor::Accessor
            <float, Acc::TYPE> >                    SplineAnimatorBase;
    typedef typename SplineAnimatorBase::T          Primitive;
    typedef typename Acc::value_type                WaypointType;
    typedef std::vector< typename Acc::value_type > Waypoints;

    template<class WaypointType>
    struct CubicCoefficient {
    private:
        WaypointType a_;
        WaypointType b_;
        WaypointType c_;
        WaypointType d_;
        template <class U>
        inline float lengthOf( U const& vec )               { return absf(vec); }
        inline float lengthOf( core::vector3df const& vec ) { return vec.getLength(); }
        inline float lengthOf( core::vector2df const& vec ) { return vec.getLength(); }
    public:
        CubicCoefficient()
            :a_(WaypointType()), b_(a_), c_(b_), d_(c_){}
        CubicCoefficient(WaypointType const& a, WaypointType const& b, WaypointType const& c, WaypointType const& d)
            :a_(a), b_(b), c_(c), d_(d){}
        inline WaypointType valueAt(float const& t) {
            return (((a_*t) + b_)*t + c_)*t + d_;
        }
        inline WaypointType derivAt(float const& t) {
            return ((3.f*a_)*t + 2.f*b_)*t + c_;
        }
        inline float getLengthAt(float const& t) {
            return lengthOf( derivAt(t) );
        }
        CubicCoefficient& operator=(CubicCoefficient const& other) {
            a_ = other.a_; b_ = other.b_; c_ = other.c_; d_ = other.d_; return *this;
        }
    };

public:
    SplineAnimator(ISceneManager* smgr, Waypoints const& points, u32 duration,
                   int const& loop = 0, std::tr1::function<void()> cb = 0,
                   s32 delayTime = 0, std::vector<float>& tensions = std::vector<float>(),
                   bool const& isClosed = false)
        :SplineAnimatorBase(smgr, 0, 0, duration, loop, cb, delayTime),
         waypoints_(points), spline_index_(waypoints_.size()/2*3), isClosed_(isClosed)
    {
        if( points.size() < 2 ) {
            std::cout << "SplineAnimator: no point to go! (you at least need 2 points!)\n";
            return;
        }
        buildEndPoints();
        tensions.insert(tensions.begin(), 0.5f);
        tensions.push_back(0.5f);
        if( tensions.size() < waypoints_.size() )
            tensions.assign(waypoints_.size(), 0.5f);
        buildCoefficients<WaypointType>(coefs_, waypoints_, tensions);
        calculateDistances();
        buildSplineParam();
    }

    virtual ~SplineAnimator(){}

    virtual void animateNode(ISceneNode* node, u32 timeMs) {
        if ( !node ) return;
        if ( static_cast<s32>(timeMs) < this->startTime_ ) return;
        if ( waypoints_.size() < 2 ) {
            this->smgr_->addToAnimatorDeletionQueue(this, node);
            return;
        }

        if( timeMs - this->startTime_ >= this->duration_ ) {
            /* we can add periodic callback here.
            if( this->periodic_cb() ) this->periodic_callback(); */
            this->startTime_ = timeMs; //or should be += this->duration_ ?
            if( this->loop_ == 0 ) {
                float const& pos = Eq<float>::calculate(1, this->start_, this->distance_, 1, node);
                calculateCurrentWaypoint(node, pos);
                if( this->cb_ ) this->cb_();
                this->smgr_->addToAnimatorDeletionQueue(this, node);
                return;
            }
            else if( this->loop_ > 0 )
                this->loop_ -= 1;
        }

        float const& pos = this->updatePos(node, timeMs);
        calculateCurrentWaypoint(node, pos);
    }

protected:
    inline int clamp_open(int idx, int size)
    {
        return ( idx<0 ? 0 : ( idx>=size ? size-1 : idx ) );
    }
    inline int clamp_close(int idx, int size)
    {
	    return ( idx<0 ? size+idx : ( idx>=size ? idx-size : idx ) );
    }
    void calculateDistances() {
        float full_length = 0;
        range_list_.push_back(0);
        for( size_t i = 1; i < waypoints_.size() - 2; ++i ) {
            full_length = getIntegratedLengthAt(i);
            range_list_.push_back( full_length );
        }
        this->end_ = this->distance_ = full_length;

        spline_.push_back( core::vector2df(0.f, 0.f) );
        float prevT = 0.0f;
        for( size_t i = 1; i < range_list_.size(); ++i ) {
            float t = static_cast<float>(i);
            int n = 3;
            for( int j=1; j <= n; ++j ) {
                float tn = prevT + (t-prevT)*(static_cast<float>(j)/n);
                spline_.push_back( core::vector2df(getIntegratedLengthAt(i, static_cast<float>(j)/n), tn) );
            }
            prevT = t;
        }
    }

    inline void calculateCurrentWaypoint(ISceneNode* node, float pos) {
        if( pos < 0 ) pos = 0.f;
        else if( pos >= range_list_.back() ) pos = range_list_.back();
        float t = evalSpline( pos );
        float u = core::fract( t );
        float mis = getIntegratedLengthAt(static_cast<int>(t)+1, u) - pos;
        float misp= mis / range_list_.back();
        float mist= abs(range_list_.size() * misp);
        float ut = (mis > 0) ? t : t+mist*5; if( ut > range_list_.size() ) ut = range_list_.size()-0.f;
        float lt = (mis < 0) ? t : t-mist*5; if( lt < 0.f ) lt = 0.f;
        t = findIntervalBinarySearch(pos, ut, lt);
        u = core::fract( t );
        Acc::set(node, coefs_[ static_cast<int>(t)+1 ].valueAt(u) );
    }

    float findIntervalBinarySearch(float const& pos, float upper_t, float lower_t) {
        float t = (upper_t + lower_t) / 2.f;
        float u = core::fract( t );
        float estimate = getIntegratedLengthAt(static_cast<int>(t)+1, u);
        while( abs( pos - estimate ) > 0.01f && abs(upper_t - lower_t) > 0.01f ) {
            if( pos > estimate ) lower_t = t;
            else if(pos < estimate) upper_t = t;
            else return t;
            t = (upper_t + lower_t) / 2.f;
            u = core::fract( t );
            estimate = getIntegratedLengthAt(static_cast<int>(t)+1, u);
        }
        return t;
    }

    inline float getIntegratedLengthAt(size_t const& idx, float const& end = 1.f)
    {
        return psc::utils::GaussLegendre::integrate( std::tr1::bind<float>(
            &CubicCoefficient<WaypointType>::getLengthAt, &coefs_[idx], std::tr1::placeholders::_1),
            0, end, 5 ) + range_list_[idx-1];
    }

    template<class W>
    void buildCoefficients(std::vector< CubicCoefficient<W> >& coefs,
                           std::vector<W> const& waypoints,
                           std::vector<float> const& tensions) {
        coefs.clear();
        coefs.push_back( CubicCoefficient<W>() ); //coef of outer start point
        int pSize = static_cast<int>(waypoints.size());
        for( int idx = 1; idx < pSize - 1 ; ++idx ) {
            W const& p0 = waypoints[ clamp_open( idx-1, pSize ) ];
            W const& p1 = waypoints[ clamp_open( idx+0, pSize ) ];
            W const& p2 = waypoints[ clamp_open( idx+1, pSize ) ];
            W const& p3 = waypoints[ clamp_open( idx+2, pSize ) ];
            //catmull-rom spline cubic coefficients.
            //tension value from 0 to 1
            float alpha = idx < static_cast<int>(tensions.size()) ? tensions[idx] : 0.f;
            W const& a = (-alpha)*p0  + (2-alpha)*p1 + (alpha-2)*p2   + (alpha)*p3;
            W const& b = (2*alpha)*p0 + (alpha-3)*p1 + (3-2*alpha)*p2 + (-alpha)*p3;
            W const& c = (-alpha)*p0  +                (alpha)*p2;
            W const& d =                 p1;

            coefs.push_back( CubicCoefficient<W>(a,b,c,d) );
        }
        coefs.push_back( CubicCoefficient<W>() ); //coef of outer end point
    }

    float evalSpline(float const& x) {
        if( spline_.size() == 0 )
            return 0.0;
        else if( spline_.size() == 1 )
            return spline_[0].Y;
        else
        {
            // determine interval
            size_t upper_b = spline_.size() - 1;
            size_t lower_b = 0;
            bool too_big   = x <= spline_[spline_index_  ].X;
            bool too_small = x >  spline_[spline_index_+1].X;
            if( too_small ) {
                ++spline_index_;
                lower_b   = spline_index_;
                too_small = x > spline_[spline_index_ + 1].X;
            }
            else if( too_big ) {
                --spline_index_;
                upper_b = spline_index_;
                too_big = x <= spline_[spline_index_].X;
            }
            while( too_big || too_small ) {
                if( too_big ) upper_b = spline_index_;
                else if( too_small ) lower_b = spline_index_;
                if( upper_b == lower_b ) break;
                spline_index_ = (upper_b + lower_b) / 2;
                too_big   = x <= spline_[spline_index_  ].X;
                too_small = x >  spline_[spline_index_+1].X;
            }
            size_t i = spline_index_;
            float delta = x - spline_[i].X;

            double b = (spline_[i+1].Y - spline_[i].Y) * hInv_[i] - h_[i] * (z_[i+1] + 2.0 * z_[i])*(1/6.0);
            double q = 0.5 * z_[i] + delta * (z_[i+1] - z_[i])*(1/6.0) * hInv_[i];
            double r = b + delta*q;
            double s = spline_[i].Y + delta*r;
            return static_cast<float>(s);
        }
    }

    void buildSplineParam() {
        std::vector<double> b, u, v;

        // pre-generate h^-1 since the same quantity could be repeatedly calculated
        for( size_t i = 0; i < spline_.size()-1; ++i )
        {
            h_.push_back( spline_[i+1].X - spline_[i].X );
            hInv_.push_back( 1.0/h_[i] );
            b.push_back( (spline_[i+1].Y - spline_[i].Y) * hInv_[i] );
        }

        // recurrence relations for u(i) and v(i) -- tridiagonal solver
        u.push_back(0.0); v.push_back(0.0);
        u.push_back( 2.0 * (h_[0] + h_[1]) );
        v.push_back( 6.0 * (b[1] - b[0]) );

        for( size_t i = 2; i < spline_.size()-1; ++i ) {
            u.push_back( 2.0 * (h_[i] + h_[i-1]) - (h_[i-1] * h_[i-1]) / u[i-1] );
            v.push_back( 6.0 * (b[i] - b[i-1])   - (h_[i-1] * v[i-1]) / u[i-1] );
        }

        z_.assign(spline_.size(), 0.0);
        z_[spline_.size()-1] = 0.0;
        for( size_t i = spline_.size() - 2; i >= 1; --i )
            z_[i] = (v[i] - h_[i] * z_[i+1]) / u[i];

        z_[0] = 0.0;
    }

    void buildEndPoints() {
        if( isClosed_ ) {
            buildClosedEndpoints();
        }
        else {
            waypoints_.insert(waypoints_.begin(), WaypointType());
            waypoints_.push_back(WaypointType());
            waypoints_.front() = 2.f*waypoints_[1] - waypoints_[2];
            size_t pSize = waypoints_.size();
            waypoints_.back() = 2.f*waypoints_[pSize-2] - waypoints_[pSize-3];
        }
    }

    void buildClosedEndpoints() {
        waypoints_.insert(waypoints_.begin(), WaypointType());
        waypoints_.push_back(WaypointType());
        size_t pSize = waypoints_.size();
        WaypointType s1 = waypoints_[2] - waypoints_[1];
        WaypointType s2 = waypoints_[pSize-3] - waypoints_[1];
        float const& d1 = distance(waypoints_[2], waypoints_[1]);
        float const& d2 = distance(waypoints_[pSize-3], waypoints_[1]);
        s1 /= d1;
        s2 /= d2;
        waypoints_.front() = waypoints_[1] + d1*s2;
        waypoints_.back()  = waypoints_[1] + d2*s1;
    }

protected:
    Waypoints waypoints_;
    size_t    spline_index_;
    bool      isClosed_;

    std::vector< float >                          range_list_;
    std::vector< CubicCoefficient<WaypointType> > coefs_;
    std::vector< core::vector2df >                spline_; //spline for cubic interporlating
    std::vector< double >                         h_;
    std::vector< double >                         hInv_;
    std::vector< double >                         z_;
};

} // scene
} // irr

#endif
