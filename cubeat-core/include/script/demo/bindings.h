#ifndef _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_
#define _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

APIEXPORT pScene* Demo_get_ui_scene(Demo*); // not shared_ptr!
APIEXPORT void    Demo_init_vs_ppl(Demo*, char const*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_init_vs_cpu(Demo*, char const*, char const*, char const*, int); // not shared_ptr!
APIEXPORT void    Demo_init_cpudemo(Demo*, char const*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_init_ai_logging(Demo*, char const*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_init_puzzle(Demo*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_init_tutorial(Demo*, char const*, char const*, char const*, bool, int); // not shared_ptr!
APIEXPORT void    Demo_init_map_starting_line(Demo*, int map_id, int n);
APIEXPORT void    Demo_set_map_garbage_amount(Demo*, int map_id, int n);

APIEXPORT int     Demo_get_time(Demo*); // not shared_ptr!
APIEXPORT int     Demo_get_map_score(Demo*, int map_id); // not shared_ptr!
APIEXPORT int     Demo_get_map_highest_chain(Demo*, int map_id); // not shared_ptr!
APIEXPORT int     Demo_get_map_garbage_left(Demo*, int map_id); // not shared_ptr!
APIEXPORT int     Demo_get_map_warning_level(Demo*, int map_id); // not shared_ptr!
APIEXPORT int const* Demo_get_map_cubes_cleared_data(Demo*, int map_id); // not shared_ptr!

APIEXPORT void    Demo_eventual_pause(Demo*); // not shared_ptr!
APIEXPORT void    Demo_eventual_resume(Demo*); // not shared_ptr!
APIEXPORT void    Demo_reinit(Demo*); // not shared_ptr!
APIEXPORT void    Demo_endgame(Demo*, int); // not shared_ptr!
APIEXPORT void    Demo_quit(Demo*); // not shared_ptr!

#endif // _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_
