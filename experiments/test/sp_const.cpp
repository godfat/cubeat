
#include <tr1/memory>

using std::tr1::shared_ptr;

struct C{
    void f(){};
    void g() const{};
};

int main(){
    shared_ptr<C const> cc(new C);
    shared_ptr<C> c(new C);
    // cc->f();
    cc->g();
    c->f();
    c->g();
    // c = cc;
    cc = c;
}
