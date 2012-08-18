
#ifndef _SHOOTING_CUBES_CONFIGURE_CONSTANTS_
#define _SHOOTING_CUBES_CONFIGURE_CONSTANTS_

/* a program-wide config class */

#include "utils/dictionary.hpp"
#include <string>

namespace psc {

struct Conf
{
private:
    utils::map_any config_;
    std::string working_path_;
    std::string config_path_;
    std::string script_path_;
    std::string version_string_;

    int screen_width_;
    int screen_height_;
    int framerate_;

    Conf();

public:
    static Conf& i() {
        static Conf singleton;
        return singleton;
    }
    //note: call this with null string to specify path relative to executable no matter what,
    //      or call this with a absolute path to your working path.
    void init(std::string const& working_path);

    //note: none of these will work if you didn't call init with the correct path.
    //this will return a path prefixed with your initial working path
    inline std::string expand(std::string const& path) const {
    #if defined(_WIN32) || defined(WIN32)
        return path; //do nothing on win32
    #else
        return working_path_ + path;
    #endif
    }
    inline std::string const& WORKING_PATH() const { return working_path_; }
    inline std::string script_path(std::string const& path) const { return script_path_ + path; }

    utils::map_any config_of(std::string const& name) const;
    std::string read_config_text(std::string const& path) const;
    void save_config(utils::map_any const& c, std::string const& name) const;

    inline int SCREEN_W() const { return screen_width_; }
    inline int SCREEN_H() const { return screen_height_; }
    inline int FRAMERATE() const { return framerate_; }

    std::string const& VERSION() const { return version_string_; }
};

}

#endif // _SHOOTING_CUBES_CONFIGURE_CONSTANTS_
