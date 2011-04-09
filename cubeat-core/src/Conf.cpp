
#include "Conf.hpp"
#include "IrrDevice.hpp"
#include <utility>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;
using namespace psc;

void Conf::init(std::string const& working_path)
{
    if( !working_path.empty() ) {
        working_path_ = working_path[working_path.size()-1] == '/' ?
                        working_path : working_path + "/"; //This must be done ahead of everything
    }

    config_ = utils::map_any::construct( read_config_text( working_path_ + "config.zzml") );

    config_path_           = expand(config_.S("config_path"));
    screen_width_          = config_.I("screenwidth");
    screen_height_         = config_.I("screenheight");
    framerate_             = config_.I("framerate");
}

utils::map_any Conf::config_of(std::string const& name) const
{
    std::string fname = name.substr(name.size()-5, 5) == std::string(".zzml") ?
                        name : name + ".zzml";
    return utils::map_any::construct( read_config_text( config_path_ + fname ) );
}

std::string Conf::read_config_text(std::string const& path) const
{
    std::ifstream infile;
    std::string str;
    std::string actual_path = path;
    infile.open( actual_path.c_str() );
    if( infile.fail() || infile.eof() ) {  //non-existing file exception
        std::cout << "No such file: " << actual_path << ", Input setup ignored." << std::endl;
        return str;              //empty string
    }

    std::string in;
    while( getline(infile, in) ) {
        trim(in);
        if( in[0] != '#' )
            str += in;
    }

    infile.close();
    //std::cout << str << std::endl;
    std::cout << "config: " << actual_path << " loaded." << std::endl;

    return str;
}
