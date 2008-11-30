
#include <iostream>
// #include <tr1/functional>
#include <boost/multi_array.hpp>
#include <boost/lambda/lambda.hpp>

using boost::lambda::_1;
using boost::lambda::_2;

using std::cout;
using std::endl;
// using std::tr1::placeholders::_1;
// using std::tr1::placeholders::_2;
using boost::multi_array;
using boost::extents;

template <class MultiArray, class Fun>
MultiArray& for_each(MultiArray& array, Fun fun){
    typename MultiArray::element* data = array.data();
    for(int i=0, iend=array.num_elements(); i!=iend; ++i)
        fun(data[i], i);
    return array;
}

int main(){
    multi_array<int, 2> a(extents[2][2]);
    for_each(a, _1 = _2*2);
    for_each(a, cout << _1);
    cout << endl;
}
