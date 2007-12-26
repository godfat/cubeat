
#ifndef _SHOOTING_CUBE_CUBE_PRESENTER_HPP_
#define _SHOOTING_CUBE_CUBE_PRESENTER_HPP_

typedef MapModel* pMapModel;
typedef CubeModel* pCubeModel;
...

class MapModel{
public:
    static pMapModel create(){
        // map doesn't need a pool
        return pMapModel(new MapModel);
    }
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
    MapPresenter():
        view_(MapView::create()),
        model_(MapModel::create())
    {}
private:
    MapView* view_;
    MapModel* model_;
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
