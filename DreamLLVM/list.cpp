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
    
    void add_native_func(dreamObj * obj, const char * name, void * func){
        
        dreamObj* obj_func = dreamFunc(func);
        set_var(obj_func, "@context", obj);
        set_var(obj, name, obj_func);
    }

    const char * as_str(dreamObj * obj){
        
        return (const char *)obj->value;
    }
    
    int as_int(dreamObj * obj){
        
        return *(int *)obj->value;
    }


    dreamObj * dreamList(int num_args, ...){
   
        va_list valist;
        dreamObj** list = (dreamObj **)calloc(num_args, sizeof(struct dreamObj *));
        va_start(valist, num_args);

        for (int i = 0; i < num_args; i++) {
            dreamObj * arg = va_arg(valist, dreamObj *);
            list[i] = smart_copy(arg);
            
        }
        va_end(valist);
        dreamObj*** list_ptr = (dreamObj ***) malloc(sizeof(struct dreamObj **));
        *list_ptr = list;
        dreamObj * obj = make_dream((void *) list_ptr, dreamObjType);
        obj->is_list = true ;
        set_var(obj, "len", dreamInt(num_args));
        
        /*
         
         **/
        
        add_native_func(obj, "get", (void *) list_get);
        add_native_func(obj, "set", (void *) list_set);
        add_native_func(obj, "push", (void *) list_push);
        add_native_func(obj, "repr", (void *) list_rep);
       
        
        
        return obj;
    }



    dreamObj * list_get(dreamObj * scope, dreamObj * index){
        if(index->type != dreamIntType)nightmare("List index must be int");
        dreamObj* self = scope->parent_scope;
        //print(1, ((dreamObj**) self->value)[*(int *)(index->value)]);
        return  ((*(dreamObj***) self->value))[*(int *)(index->value)];

    }

    dreamObj * list_set(dreamObj * scope, dreamObj * index, dreamObj * value){
        if(index->type != dreamIntType)nightmare("List index must be int");
        dreamObj* self = scope->parent_scope;
        //print(1, ((dreamObj**) self->value)[*(int *)(index->value)]);
        ((*(dreamObj***) self->value))[*(int *)(index->value)] = smart_copy(value);
        return value;
    }


    dreamObj * list_push(dreamObj * scope, dreamObj * new_item){
        dreamObj* self = scope->parent_scope;
        //scope->parent_scope;
        
        //printf("%s",self->name);
        
        int len = * ((int *)(get_var(self, "len") -> value));
       // dreamInt(len+1);
       set_var(self, "len", dreamInt(len+1));
        //printf("%d", len);
       
      //  self->value =
       // dreamObj ** arr = ((dreamObj **)self->value);
        //printf("fowpfkewof\n");
      //  printf("VIBE CHECK %d\n", self->value == scope->parent_scope->value);
        
        dreamObj** temp_list = (dreamObj **)malloc((len + 1) * sizeof(struct dreamObj *));
        for(int i=0; i < len; i++){
            (temp_list[i]) = (*((dreamObj ***)self->value))[i];
        }
       // free(self->value);
       
        *((dreamObj ***)self->value) = temp_list;
        (*((dreamObj ***)self->value))[len] = smart_copy(new_item);
        
        
        //scope->parent_scope->value = self->value;
       
   //     print(1, ((dreamObj **)scope->parent_scope->value)[3]);
        
        return nullDream;
    }

    //TODO: idk, this just feels wrong
    dreamObj * list_rep(dreamObj * self){
        int len = * ((int *)(get_var(self, "len") -> value));
        char * str = (char *)"[";
        for(int i=0; i < len; i++){
            dreamObj * obj = ((*(dreamObj***) self->value))[i];
            char * repr = (char *)rep(obj);
            if(obj->type == dreamStrType)repr = cat("'", "'", repr);
            
            str = cat(str, repr);
            if(i!=len-1) str = cat(str, ", ");
        }
        str = cat(str, "]");
        return dreamStr(str);
    }
        


    //code for count iterator

    dreamObj * count(dreamObj * num){

        dreamObj * iter = dreamObject();
        set_var(iter, "max", num);
        
        
        set_var(iter, "index", dreamInt(0));
        
        dreamObj* next_func = dreamFunc((void *) iter_next);
        set_var(next_func, "@context", iter);
        set_var(iter, "next", next_func);
        
        return iter;
    }


    dreamObj * iter_next(dreamObj * scope){
        
        dreamObj* self =  scope->parent_scope;
        dreamObj * index = get_var(self, "index");
        dreamObj * max = get_var(self, "max");
        int val = (*(int *)(index->value));
        (*(int *)index->value) = val + 1;
        
        if(val>=(*(int *)(max->value)))return nullDream;
        
        return dreamInt(val);

    }

    

}
