
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

APIEXPORT void on_connected();
APIEXPORT void on_matched();
APIEXPORT void on_disconnected();
APIEXPORT int poll_from_C();
APIEXPORT bool check_quit();
