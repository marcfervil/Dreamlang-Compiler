//
//  list.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/8/21.
//

#include "list.hpp"

extern "C"{
    

    
    char * cat(const char * left, const char * right, const char * sep=NULL){
        char * str;
        if(sep == NULL)
            asprintf(&str, "%s%s", left, right);
        else
            asprintf(&str, "%s%s%s", left, sep, right);
        return str;
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

        add_native_func(obj, "get", (void *) list_get);
        add_native_func(obj, "set", (void *) list_set);
        add_native_func(obj, "push", (void *) list_push);
        add_native_func(obj, "repr", (void *) list_rep);


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

        
        dreamObj** temp_list = (dreamObj **)malloc((len + 1) * sizeof(struct dreamObj *));
        for(int i=0; i < len; i++){
            (temp_list[i]) = (*((dreamObj ***)self->value))[i];
        }
        free(*((dreamObj ***)self->value));
       
        *((dreamObj ***)self->value) = temp_list;
        (*((dreamObj ***)self->value))[len] = smart_copy(new_item);
        

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

        dreamObj * count_obj = dreamObject();
        add_native_func(count_obj, "iter", (void *) count_iter);
        set_var(count_obj, "num", num);

        return count_obj;
    }

    dreamObj * count_iter(dreamObj * scope){
        dreamObj* self =  scope->parent_scope;
        dreamObj * iter = dreamObject();
        set_var(iter, "max", get_var(self, "num"));


        set_var(iter, "index", dreamInt(0));

        dreamObj* next_func = dreamFunc((void *) count_iter_next);
        set_var(next_func, "@context", iter);
        set_var(iter, "next", next_func);

        return iter;
    }

    dreamObj * count_iter_next(dreamObj * scope){
        
        dreamObj* self =  scope->parent_scope;
        dreamObj * index = get_var(self, "index");
        dreamObj * max = get_var(self, "max");
        int val = (*(int *)(index->value));
        (*(int *)index->value) = val + 1;
        
        if(val>=(*(int *)(max->value)))return nullDream;
        
        return dreamInt(val);

    }

    

}
