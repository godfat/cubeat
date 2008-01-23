
#ifndef _FOR_EACH_MULTI_ARRAY_HPP_
#define _FOR_EACH_MULTI_ARRAY_HPP_

namespace psc{

template <class MultiArray, class Fun>
MultiArray& for_each(MultiArray& array, Fun fun){
    typename MultiArray::element* data = array.origin();
    for(int i=0, iend=array.num_elements(); i!=iend; ++i)
        fun(data[i], i);
    return array;
}

template <class MultiArray, class Fun>
MultiArray& reverse_for_each(MultiArray& array, Fun fun){
    typename MultiArray::element* data = array.origin();
    for(int i=array.num_elements()-1, iend=-1; i!=iend; --i)
        fun(data[i], i);
    return array;
}

} // end of namespace

#endif
