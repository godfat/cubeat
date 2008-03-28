
#ifndef _SHOOTING_CUBES_WAYPOINT_ANIMATOR_
#define _SHOOTING_CUBES_WAYPOINT_ANIMATOR_

#include "Accessor_proto.hpp"
#include "view/detail/CustomAnimator.hpp"

//for debug & warning purpose
#include <iostream>

#include <vector>

namespace irr{

namespace scene{

template <template <class> class Eq, class Acc>
class WaypointAnimator : public CustomAnimator<Eq, psc::accessor::Accessor<float, Acc::TYPE> >
{
    typedef CustomAnimator
        <Eq, psc::accessor::Accessor
            <float, Acc::TYPE> >                    WaypointAnimatorBase;
    typedef typename WaypointAnimatorBase::T        Primitive;
    typedef typename Acc::value_type                WaypointType;
    typedef std::vector< typename Acc::value_type > Waypoints;

public:
    WaypointAnimator(ISceneManager* smgr, Waypoints const& points, u32 const& duration,
                    int const& loop = 0, std::tr1::function<void()>const& cb = 0,
                    s32 const& delayTime = 0, bool const& isClosed = false)
        :WaypointAnimatorBase(smgr, 0, 0, duration, loop, cb, delayTime),
         waypoints_(points), now_waypoint_index_(waypoints_.size()/2)
    {
        if( waypoints_.size() < 2 ) {
            std::cout << "WaypointAnimator: no point to go! (you at least need 2 points!)\n";
            return;
        }
        if( isClosed && waypoints_.front() != waypoints_.back() )
            waypoints_.push_back( waypoints_.front() );

        calculateDistances();
    }

    virtual ~WaypointAnimator(){}

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
    void calculateDistances() {
        float full_length = 0;
        range_list_.push_back(0);
        for( size_t i = 0; i < waypoints_.size() - 1; ++i ) {
            full_length += this->distance(waypoints_[i+1], waypoints_[i]);
            range_list_.push_back( full_length );
        }
        this->end_ = this->distance_ = full_length;
    }

    inline void calculateCurrentWaypoint(ISceneNode* node, float const& pos) {
        size_t upperb = waypoints_.size() - 1;
        size_t lowerb = 0;
        if( pos <= this->start_ )    now_waypoint_index_ = 0;
        else if( pos >= this->end_ ) now_waypoint_index_ = waypoints_.size() - 2;
        else {
            bool too_small = pos >  range_list_[now_waypoint_index_ + 1],
                 too_big   = pos <= range_list_[now_waypoint_index_];
            if( too_small ) {
                ++now_waypoint_index_;
                lowerb = now_waypoint_index_;
                too_small = pos >  range_list_[now_waypoint_index_ + 1];
            }
            else if( too_big ) {
                --now_waypoint_index_;
                upperb = now_waypoint_index_;
                too_big   = pos <= range_list_[now_waypoint_index_];
            }
            while( too_small || too_big ) {
                if( too_small )    lowerb = now_waypoint_index_;
                else if( too_big ) upperb = now_waypoint_index_;
                now_waypoint_index_ = (upperb + lowerb)/2;
                if( lowerb == upperb ) break;
                too_small = pos >  range_list_[now_waypoint_index_ + 1];
                too_big   = pos <= range_list_[now_waypoint_index_];
            }
        }
        float const& temp           = pos - range_list_[now_waypoint_index_];
        float const& dist_to_next   = range_list_[now_waypoint_index_+1] - range_list_[now_waypoint_index_];
        WaypointType const& start   = waypoints_[now_waypoint_index_];
        WaypointType const& waytogo = waypoints_[now_waypoint_index_+1] - start;
        Acc::set(node, waytogo*temp/dist_to_next + start);
    }

protected:
    Waypoints    waypoints_;
    size_t       now_waypoint_index_;
    std::vector< float > range_list_;
};

} // scene
} // irr

#endif
