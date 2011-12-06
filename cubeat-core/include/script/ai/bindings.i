
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

typedef struct {
    enum { PSC_AI_NONE = 0, PSC_AI_SHOOT, PSC_AI_HASTE };
    int x, y;
    int delay;
    unsigned int type; //enum
} LuaAICommand;

APIEXPORT void        AIBrain_push_command(AIBrain*, LuaAICommand*); //not shared_ptr!
APIEXPORT pSimpleMap* AIBrain_get_ally_map(AIBrain*, unsigned int);  //not shared_ptr!
APIEXPORT pSimpleMap* AIBrain_get_enemy_map(AIBrain*, unsigned int); //not shared_ptr!

APIEXPORT void SimpleMap_print_data_for_debug(pSimpleMap*);
APIEXPORT int  SimpleMap_warning_level(pSimpleMap*);
APIEXPORT int  SimpleMap_garbage_left(pSimpleMap*);
APIEXPORT int  SimpleMap_width(pSimpleMap*);
APIEXPORT int  SimpleMap_height(pSimpleMap*);
APIEXPORT void SimpleMap__gc(pSimpleMap*);
APIEXPORT bool SimpleMap_cube_exist_at(pSimpleMap*, int, int);
APIEXPORT int  SimpleMap_grounded_cube_count(pSimpleMap*);

APIEXPORT pSimpleCube*  SimpleMap_get_cube(pSimpleMap*, int, int);
APIEXPORT pSimpleCube*  SimpleMap_get_grounded_cube(pSimpleMap*, int, int);
APIEXPORT pSimpleCube*  SimpleMap_get_firepoint_cube(pSimpleMap*, int, int);
APIEXPORT pSimpleCube** SimpleMap_get_garbages(pSimpleMap*, unsigned int* size_out);
APIEXPORT pSimpleCube** SimpleMap_get_brokens(pSimpleMap*, unsigned int* size_out);

APIEXPORT bool SimpleCube_exist(pSimpleCube*);
APIEXPORT bool SimpleCube_is_garbage(pSimpleCube*);
APIEXPORT bool SimpleCube_is_broken(pSimpleCube*);
APIEXPORT int  SimpleCube_x(pSimpleCube*);
APIEXPORT int  SimpleCube_y(pSimpleCube*);
APIEXPORT void SimpleCube__gc(pSimpleCube*);

APIEXPORT void SimpleCubeList__gc(pSimpleCube** list, int size);
