//
//  dreamInt.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamInt.hpp"

dreamObj * num_equals(dreamObj * me, dreamObj * other){
    return dreamBool(((other->type==dreamIntType ||other->type==dreamBoolType)  &&
                    (*(int*) me->value)==(*(int*) other->value)));
}

dreamObj * num_add(dreamObj * me, dreamObj * other){
    
    /*
    if (other->type == dreamStrType){
        char * other_val;
        asprintf(&other_val, "%i", *(int *) other->value);
    }else{
        other_val = ( char *) other->value;
    }*/
    
    return dreamInt( (*(int*) me->value) + (*(int*) other->value));
}


dreamObj * dreamInt(int value){
    int *num = (int *)malloc(sizeof *num);
    *num = value;
    dreamObj * obj = make_dream((void *)num, dreamIntType);
    set_var(obj, "equals", dreamFunc((void *)num_equals));
    set_var(obj, "add", dreamFunc((void *)num_add));
    return obj;
}
