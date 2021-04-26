//
//  list.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/8/21.
//

#include "list.hpp"

extern "C"{

    
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

        add_native_func(obj, "get", (void *) list_get);
        add_native_func(obj, "set", (void *) list_set);
        add_native_func(obj, "push", (void *) list_push);
        add_native_func(obj, "repr", (void *) list_rep);
        add_native_func(obj, "iter", (void *) list_iter);

        return obj;
    }


    dreamObj * list_get(dreamObj * scope, dreamObj * index){
        if(index->type != dreamIntType)nightmare("List index must be int");
        dreamObj* self = scope->parent_scope;
        return ((*(dreamObj***) self->value))[*(int *)(index->value)];

    }

    dreamObj * list_set(dreamObj * scope, dreamObj * index, dreamObj * value){
        if(index->type != dreamIntType)nightmare("List index must be int");
        dreamObj* self = scope->parent_scope;
        ((*(dreamObj***) self->value))[*(int *)(index->value)] = smart_copy(value);
        return value;
    }


    dreamObj * list_push(dreamObj * scope, dreamObj * new_item){
        dreamObj* self = scope->parent_scope;

        int len = * ((int *)(get_var(self, "len") -> value));
        set_var(self, "len", dreamInt(len+1));

        //TODO implement more performant list appending
        dreamObj** temp_list = (dreamObj **)malloc((len + 1) * sizeof(struct dreamObj *));
        for(int i=0; i < len; i++){
            (temp_list[i]) = (*((dreamObj ***)self->value))[i];
        }
        free(*((dreamObj ***)self->value));
       
        *((dreamObj ***)self->value) = temp_list;
        (*((dreamObj ***)self->value))[len] = smart_copy(new_item);
        

        return nullDream;
    }

    dreamObj *list_iter(dreamObj *scope) {
        dreamObj *self = scope->parent_scope;
        dreamObj *iter = dreamObject();
        iter->parent_scope = self;

        set_var(iter, "index", dreamInt(0));

        add_native_func(iter, "next", (void *) list_iter_next);

        return iter;
    }

    dreamObj *list_iter_next(dreamObj *scope) {
        dreamObj *self = scope->parent_scope;
        dreamObj *lst =  self->parent_scope;
        dreamObj *index = get_var(self, "index");
        dreamObj *max = get_var(lst, "len");

        int val = (*(int *) (index->value));
        (*(int *) index->value) = val + 1;

        if (val >= (*(int *) (max->value)))return nullDream;


        return ((*(dreamObj***) lst->value))[val];
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
        




    

}
