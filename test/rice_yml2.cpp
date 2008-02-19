
// g++ rice_yml2.cpp -o rice_yml2 -I/opt/local/lib/ruby/1.8/i686-darwin9.1.0/ -I/opt/local/lib/ruby/gems/1.8/gems/rice-1.0.1/ -L/opt/local/lib/ruby/gems/1.8/gems/rice-1.0.1/rice/ -lrice -lruby -std=c++98 -Wall -w

#include <ruby.h>
#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <iostream>

using Rice::Object;
using Rice::Class;
using Rice::Hash;
using Rice::Array;
using Rice::String;
using Rice::protect;

std::string spacer(int depth){
    std::string result;
    for(int i=0; i<depth; ++i)
        result += "  ";
    return result;
}

class Extractor{
public:
    Extractor(): hash_class_(Hash().class_of()),
                 array_class_(Array().class_of()),
                 string_class_(String().class_of())
    {}
    void extract(Object const& obj, int depth = 0) const{
        if(obj.is_instance_of(hash_class_))
            extract_hash(obj, depth);
        else if(obj.is_instance_of(array_class_))
            extract_array(obj, depth);
        else if(obj.is_instance_of(string_class_))
            std::cout << spacer(depth) << obj << "\n";
        else
            std::cout << spacer(depth) << obj << "\n";
    }
private:
    void extract_array(Array const& obj, int depth) const{
        for(Array::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i){
            extract(*i, depth);
        }
    }
    void extract_hash(Hash const& obj, int depth) const{
        for(Hash::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i){
            std::cout << spacer(depth) << i->key << ":";
            if(static_cast<Object>(i->value).is_instance_of(hash_class_))
                std::cout << "\n", extract(i->value, depth+1);
            else
                std::cout << " ", extract(i->value, 0);
        }
    }
private:
    Class hash_class_, array_class_, string_class_;
};

int main(){
    ruby_init();
    rb_eval_string("$LOAD_PATH << '/opt/local/lib/ruby/1.8'");
    rb_eval_string("$LOAD_PATH << '/opt/local/lib/ruby/1.8/i686-darwin9.1.0'");
    rb_require("yaml");
    Hash h(protect(rb_eval_string, "YAML.load(File.read('database.yml'))"));
    Extractor e;
    e.extract(h);
    std::cout << std::endl;
    std::cout << static_cast<Hash>(h[String("development")])[String("adapter")] << std::endl;
}
