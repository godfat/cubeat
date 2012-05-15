#ifndef _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_
#define _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

APIEXPORT pScene* Demo_get_ui_scene(Demo*);

#endif // _SHOOTING_CUBES_SCRIPT_UI_DEMO_BINDINGS_H_
