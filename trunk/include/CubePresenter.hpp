
#ifndef _SHOOTING_CUBE_CUBE_PRESENTER_HPP_
#define _SHOOTING_CUBE_CUBE_PRESENTER_HPP_

typedef MapModel* pMapModel;
typedef CubeModel* pCubeModel;
typedef MapSetting* pMapSetting;
...

template <class MultiArray, class Fun>
MultiArray& for_each(MultiArray& array, Fun fun){
    typename MultiArray::element* data = array.data();
    for(int i=0, iend=array.num_elements(); i!=iend; ++i)
        fun(data[i], i);
    return array;
}

namespace psc{
    namespace model{
        class Map;
        class Cube;
    }
    namespace presenter{
        class Map;
        class Cube;
    }
    namespace view{
        class Map;
        class Cube;
    }
}

namespace psc{ namespace model{
    class Map{
    public:
        typedef multi_array<pCube, 2> container_type;
        static pMap create(pMapSetting setting){
            // map doesn't need a pool
            return pMap(new Map(setting));
        }
        Map(pMapSetting setting): setting_(setting),
         cubes_(extents[setting.width][setting.height])
        {
            init_cubes();
        }
        pMap cycle(){
            n_of_newcomers_ = 0;
            Cube* cubes = cubes_.origin();
            for(int i=--ms().size(), iend=-1; i!=iend; --i){
                if(Cube* s = cubes[i]){
                    if( !s->has_grounded() ) ++n_of_newcomers_;
                    if( s->cycle_and_die() ){
                        cubes[i] = NULL;
                    }
                }
            }
        	// n_of_newcomers_ = 0;

            // for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
            //     i!=iend; ++i)
            // {
            //     if(Square* s = *i){
            //       if( !s->has_grounded() ) ++n_of_newcomers_;
            //       if( s->cycle_and_die() ){
            //         *i = NULL;
            //       }
            //     }
            // }
        }
        pMapSetting& ms() const{ return *setting_; }
    private:
        Map& init_cubes(){
            typedef multi_array<int, 2> vector_2d;
            using boost::lambda::_1;
            using boost::lambda::_2;
    		vector_2d square_colors(ms().width(), ms().starting_line());

            int const one_color_amounts = std::ceil(static_cast<double>(ms().width())*ms().starting_line()/ms().color_amounts());
            for_each(square_colors, _1 = _2/one_color_amounts);

            while(!is_gooood(square_colors, ms().chain_amounts()));
            for(int i=0, iend=square_colors.num_elements(); i!=iend; ++i)
            {
                insert(Cube::create(this, i%ms().width(), ms().height()-1-i/ms().width(), squares_colors[i]));
            }
            return *this;
        }
        Map& insert(pCube cube){
            cubes_[x][y] = cube;
        }

    private:
        pMapSetting setting_;
        container_type cubes_;
    };

    class Cube{<% @prefix = ' '*4 %>
    <%= accessor :int, :x, :y %>
    public:
        static pCube create(pMap m, int x, int y, int color_id){
            // it shouldn't be here... just for demo we have a pool.
            static boost::object_pool<Cube> pool;
            return pCube(pool.allocate()->init(m, x, y, color_id));
        }
        Cube(){}
        Cube* init(pMap m, int x, int y, int color_id){
            color_ = Color::from_id(color_id);
            return this;
        }
    private:
        pColor color_;
    };
}}

namespace psc{ namespace presenter{
    class Map{
    public:
        typedef std::list<pCube> CubeList;
    public:
        Map(pMapSetting setting):
            setting_(setting),
            model_(model::Map::create(this, setting)),
            view_(view::Map::create(this))
        {}
    public:
        Map& operator<<(model::pCube mCube){
            cubes_.push_back(Cube::create(mCube));
        }

    private:
        pMapSetting setting_;
        model::pMap model_;
        view::pMap view_;
        CubeList cubes_;
    };

    class Cube{
    public:
        Cube(pMap map, int x, int y, int color_id):
            map_(map),
            model_(model::Cube::create(map->model(), x, y, color_id)),
            view_(view::Cube::create(map->view()), x, y, color_id)
        {}
    private:
        pMap map_;
        model::pCube model_;
        view::pCube view_;
    };
}}

#endif
