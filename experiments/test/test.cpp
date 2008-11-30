
namespace psc{
  enum Index{x, y, z};
  typedef boost::tuple<double, double, double> Position;
  // Position p;
  // get<x>(p) => x
  // get<y>(p) => y

  class CubePresenter{
  public:
    typedef CubePresenter this_type;

    typedef click_callback_type boost::function<void(Position, Player*)>;
    click_callback_type on_click;

    typedef ?? time_duration_type;

    typedef long_press_callback_type boost::function<void(Position, time_duration_type)>;
    long_press_callback_type on_long_press;

    this_type& add_click_listener(click_callback_type);
    this_type& add_long_press_listener(long_press_callback_type);

    CubePresenter(){
      on_click = function(Position pos, Player* pl){
        view_->go_exploding();
        model_->go_dying();
        pl->consume_bullet();
      }
    }

  private:
    CubeView* view_;
    CubeModel* model_;
  };

  template <class Imp>
  class CubeView{
  public:
    typedef CubeView<Imp> this_type;
    this_type& move(Position p){ imp_.move(p); }
  private:
    Loki::RimplOf<this_type>::Type imp_;
  };

  class CubeModel{
  private:
    State* state_;
  };

}

namespace Loki // gcc!!
{

  struct Irrlicht{};
  struct Ogre{};

  template<>
  class ImplOf< CubeView<Irrlicht> > : public SmallObject<> // inherit SmallObj for speed up
  {
    typedef ImplOf< CubeView<Irrlicht> > this_type;
    this_type& move(Position p){
      node_.move(get<x>(p), get<y>(p), get<z>(p));
      return *this;
    }
    private irr_scene_node* node_;
  };
}
