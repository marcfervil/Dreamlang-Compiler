//
//  util.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "util.hpp"

extern "C"{


char * dog(const char * left, const char * right, const char * sep){
    char * str;
    if(sep == NULL)
        asprintf(&str, "%s%s", left, right);
    else
        asprintf(&str, "%s%s%s", left, sep, right);
    return str;
}


 dreamObj * deref_var(dreamObj ** obj){
    return (obj==NULL) ? NULL : *obj;
}

void add_native_func(dreamObj * obj, const char * name, void * func){

    dreamObj* obj_func = dreamFunc(func);
    set_var(obj_func, "@context", obj);
    set_var(obj, name, obj_func);
}

int to_int(dreamObj * obj){
    return *((int *)obj->value);
}

char * to_char(dreamObj * obj){
    return ((char *)obj->value);
}

}
