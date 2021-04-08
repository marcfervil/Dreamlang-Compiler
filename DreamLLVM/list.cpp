//
//  list.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/8/21.
//


#ifndef standard_hpp
#define HASHSIZE 1022
#include "standard.hpp"
#endif

extern "C"{

    dreamObj * dreamList(int num_args, ...){
   
        va_list valist;
        dreamObj** list = (dreamObj **)malloc(num_args * sizeof(dreamObj *));
        va_start(valist, num_args);

        for (int i = 0; i < num_args; i++) {
            dreamObj * arg = va_arg(valist, dreamObj *);
            //printf("%s", rep(arg));
            list[i] = copy(arg);
        }
        va_end(valist);
        dreamObj * obj = make_dream((void *) list, dreamObjType);
        
       // set_var(obj, "luv", dreamStr("dopksodp"));
        
        //print(1, ((dreamObj**) obj->value)[0]);
        //list_get(obj, dreamInt(1));
        
        
        dreamObj* func = dreamFunc((void *) list_get);
        set_var(func, "@context", obj);
        
        set_var(obj, "get", func);
        
        return obj;
    }


    dreamObj * list_get(dreamObj * scope, dreamObj * index){
        if(index->type != dreamIntType)nightmare("List index must be int");
        dreamObj* self =  scope->parent_scope;
        return  ((dreamObj**) self->value)[*(int *)(index->value)];

    }
    
}
