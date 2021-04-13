//
//  mac_compat.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/12/21.
//


#include "mac_compat.hpp"


extern "C"{


dreamObj * mac_start(dreamObj * scope){
    dreamObj* self = scope->parent_scope;
    
    Widget * app = (Widget *)self->value;
    app->start();
    
    return nullDream;
    
}

dreamObj * app(dreamObj * name){

    
   
    
    dreamObj * obj = make_dream((void *)  mac_init(((char *)name->value)), dreamObjType);
    
    add_native_func(obj, "start", (void *) mac_start);
    
   
    
    
    return obj;
}



}
