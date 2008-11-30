
#include <boost/multi_array.hpp>
#include <tr1/tuple>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace boost;
using namespace std;
using namespace tr1;

namespace std{
    ostream& operator<<(ostream& os, tuple<int,int> const& t){
        return os << "fst: " << get<0>(t) << " snd: " << get<1>(t);
    }
}

int main(){
    multi_array<tuple<int, int>, 2> m(extents[6][11]);
    for(int y=0; y<11; ++y)
        for(int x=0; x<6; ++x)
            m[x][y] = tie(x, y);

    copy(m.origin(), m.origin()+6*11, ostream_iterator<tuple<int,int> >(cout, "\n"));
    cout << endl;
}
