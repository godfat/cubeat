
#ifndef _SHOOTING_CUBE_CUBE_PRESENTER_HPP_
#define _SHOOTING_CUBE_CUBE_PRESENTER_HPP_

typedef MapModel* pMapModel;
typedef CubeModel* pCubeModel;
typedef MapSetting* pMapSetting;
...

1. map presenter is created.
2. map model is inited.
3. map view is inited.

4. cube presenter list is created, passing map presenter to it.
5. cube model is inited, getting map model.
6. map model gets cube model.

7. cube view is inited, getting map view.
8. map view gets cube view.

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
