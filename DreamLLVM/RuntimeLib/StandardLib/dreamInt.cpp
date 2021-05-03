//
//  dreamInt.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamInt.hpp"
extern "C" {

dreamObj *** proto_int;
int has_proto_int = 0;

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

    if(!has_proto_int ){
        proto_int = (dreamObj ***)malloc(HASHSIZE * sizeof(dreamObj **));
        for (int i = 0; i < HASHSIZE; i++) {
            (proto_int[i]) = (dreamObj **) malloc(sizeof(struct dreamObj *));
            *(proto_int[i]) = NULL;
        }
        obj->vars = proto_int;
        set_var(obj, "equals", dreamFunc((void *)num_equals));
        set_var(obj, "add", dreamFunc((void *)num_add));
        has_proto_int = 1;

    }else{
        obj->vars = proto_int;
    }

   // set_var(obj, "equals", dreamFunc((void *)num_equals));
   // set_var(obj, "add", dreamFunc((void *)num_add));

    //print(1, get_var(obj, "add"));
    return obj;
}
}
