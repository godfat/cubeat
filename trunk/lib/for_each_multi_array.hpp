
#ifndef __FOR_EACH_MULTI_ARRAY_HPP_
#define __FOR_EACH_MULTI_ARRAY_HPP_

template <class MultiArray, class Fun>
MultiArray& for_each(MultiArray& array, Fun fun){
    typename MultiArray::element* data = array.data();
    for(int i=0, iend=array.num_elements(); i!=iend; ++i)
        fun(data[i], i);
    return array;
}

#endif
