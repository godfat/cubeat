
#ifndef _FOR_EACH_MULTI_ARRAY_HPP_
#define _FOR_EACH_MULTI_ARRAY_HPP_

#include "utils/vector_2d.hpp"

namespace psc{ namespace utils{

template <class MultiArray, class Fun>
MultiArray& for_each(MultiArray& array, Fun fun){
    typename MultiArray::element* data = array.origin();
    for(int i=0, iend=array.num_elements(); i!=iend; ++i)
        fun(data[i], i);
    return array;
}

template <class MultiArray, class Fun>
MultiArray& for_each_ignore_null(MultiArray& array, Fun fun){
    for(typename MultiArray::size_type y=0; y != array.shape()[1]; ++y)
        for(typename MultiArray::size_type x=0; x != array.shape()[0]; ++x)
            if(array[x][y])
                fun(array[x][y], x, y);
    return array;
}

}} // end of namespace

#endif
