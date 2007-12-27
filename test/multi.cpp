
#include <iostream>
#include <boost/multi_array.hpp>

using std::cout;
using std::endl;
using boost::multi_array;
using boost::extents;
typedef multi_array<int, 2> array_2d;

int main(){
    array_2d a(extents[10][10]);
    for(array_2d::const_iterator i=a.begin(), iend=a.end(); i!=iend; ++i)
        for(array_2d::const_iterator::value_type::const_iterator j=i->begin(), jend=i->end(); j!=jend; ++j)
            cout << *j;
    cout << endl;

    for(array_2d::index y=0; y!=10; ++y)
        for(array_2d::index x=0; x!=10; ++x)
            cout << a[y][x];
    cout << endl;

    cout << a.size() << endl;
}
