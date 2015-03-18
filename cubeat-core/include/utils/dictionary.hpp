
#ifndef _SHOOTING_CUBES_UTILS_DICTIONARY_
#define _SHOOTING_CUBES_UTILS_DICTIONARY_

#include <boost/functional/hash.hpp>
#include <boost/any.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <vector>
#include <string>
//#include <utility>

//////////////////////////////////////////////////////////////////////////////

/* An Arbitary Value Type Dictionary Implementation by arch.jslin 2008.2.19

   * In A Nutshell:

     in python, ruby, or actionscript:

        map["name"][1]["phone"]; //in dynamic language this is easy

     now in C++:

        map.V("name").M(1).S("phone"); .... //no so easy, but usable.
        //"name" extracts a vector from map, 1 extracts a map from the vector,
          "phone" extracts a string from the sub-map.

   * key type only support signed int and std::string (but I think that's enough.)

     you can parse a text file containing only this expression into the
     container, by using:

         map  = map_any::construct( fetchConfig("path") );

     or, when the outermost container expression in the file is a list:

         list = vector_any::construct( fetchConfig("path") );

   * a correct string expression of current implementation (between quotation),

     "'p2': {'somelist': [-1.1, [14, 15, [3]], 6, 13], 1: '234', 'val': 5},
      'p1': {'pic': ['123','ab','c'], 'val': 10} "

     Above is just the same as a dictionary & list nested-expression of
     Python 2.5, however, when you are specifying strings, you can do it
     without quotations; and hexdecimal expressions are accepted as integer
     item.

     number:   1, -1, 1.0, -1.0, 0xfe, -0xfd.
     string:   'string', ThisIsAString_Too (don't use numeric letter in the front)
        map:   { ... }
     vector:   [ ... ]
     splitter: ,(comma) :(colon when mapping key to value in a map)

     BEWARE: the outermost [ ] and/or { }, which I didn't write in the above
     example, MUST BE omitted, just don't write them at all.

     BEWARE: this expression syntax has no tolerance at all, only thing
     that is tolerable are white spaces.

   * you can even make comments, using '#' at the first character in a line,
     as long as there's no other expression in the same line with comments.
     Because the commented line will be completely ignored.

   * a dictionary/ vector here can contain:
         sub-vector, float, int, string, sub-dictionary

   TODO:

   * added common container interfaces like clear(), size(), empty() etc,
     but not fully tested.

   * need a toString or toFile function. (this is hard if you want a pretty format)

*/

namespace psc {
namespace utils {

struct any_type : public boost::any
{
    any_type():boost::any(){} // NOTE 2015: SHOULD NOT DEFAULT boost::any to ZERO... the non-existent value
                              //            when trying to cast to any_cast<int> it will be silently ignored!

    template<typename ValueType>
    any_type(ValueType const& value)
      : boost::any(value){}

    any_type(any_type const& other)
      : boost::any( static_cast<boost::any const&>(const_cast<any_type&>(other)) ) {} //up-cast
};

/////////////////////// start of the dictionary definitions /////////////////////

typedef std::tr1::unordered_map< any_type, any_type, boost::hash<any_type> >
    map_any_;
typedef std::vector< any_type > //Can't use list here cuz we dependent on random access.
    vector_any_;
typedef std::pair<any_type const, any_type>
    pair_any;

class map_any; //forward

class vector_any
{
public:
    typedef vector_any_::iterator       iterator;
    typedef vector_any_::const_iterator const_iterator;
    static vector_any construct( std::string const& str );

public:
    inline int&         I(int const& i) { return boost::any_cast<int&>        (data_[i]); }
    inline double&      F(int const& i) { return boost::any_cast<double&>     (data_[i]); }
    inline std::string& S(int const& i) { return boost::any_cast<std::string&>(data_[i]); }
    inline vector_any&  V(int const& i) { return boost::any_cast<vector_any&> (data_[i]); }
    inline map_any&     M(int const& i) { return boost::any_cast<map_any&>    (data_[i]); }

    inline int const&         I(int const& i) const { return boost::any_cast<int const&>        (data_[i]); }
    inline double const&      F(int const& i) const { return boost::any_cast<double const&>     (data_[i]); }
    inline std::string const& S(int const& i) const { return boost::any_cast<std::string const&>(data_[i]); }
    inline vector_any const&  V(int const& i) const { return boost::any_cast<vector_any const&> (data_[i]); }
    inline map_any const&     M(int const& i) const { return boost::any_cast<map_any const&>    (data_[i]); }

    template<class T>
    inline T& at(int const& i) { return boost::any_cast<T&>(data_.at(i)); }
    template<class T>
    inline T const& at(int const& i) const { return boost::any_cast<T const&>(data_.at(i)); }

    inline void push_back(int         const& i) { data_.push_back(i); }
    inline void push_back(double      const& i) { data_.push_back(i); }
    inline void push_back(std::string const& i) { data_.push_back(i); }
    inline void push_back(char const* const& i) { data_.push_back(std::string(i)); }
    inline void push_back(vector_any  const& i) { data_.push_back(i); }
    inline void push_back(map_any     const& i) { data_.push_back(i); }
    template<class T>
    inline void push_back(T const& i) { data_.push_back(i); }

    inline any_type& operator[](int const& i) { return data_[i]; }

    //these common functions are not tested
    inline size_t size() const { return data_.size(); }
    inline bool empty()  const { return data_.empty(); }
    inline void pop_back() { data_.pop_back(); }
    inline void clear()    { data_.clear(); }

    inline iterator erase(iterator loc)           { return data_.erase(loc); }
    inline iterator erase(iterator s, iterator e) { return data_.erase(s,e); }
    inline iterator insert(iterator loc, any_type const& v)       { return data_.insert(loc,v); }
    inline void insert(iterator loc, size_t n, any_type const& v) { data_.insert(loc,n,v); }
    inline void insert(iterator loc, iterator s, iterator e)        { data_.insert(loc,s,e); }

    inline iterator begin()             { return data_.begin(); }
    inline const_iterator begin() const { return data_.begin(); }
    inline iterator end()             { return data_.end(); }
    inline const_iterator end() const { return data_.end(); }

    inline any_type& front()             { return data_.front(); }
    inline any_type const& front() const { return data_.front(); }
    inline any_type& back()              { return data_.back(); }
    inline any_type const& back() const  { return data_.back(); }

    std::string serialize(std::string const& indent = "") const;

protected:
    mutable vector_any_ data_; //bad, because there is no any_const_cast() const
};


class map_any
{
public:
    typedef map_any_::iterator       iterator;
    typedef map_any_::const_iterator const_iterator;
    static map_any construct( std::string const& str );

    void operator=(map_any const& rhs){
        dict_ = rhs.dict_;
    }

public:
    inline int&         I(int         const& i) { return boost::any_cast<int&>        (dict_[i]); }
    inline double&      F(int         const& i) { return boost::any_cast<double&>     (dict_[i]); }
    inline std::string& S(int         const& i) { return boost::any_cast<std::string&>(dict_[i]); }
    inline vector_any&  V(int         const& i) { return boost::any_cast<vector_any&> (dict_[i]); }
    inline map_any&     M(int         const& i) { return boost::any_cast<map_any&>    (dict_[i]); }
    inline int&         I(std::string const& i) { return boost::any_cast<int&>        (dict_[i]); }
    inline double&      F(std::string const& i) { return boost::any_cast<double&>     (dict_[i]); }
    inline std::string& S(std::string const& i) { return boost::any_cast<std::string&>(dict_[i]); }
    inline vector_any&  V(std::string const& i) { return boost::any_cast<vector_any&> (dict_[i]); }
    inline map_any&     M(std::string const& i) { return boost::any_cast<map_any&>    (dict_[i]); }

    inline int const &         I(int         const& i) const { return boost::any_cast<int const&>        (dict_[i]); }
    inline double const &      F(int         const& i) const { return boost::any_cast<double const&>     (dict_[i]); }
    inline std::string const & S(int         const& i) const { return boost::any_cast<std::string const&>(dict_[i]); }
    inline vector_any const &  V(int         const& i) const { return boost::any_cast<vector_any const&> (dict_[i]); }
    inline map_any const &     M(int         const& i) const { return boost::any_cast<map_any const&>    (dict_[i]); }
    inline int const &         I(std::string const& i) const { return boost::any_cast<int const&>        (dict_[i]); }
    inline double const &      F(std::string const& i) const { return boost::any_cast<double const&>     (dict_[i]); }
    inline std::string const & S(std::string const& i) const { return boost::any_cast<std::string const&>(dict_[i]); }
    inline vector_any const &  V(std::string const& i) const { return boost::any_cast<vector_any const&> (dict_[i]); }
    inline map_any const &     M(std::string const& i) const { return boost::any_cast<map_any const&>    (dict_[i]); }

    template<class T>
    inline T& at(int const& i) { return boost::any_cast<T&>(dict_[i]); }
    template<class T>
    inline T& at(std::string const& i) { return boost::any_cast<T&>(dict_[i]); }
    template<class T>
    inline T const& at(int const& i) const { return boost::any_cast<T const&>(dict_[i]); }
    template<class T>
    inline T const& at(std::string const& i) const { return boost::any_cast<T const&>(dict_[i]); }

    inline iterator       find(std::string const& k) { return dict_.find(k); }
    inline const_iterator find(std::string const& k) const { return dict_.find(k); }
    inline iterator       find(int         const& k) { return dict_.find(k); }
    inline const_iterator find(int         const& k) const { return dict_.find(k); }
    bool                  exist(std::string const& k) const;

    inline void insert(std::string const& k, int         const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(std::string const& k, double      const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(std::string const& k, std::string const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(std::string const& k, char const* const& v) { dict_.insert(std::make_pair(k,std::string(v))); }
    inline void insert(std::string const& k, vector_any  const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(std::string const& k, map_any     const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(int         const& k, int         const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(int         const& k, double      const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(int         const& k, std::string const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(int         const& k, char const* const& v) { dict_.insert(std::make_pair(k,std::string(v))); }
    inline void insert(int         const& k, vector_any  const& v) { dict_.insert(std::make_pair(k,v)); }
    inline void insert(int         const& k, map_any     const& v) { dict_.insert(std::make_pair(k,v)); }
    template<class T>
    inline void insert(std::string const& k, T const& v) { dict_.insert(std::make_pair(k,v)); }
    template<class T>
    inline void insert(int         const& k, T const& v) { dict_.insert(std::make_pair(k,v)); }

    inline any_type& operator[](int const& i)         { return dict_[i]; }
    inline any_type& operator[](std::string const& i) { return dict_[i]; }
    inline any_type& operator[](char const* const& i) { return dict_[std::string(i)]; }
    inline any_type& operator[](any_type const& i)  { return dict_[i]; }

    //these common functions are not tested
    inline size_t size() const { return dict_.size(); }
    inline bool empty()  const { return dict_.empty(); }
    inline void clear()        { dict_.clear(); }

    inline iterator begin()             { return dict_.begin(); }
    inline const_iterator begin() const { return dict_.begin(); }
    inline iterator end()             { return dict_.end(); }
    inline const_iterator end() const { return dict_.end(); }

    inline size_t count(any_type const& k)  { return dict_.count(k); }
    inline size_t erase(any_type const& k)  { return dict_.erase(k); }
    inline void erase(const_iterator loc)                 { dict_.erase(loc); }
    inline void erase(const_iterator s, const_iterator e) { dict_.erase(s,e); }

    std::string serialize(std::string const& indent = "") const;

protected:
    mutable map_any_ dict_; //bad, because there is no any_const_cast() const
};

std::size_t hash_value(any_type const& value);
bool operator==(any_type const& a, any_type const& b);

} //utils
} //psc

#endif // _SHOOTING_CUBES_UTILS_DICTIONARY_
