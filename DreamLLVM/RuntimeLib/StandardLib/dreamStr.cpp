//
//  dreamStr.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamStr.hpp"

extern "C" {
dreamObj * dreamStr(const char * value){
    
    char* result = (char*) malloc( strlen(value) + 1 );
    strcpy(result, value);
    
    dreamObj * obj = make_dream((void *) result, dreamStrType);
  
    set_var(obj, "equals", dreamFunc((void *) str_equals));
    set_var(obj, "add", dreamFunc((void *)str_add));

    return obj;
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
