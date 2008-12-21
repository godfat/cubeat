
#include "utils/dictionary.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace boost::algorithm;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::any_cast;

namespace psc {
namespace utils {

//////////////////////// Helper & Common Func /////////////////////////

inline bool skip_it( char const& ch ) {
    if( ch == ' ' || ch == ',' || ch == ':' ) return true;
    return false;
}

inline bool is_number( char const& ch ) {
    if( all( std::string()+ch, is_digit() ) || ch == '-' ) return true;
    return false;
}

inline bool is_map( char const& ch ) {
    if( ch == '{' ) return true;
    return false;
}

inline bool is_vector( char const& ch ) {
    if( ch == '[' ) return true;
    return false;
}

inline bool is_string( char const& ch ) {
    if( all( std::string()+ch, is_alpha() ) || ch == '\'' ) return true;
    return false;
}

//Note: input index will be increased
inline std::string read_string_at( std::string const& str, int& i ) {
    int from = -1, to = -1;
    if( str[i] == '\'' ) {
        from = i+1;                        //skip the first quotation
        to = str.find_first_of('\'',from);
        i = to + 1;                        //skip the enclosing '\''
    }
    //if we find the first token is an alphabet, we assume there's no quotation.
    else if( all( str.substr(i, 1), is_alpha() ) ) {
        from = i;                          //there's no quotation to skip
        to = str.find_first_of(" ,:]}",from);
        i = to;                            //there's no quotation to skip
    }
    return str.substr( from, to-from );
}

template<class T> //Note: input index will be increased
inline T read_container_at( std::string const& str, int& i, char const& lb, char const& rb ) {
    int from = i+1;
    int to = from;
    int resolve_bracket = 1;
    while( resolve_bracket > 0 ) {
        if( str[to] == lb )      ++resolve_bracket;
        else if( str[to] == rb ) --resolve_bracket;
        ++to;
    }
    --to;
    i = to + 1;                        //skip the enclosing bracket
    return T::construct( str.substr( from, to-from ) );
}

template <class T> //Note: input index will be increased
inline T read_number_at( std::string const& str, int& i) {
    int from = i;
    T ret = 0;
    int to = str.find_first_of(" ,:]}",from);
    (to == -1) ? to = str.length() : to;
    std::string sub = str.substr( from, to-from );

    if ( (str[from] == '0' && str[from+1] == 'x') ||      //process hexdecimal
         (str[from+1] == '0' && str[from+2] == 'x') ) {
        std::istringstream in(sub);
        in >> std::hex >> ret;
    }
    else ret = lexical_cast<T>( sub );
    //will throw, make it throw before i is increased
    i = to; //basically skips nothing
    return ret;
}

////////////////// construct of map_any & vector_any //////////////////

map_any map_any::construct(std::string const& str)
{
    map_any map;
    int i = 0;
    while( i < (int)str.length() ) {

        if( skip_it( str[i] ) ) { ++i; continue; }

        any_type key;                         //we don't know what type the key is.

        if( is_string( str[i] ) ) {
            key = read_string_at(str, i);
        }
        //parse key: when key is a number
        else if( is_number( str[i] ) ) {
            key = read_number_at<int>(str, i);
        }
        else { /*parse error*/
            std::cerr << "An exception raised when parsing key at position "
                      << i << ", element: " << str[i] << "\n";
            break;
        }

        //skip anything we don't want between key:value
        while ( skip_it( str[i] ) )
            ++i;

        if( is_number( str[i] ) ) {
            try {
                map[ key ] = read_number_at<int>(str, i);
            }
            catch(bad_lexical_cast&) {
                map[ key ] = read_number_at<double>(str, i);
            }
        }
        else if( is_string( str[i] ) ) {
            map[ key ] = read_string_at(str, i);
        }
        else if( is_vector( str[i] ) ) {
            map[ key ] = read_container_at<vector_any>(str, i, '[',']');
        }
        else if( is_map( str[i] ) ) {
            map[ key ] = read_container_at<map_any>(str, i, '{','}');
        }
        else { /*parse error*/
            std::cerr << "An Exception Raised when parsing value at position "
                      << i << ", element: " << str[i] << "\n";
            break;
        }
    }
    return map;
}

vector_any vector_any::construct(std::string const& str)
{
    vector_any vec;
    int i = 0;
    while( i < (int)str.length() ) {

        if( skip_it( str[i] ) ) { ++i; continue; }

        if( is_number( str[i] ) ) {
            try {
                vec.push_back( read_number_at<int>(str, i) );
            }
            catch(bad_lexical_cast&) {
                vec.push_back( read_number_at<double>(str, i) );
            }
        }
        else if ( is_string( str[i] ) ) {
            vec.push_back( read_string_at(str, i) );
        }
        else if ( is_vector( str[i] ) ) {
            vec.push_back( read_container_at<vector_any>(str, i, '[',']') );
        }
        else if( is_map( str[i] ) ) {
            vec.push_back( read_container_at<map_any>(str, i, '{','}') );
        }
        else { /*parse error*/
            std::cerr << "An exception raised when parsing list. element: "
                      << str[i] << " pos: " << i << "\n";
            break;
        }
    }
    return vec;
}

////////////////////////// config reader ////////////////////////////

std::string fetchConfig(std::string const& path)
{
    std::ifstream infile;
    std::string str;
    infile.open( path.c_str() );
    if( infile.fail() || infile.eof() ) {  //non-existing file exception
        std::cout << "No such file: " << path << ", Input setup ignored." << std::endl;
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
    std::cout << "config: " << path << " loaded." << std::endl;

    return str;
}

///////////////////////// any hash and equal overloading /////////////////////

std::size_t hash_value(any_type const& value)
{
    using boost::any_cast;
    using boost::bad_any_cast;
    try {
        boost::hash<int> hasher;
        return hasher( any_cast<int>(value.imp) );
    }
    catch(bad_any_cast&) {
        try {
            boost::hash<std::string> hasher;
            return hasher( any_cast<std::string>(value.imp) );
        }
        catch(bad_any_cast&) {
            std::cerr << "bad_any_cast occurred.\n";
            return 0;
        }
    }
}

bool operator==(any_type const& a, any_type const& b)
{
    using boost::any_cast;
    using boost::bad_any_cast;
    if( a.type() == b.type() ) {
        if( a.type() == typeid(int) )
            return any_cast<int>(a.imp) == any_cast<int>(b.imp);
        else if( a.type() == typeid(std::string) )
            return any_cast<std::string>(a.imp) == any_cast<std::string>(b.imp);
        else return false;
    }
    return false;
}

} //utils
} //psc

