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
APIEXPORT void    Demo_init_story(Demo*, char const*, char const*, char const*, int); // not shared_ptr!
APIEXPORT void    Demo_init_tutorial(Demo*, char const*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_init_cpudemo(Demo*, char const*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_init_ai_logging(Demo*, char const*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_init_single(Demo*, int, int, char const*, char const*, bool); // not shared_ptr!
APIEXPORT void    Demo_init_map_starting_line(Demo*, int map_id, int n); // not shared_ptr!
APIEXPORT void    Demo_init_map_with_config(Demo*, int map_id, char const*); // not shared_ptr!
APIEXPORT void    Demo_set_map_garbage_amount(Demo*, int map_id, int n); // not shared_ptr!
APIEXPORT void    Demo_set_map_dropping(Demo*, int map_id, bool flag); // not shared_ptr!
APIEXPORT void    Demo_set_only_one_shot_for_puzzle(Demo*); // not shared_ptr!
APIEXPORT void    Demo_set_stage_name(Demo*, char const*); // not shared_ptr!
APIEXPORT void    Demo_set_countdown(Demo*, bool); // not shared_ptr!
APIEXPORT void    Demo_set_time(Demo*, int); // not shared_ptr!

APIEXPORT int     Demo_get_time(Demo*); // not shared_ptr!
APIEXPORT int     Demo_get_map_score(Demo*, int map_id); // not shared_ptr!
APIEXPORT int     Demo_get_map_highest_chain(Demo*, int map_id); // not shared_ptr!
APIEXPORT int     Demo_get_map_garbage_left(Demo*, int map_id); // not shared_ptr!
APIEXPORT int     Demo_get_map_warning_level(Demo*, int map_id); // not shared_ptr!
APIEXPORT int     Demo_get_map_broken_num(Demo*, int map_id); // not shared_ptr!
APIEXPORT int const* Demo_get_map_cubes_cleared_data(Demo*, int map_id); // not shared_ptr!
APIEXPORT int const* Demo_get_map_cubes_matched_data(Demo*, int map_id); // not shared_ptr!
APIEXPORT AIPlayer*  Demo_get_ai_player(Demo*); // not shared_ptr!
APIEXPORT bool    Demo_is_map_all_waiting(Demo*, int map_id); // not shared_ptr!
APIEXPORT bool    Demo_is_map_empty(Demo*, int map_id); // not shared_ptr!
APIEXPORT bool    Demo_is_puzzle_started(Demo*); // not shared_ptr!

APIEXPORT void    Demo_eventual_pause(Demo*); // not shared_ptr!
APIEXPORT void    Demo_eventual_resume(Demo*); // not shared_ptr!
APIEXPORT void    Demo_reinit(Demo*); // not shared_ptr!
APIEXPORT void    Demo_endgame(Demo*, int); // not shared_ptr!
APIEXPORT void    Demo_leave_and_cleanup(Demo*); // not shared_ptr!
APIEXPORT void    Demo_quit(Demo*); // not shared_ptr!

#endif // _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_
