#ifndef _SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_
#define _SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

APIEXPORT pScene*  TestUI_get_ui_scene(TestUI*);

APIEXPORT pSprite* Sprite_create(char const*, pScene*, int, int, bool);

APIEXPORT void     Sprite_set_pos(pSprite*, double, double);

APIEXPORT void     Scene__gc(pScene*);
APIEXPORT void     Sprite__gc(pSprite*);

#endif //_SHOOTING_CUBES_SCRIPT_UI_TEST_BINDINGS_

