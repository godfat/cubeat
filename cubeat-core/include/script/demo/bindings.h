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
APIEXPORT void    Demo_init_tutorial(Demo*, char const*, char const*, char const*); // not shared_ptr!
APIEXPORT void    Demo_eventual_pause(Demo*); // not shared_ptr!
APIEXPORT void    Demo_eventual_resume(Demo*); // not shared_ptr!
APIEXPORT void    Demo_reinit(Demo*); // not shared_ptr!
APIEXPORT void    Demo_endgame(Demo*, int); // not shared_ptr!
APIEXPORT void    Demo_quit(Demo*); // not shared_ptr!

#endif // _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_
