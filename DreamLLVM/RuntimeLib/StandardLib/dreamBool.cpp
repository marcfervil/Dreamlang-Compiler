//
//  dreamBool.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamBool.hpp"

extern "C" {
dreamObj * dreamBool(int value){
    int *num = (int *)malloc(sizeof *num);
    *num = value;
    dreamObj * obj = make_dream((void *)num, dreamBoolType);
    set_var(obj, "equals", dreamFunc((void *)num_equals));
    return obj;
}
}
