
#include "Conf.hpp"
#include "IrrDevice.hpp"
#include <utility>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;
using namespace psc;

Conf::Conf()
    :version_string_("CuBeat 0.5.2 alpha")
{}

void Conf::init(std::string const& working_path)
{
    if( !working_path.empty() ) {
        working_path_ = working_path[working_path.size()-1] == '/' ?
                        working_path : working_path + "/"; //This must be done ahead of everything
    }

    config_ = utils::map_any::construct( read_config_text( working_path_ + "rc/config.zzml") );

    config_path_           = expand("rc/config/");
    script_path_           = expand("rc/script/");
    shader_path_           = expand("rc/shader/");
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
        std::cout << "No such file: " << actual_path << ", setup ignored." << std::endl;
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

void Conf::save_config(utils::map_any const& c, std::string const& name) const
{
    std::string fname = name.substr(name.size()-5, 5) == std::string(".zzml") ?
                        name : name + ".zzml";
    std::string fullpath = config_path_ + fname;

    //std::fstream outfile;
    FILE *outfile;
    //outfile.open( fullpath.c_str(), std::fstream::in | std::fstream::out );
    outfile = fopen( fullpath.c_str(), "w" );
    if( !outfile ) {
        std::cout << "File: " << fullpath << " open failed." << std::endl;
        return;
    }

    //outfile << c.serialize();
    std::string str = c.serialize();
    std::string stripped = str.substr(2, str.size()-5);
    std::cout << stripped << std::endl;
    fprintf(outfile, "%s\n", stripped.c_str());
    //outfile.close();
    fclose(outfile);

    std::cout << "config: " << fullpath << " saved." << std::endl;
}
