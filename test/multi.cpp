
#include <iostream>
#include <boost/multi_array.hpp>
#include <boost/array.hpp>

using std::cout;
using std::endl;
using boost::multi_array;
using boost::array;
using boost::extents;
typedef multi_array<int, 2> array_2d;

int main(){
    int count = -1;
    array_2d a(extents[10][10]);
    for(array_2d::iterator i=a.begin(), iend=a.end(); i!=iend; ++i)
        for(array_2d::iterator::value_type::iterator j=i->begin(), jend=i->end(); j!=jend; ++j)
            *j = ++count;

    for(array_2d::index y=0; y!=10; ++y)
        for(array_2d::index x=0; x!=10; ++x)
            cout << a[y][x] << " ";
    cout << endl;

    typedef array_2d::index_range range;
    array_2d::index_gen indices;
    array_2d::array_view<1>::type myview =
        a[ indices[range()][0] ];

    for(array_2d::index i=0; i!=10; ++i)
        cout << myview[i] << " ";
    cout << endl;

    cout << a.origin()[0] << endl;
    cout << a.origin()[10] << endl;
}
