//
//  dreamBool.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamBool.hpp"

extern "C" {

dreamObj *** proto_bool;
int has_proto_bool = 0;

dreamObj * dreamBool(int value){

   // dreamObj *obj = (dreamObj *) malloc(sizeof(struct dreamObj));

    int *num = (int *)malloc(sizeof *num);
    *num = value;
    dreamObj * obj = make_dream((void *)num, dreamBoolType);

    if(!has_proto_bool ){
        proto_bool = (dreamObj ***)malloc(HASHSIZE * sizeof(dreamObj **));
        for (int i = 0; i < HASHSIZE; i++) {
            (proto_bool[i]) = (dreamObj **) malloc(sizeof(struct dreamObj *));
            *(proto_bool[i]) = NULL;
        }
        obj->vars = proto_bool;
        set_var(obj, "equals", dreamFunc((void *)num_equals));
        has_proto_bool = 1;

    }else{
        obj->vars = proto_bool;
    }

   // set_var(obj, "equals", dreamFunc((void *)num_equals));
    return obj;
}
}
