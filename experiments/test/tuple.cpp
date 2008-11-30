
#include <tr1/tuple>
#include <tr1/type_traits>
#include <iostream>

using namespace std::tr1;

template <bool> struct is_equal_type       { typedef true_type type; };
template <>     struct is_equal_type<false>{ typedef false_type type; };

template <int Lhs, int Rhs> struct is_equal{
    enum{ value = Lhs == Rhs };
    typedef typename is_equal_type<value>::type type;
};

template <class T, class Tuple, int Index> struct type_index_imp;

template <class T, class Tuple, int Index, class Bool>
struct type_index_check{
    enum{ value = T::type_not_found };
};

template <class T, class Tuple, int Index>
struct type_index_check<T, Tuple, Index, false_type>{
        enum{ value = type_index_imp<T, Tuple, Index+1>::value };
};

template <class T, class Tuple, int Index, class Bool>
struct type_index_if{
    enum{ value = Index }; // found, stop recursion
};

template <class T, class Tuple, int Index>
struct type_index_if<T, Tuple, Index, false_type>{
    enum{ value = type_index_check<T, Tuple, Index,
            typename is_equal<Index+1, tuple_size<Tuple>::value>::type>::value };
};

template <class T, class Tuple, int Index>
struct type_index_imp{
    enum{ value = type_index_if<T, Tuple, Index,
            typename is_same<T, typename tuple_element<Index, Tuple>::type>::type>::value };
};

template <class T, class Tuple>
struct type_index{
    enum{ value = type_index_imp<T, Tuple, 0>::value };
};

int main(){
    typedef tuple<int, char, void> test_t;
    std::cout << type_index<int, test_t>::value << std::endl;
    std::cout << type_index<char, test_t>::value << std::endl;
    std::cout << type_index<void, test_t>::value << std::endl;
    std::cout << tuple_size<test_t>::value << std::endl;
    // std::cout << type_index<int*, test_t>::value << std::endl;
}
