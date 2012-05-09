#ifndef _SHOOTING_CUBES_SCRIPT_UI_PUZZLE_BINDINGS_H_
#define _SHOOTING_CUBES_SCRIPT_UI_PUZZLE_BINDINGS_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

APIEXPORT pScene* get_puzzle_scene(Puzzle*);

#endif // _SHOOTING_CUBES_SCRIPT_UI_PUZZLE_BINDINGS_H_
