#ifndef _SHOOTING_CUBES_SCRIPT_AI_BINDINGS_
#define _SHOOTING_CUBES_SCRIPT_AI_BINDINGS_

#include "script/basics.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

typedef struct {
    enum { AI_NONE = 0, AI_SHOOT, AI_SHOOT_OTHER, AI_HASTE, AI_USE_ABILITY };
    int x, y;
    int delay;
    unsigned int type; //enum
} LuaAICommand;

APIEXPORT void        AIPlayer_push_command(AIPlayer*, LuaAICommand*); //not shared_ptr!
APIEXPORT void        AIPlayer_set_interval(AIPlayer*, unsigned int); //not shared_ptr!
APIEXPORT void        AIPlayer_set_missrate(AIPlayer*, unsigned int); //not shared_ptr!
APIEXPORT void        AIPlayer_start_thinking(AIPlayer* p); //not shared_ptr!
APIEXPORT void        AIPlayer_stop_thinking(AIPlayer* p); //not shared_ptr!
APIEXPORT pSimpleMap* AIPlayer_get_ally_map(AIPlayer*, unsigned int);  //not shared_ptr!
APIEXPORT pSimpleMap* AIPlayer_get_enemy_map(AIPlayer*, unsigned int); //not shared_ptr!
APIEXPORT int         AIPlayer_cmdqueue_size(AIPlayer*); //not shared_ptr!
APIEXPORT double      AIPlayer_get_heat(AIPlayer*);      //not shared_ptr!
APIEXPORT int         AIPlayer_ability_kind(AIPlayer*);  //not shared_ptr!
APIEXPORT unsigned int AIPlayer_ability_left(AIPlayer*); //not shared_ptr!

APIEXPORT void SimpleMap_print_data_for_debug(pSimpleMap*);
APIEXPORT int  SimpleMap_warning_level(pSimpleMap*);
APIEXPORT int  SimpleMap_garbage_left(pSimpleMap*);
APIEXPORT int  SimpleMap_width(pSimpleMap*);
APIEXPORT int  SimpleMap_height(pSimpleMap*);
APIEXPORT int  SimpleMap_id(pSimpleMap*);
APIEXPORT void SimpleMap__gc(pSimpleMap*);
APIEXPORT bool SimpleMap_cube_exist_at(pSimpleMap*, int, int);
APIEXPORT int  SimpleMap_grounded_cube_count(pSimpleMap*);
APIEXPORT bool SimpleMap_still_chaining(pSimpleMap*);
APIEXPORT bool SimpleMap_dropping_locked(pSimpleMap*);

APIEXPORT pSimpleCube*  SimpleMap_get_cube(pSimpleMap*, int, int);
APIEXPORT pSimpleCube*  SimpleMap_get_grounded_cube(pSimpleMap*, int, int);
APIEXPORT pSimpleCube*  SimpleMap_get_firepoint_cube(pSimpleMap*, int, int, int, int* power_out);
APIEXPORT void          SimpleMap_get_firemap(pSimpleMap*, int* firemap_out);
APIEXPORT int           SimpleMap_get_base_attack(pSimpleMap*);
APIEXPORT pSimpleCube** SimpleMap_get_garbages(pSimpleMap*, unsigned int* size_out);
APIEXPORT pSimpleCube** SimpleMap_get_brokens(pSimpleMap*, unsigned int* size_out);

APIEXPORT bool SimpleCube_exist(pSimpleCube*);
APIEXPORT bool SimpleCube_is_garbage(pSimpleCube*);
APIEXPORT bool SimpleCube_is_broken(pSimpleCube*);
APIEXPORT int  SimpleCube_x(pSimpleCube*);
APIEXPORT int  SimpleCube_y(pSimpleCube*);
APIEXPORT void SimpleCube__gc(pSimpleCube*);

APIEXPORT void SimpleCubeList__gc(pSimpleCube** list, int size);

#endif //_SHOOTING_CUBES_SCRIPT_AI_BINDINGS_
