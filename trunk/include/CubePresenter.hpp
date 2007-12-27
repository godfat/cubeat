
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

class MapModel{
public:
    typedef multi_array<2, pCubeModel> container_type;
    static pMapModel create(pMapSetting setting){
        // map doesn't need a pool
        return pMapModel(new MapModel(setting));
    }
    MapModel(pMapSetting setting): setting_(setting),
     cubes_(extents[setting.width][setting.height])
    {
    }
    pMapModel cycle(){
        BOOST_FOREACH(container_type::iterator row, cubes_){
            BOOST_FOREACH(pCubeModel cube, row){
                
            }
        }
    	// n_of_newcomers_ = 0;

        // for(container_type::reverse_iterator i=data_.rbegin(), iend=data_.rend();
        //     i!=iend; ++i)
        // {
        //     if(Square* s = *i){
        //      if( !s->has_grounded() ) ++n_of_newcomers_;
        //         if( s->cycle_and_die() ){
        //             *i = NULL;
        //         }
        //     }
        // }
    }
private:
    pMapSetting setting_;
    container_type cubes_;
};

class CubeModel{
public:
    static pCubeModel create(pMapModel p, int x, int y, int color_id){
        // it shouldn't be here... just for demo we have a pool.
        static boost::object_pool<CubeModel> pool;
        return pCubeModel(pool.allocate()->init(p, x, y, color_id));
    }
};

class MapPresenter{
public:
    MapPresenter(pMapSetting setting):
        setting_(setting),
        view_(MapView::create()),
        model_(MapModel::create(setting))
    {}
private:
    pMapSetting setting_;
    pMapView view_;
    pMapModel model_;
};

class CubePresenter{
public:
    CubePresenter(MapPresenter* map, int x, int y, int color_id):
        map_(map),
        view_(CubeView::create(map->view())),
        model_(CubeModel::create(map->model(), x, y, color_id))
    {}
private:
    MapPresenter* map_;
    CubeView* view_;
    CubeModel* model_;
};

#endif
