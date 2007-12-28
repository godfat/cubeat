
#ifndef _SHOOTING_CUBE_CUBE_PRESENTER_HPP_
#define _SHOOTING_CUBE_CUBE_PRESENTER_HPP_

typedef MapModel* pMapModel;
typedef CubeModel* pCubeModel;
typedef MapSetting* pMapSetting;
...

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
        typedef multi_array<2, pCube> container_type;
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
        Map(pMapSetting setting):
            setting_(setting),
            model_(model::Map::create(this, setting)),
            view_(view::Map::create(this))
        {
            
        }
    private:
        pMapSetting setting_;
        model::pMap model_;
        view::pMap view_;
        Cubes cubes_;
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
