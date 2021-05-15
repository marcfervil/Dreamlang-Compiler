//
//  dreamStr.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamStr.hpp"

extern "C" {

int has_proto_str = 0;
dreamObj *** proto_str;

dreamObj * dreamStr(const char * value){
    
    char* result = (char*) malloc( strlen(value) + 1 );
    strcpy(result, value);
    
    dreamObj * obj = make_dream((void *) result, dreamStrType);
    if(!has_proto_str){
        proto_str = make_vars();
        obj->vars = proto_str;
        
        set_var(obj, "equals", dreamFunc((void *) str_equals));
        set_var(obj, "add", dreamFunc((void *)str_add));

        add_native_func(obj, "get",  (void *) str_get);
        add_native_func(obj, "len",  (void *) str_len);
    }else{
        obj->vars = proto_str;
    }
    
    return obj;
}

dreamObj * str_get(dreamObj * scope, dreamObj * index){
    dreamObj* self = scope->parent_scope;
    char str[2] = "\0";
    str[0] = to_char(self)[to_int(index)];
    return dreamStr(str);
}

dreamObj * str_len(dreamObj * scope){
    dreamObj* self = scope->parent_scope;
    return dreamInt(strlen(to_char(self)));
}

dreamObj * str_add(dreamObj * me, dreamObj * other){
    const char * me_val = (const char *) me->value;
    char * other_val ;
    
    if (other->type == dreamIntType){
        
        asprintf(&other_val, "%i", *(int *) other->value);
    }else{
        other_val = ( char *) other->value;
    }
    
    char* result;
    result = ( char*) malloc((strlen(me_val)+strlen(other_val)+1 ) * sizeof(char));
    strcpy(result, me_val);
    strcat(result, other_val);
    return dreamStr(( char * )result);
}

dreamObj * str_equals(dreamObj * me, dreamObj * other){
    const char * me_val = (const char *) me->value;
    const char * other_val = (const char *) other->value;
    
    return dreamBool(other->type==dreamStrType && strcmp(me_val, other_val)==0);
}
}
