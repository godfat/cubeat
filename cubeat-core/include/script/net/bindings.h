
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define APIEXPORT __declspec(dllexport)
#else
#define APIEXPORT
#endif

APIEXPORT void on_connected(char const*);
APIEXPORT void on_matched(char const*);
APIEXPORT void on_disconnected(char const*);
APIEXPORT void on_received(char const*);
APIEXPORT char const* poll_from_C();
APIEXPORT bool check_quit();
