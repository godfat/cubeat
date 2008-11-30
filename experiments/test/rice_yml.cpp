
// g++ rice_yml.cpp -o rice_yml -I/opt/local/lib/ruby/1.8/i686-darwin9.1.0/ -I/opt/local/lib/ruby/gems/1.8/gems/rice-1.0.1/ -L/opt/local/lib/ruby/gems/1.8/gems/rice-1.0.1/rice/ -lrice -lruby -std=c++98 -Wall -w

#include <ruby.h>

#include <rice/Hash.hpp>
#include <rice/Array.hpp>

#include <iostream>
#include <string>

using Rice::Object;
using Rice::Class;
using Rice::Hash;
using Rice::Array;
using Rice::String;
using Rice::protect;

void extract_hash(Hash const&);
void extract_array(Array const& array){
    Class hash_class(Hash().class_of());
    Class array_class(Array().class_of());
    Class string_class(String().class_of());
    for(Array::const_iterator i=array.begin(), iend=array.end(); i!=iend; ++i){
        if(i->is_instance_of(hash_class))
            extract_hash(*i);
        else if(i->is_instance_of(array_class))
            extract_array(*i);
        else if(i->is_instance_of(string_class))
            std::cout << *i << "\n";
        else
            std::cout << *i << "\n";
    }
}

void extract_hash(Hash const& hash){
    Class hash_class(Hash().class_of());
    Class array_class(Array().class_of());
    Class string_class(String().class_of());
    for(Hash::const_iterator i=hash.begin(), iend=hash.end(); i!=iend; ++i){
        std::cout << i->key << ":\n";
        Object obj(i->value);
        if(obj.is_instance_of(hash_class))
            extract_hash(obj);
        else if(obj.is_instance_of(array_class))
            extract_array(obj);
        else if(obj.is_instance_of(string_class))
            std::cout << i->value << "\n";
        else
            std::cout << i->value << "\n";
    }
}

int main(){
    ruby_init();
    rb_eval_string("$LOAD_PATH << '/opt/local/lib/ruby/1.8'");
    rb_eval_string("$LOAD_PATH << '/opt/local/lib/ruby/1.8/i686-darwin9.1.0'");
    rb_require("yaml");
    Hash h(protect(rb_eval_string, "YAML.load(File.read('database.yml'))"));
    extract_hash(h);
    std::cout << std::endl;
    std::cout << static_cast<Hash>(h[String("development")])[String("adapter")] << std::endl;
}
