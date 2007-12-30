
#ifndef _SHOOTING_CUBE_CUBE_PRESENTER_HPP_
#define _SHOOTING_CUBE_CUBE_PRESENTER_HPP_

typedef MapModel* pMapModel;
typedef CubeModel* pCubeModel;
typedef MapSetting* pMapSetting;
...

template <class MultiArray, class Fun>
MultiArray& for_each(MultiArray& array, Fun fun){
    MultiArray::element_type* data = array.origin();
    for(int i=0, iend=array.size(); i!=iend; ++i)
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
        MapModel(pMapSetting setting): setting_(setting),
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
            using std::tr1::_1;
            using std::tr1::_2;
    		vector_2d square_colors(ms().width(), ms().starting_line());

            int const one_color_amounts = std::ceil(static_cast<double>(ms().width())*ms().starting_line()/ms().color_amounts());
            for_each(square_colors, _1 = _2/one_color_amounts);

            // while(!is_gooood(square_colors, ms().chain_amounts));
            // {
            //     int count = 0;
            //     for(vector_2d<int>::iterator i=square_colors.begin(), iend=square_colors.end();
            //         i!=iend; ++i, ++count)
            //     {
            //         insert(SF::Instance().Create<Square>(), count%ms().width, ms().height-1-count/ms().width, *i);
            //     }
            // }
            return *this;
        }

    private:
        pMapSetting setting_;
        container_type cubes_;
    };

    class Cube{
    public:
        static pCube create(pMap p, int x, int y, int color_id){
            // it shouldn't be here... just for demo we have a pool.
            static boost::object_pool<Cube> pool;
            return pCube(pool.allocate()->init(p, x, y, color_id));
        }
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
