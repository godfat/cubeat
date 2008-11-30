#ifndef _SHOOTING_CUBES_DATA_ANIMATOR_PARAM_
#define _SHOOTING_CUBES_DATA_ANIMATOR_PARAM_

#include "data/BasicViewTypes.hpp"
#include <tr1/functional>
#include <vector>

namespace psc {
namespace data {

template <template <class> class Eq, class Acc>
struct AnimatorParam {
    typedef typename Acc::value_type T;

    AnimatorParam():
        start_(T()), end_(T()), speedfunc_(0), is_start_specified_(false),
        is_speedfunc_specified_(false), duration_(1000), loop_(0), cb_(0), delay_(0) {}

    AnimatorParam& start(T const& i) { start_ = i; is_start_specified_ = true; return *this; }
    AnimatorParam& end(T const& i) { end_ = i; return *this; }
    AnimatorParam& duration(unsigned int const& i) { duration_ = i; return *this; }
    AnimatorParam& loop(int const& i) { loop_ = i; return *this; }
    AnimatorParam& cb(std::tr1::function<void()> const& i) { cb_ = i; return *this; }
    AnimatorParam& delay(int const& i) { delay_ = i; return *this; }
    AnimatorParam& speedfunc(std::tr1::function<float()> const& i) {
        speedfunc_ = i; is_speedfunc_specified_ = true; return *this;
    }

    T                           const& start()    const { return start_; }
    T                           const& end()      const { return end_; }
    unsigned int                const& duration() const { return duration_; }
    int                         const& loop()     const { return loop_; }
    std::tr1::function<void()>  const& cb()       const { return cb_; }
    int                         const& delay()    const { return delay_; }
    std::tr1::function<float()> const& speedfunc()const { return speedfunc_; }

    bool const& startValid()     const { return is_start_specified_; }
    bool const& speedFuncValid() const { return is_speedfunc_specified_; }

private:
    T                           start_;
    T                           end_;
    std::tr1::function<float()> speedfunc_;
    bool                        is_start_specified_;
    bool                        is_speedfunc_specified_;

protected:
    unsigned int                duration_;
    int                         loop_;
    std::tr1::function<void()>  cb_;
    int                         delay_;
};

template <template <class> class Eq, class Acc>
struct WaypointParam : public AnimatorParam<Eq, Acc> {
    typedef typename Acc::value_type T;

    WaypointParam():
        waypoints_(std::vector<T>(2,T())), closed_(false) {}

    WaypointParam& waypoints(std::vector<T>const& i) { waypoints_ = i; return *this; }
    WaypointParam& closed(bool const& i) { closed_ = i; return *this; }
    WaypointParam& tensions(std::vector<float>const& i) { tensions_ = i; return *this; }

    std::vector<T>     const& waypoints() const { return waypoints_; }
    std::vector<float> const& tensions() const  { return tensions_; }
    bool               const& closed() const    { return closed_; }

private:
    std::vector<T>     waypoints_;
    std::vector<float> tensions_;
    bool               closed_;
};

template <template <class> class Eq, class Acc>
struct CirclingParam : public AnimatorParam<Eq, Acc> {
    typedef typename Acc::value_type T;

    CirclingParam():
        center_(T()), start_(vec2()), end_(vec2()), rotation_(vec3()) {}

    CirclingParam& start(vec2 const& i) { start_ = i; return *this; }
    CirclingParam& end(vec2 const& i) { end_ = i; return *this; }
    CirclingParam& center(T const& i) { center_ = i; return *this; }
    CirclingParam& rotation(vec3 const& i) { rotation_ = i; return *this; }

    vec2 const& start()   const { return start_; }
    vec2 const& end()     const { return end_; }
    T    const& center()  const { return center_; }
    vec3 const& rotation()const { return rotation_; }

private:
    T    center_;
    vec2 start_;
    vec2 end_;
    vec3 rotation_;
};

} //data
} //psc

#endif // _SHOOTING_CUBES_DATA_ANIMATOR_PARAM_
