//
//  dreamFunc.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamFunc.hpp"



extern "C" {





dreamCall to_call(dreamObj * obj){
    return (dreamCall) obj->value ;
}



dreamObj * dreamFunc(void * value){
    
    dreamObj * obj = make_dream((void *)value, dreamFuncType);
    
    return obj;
}



dreamObj * dreamFuncWithContext(void * value, dreamObj * context){
    dreamObj * obj = dreamFunc(value);
    set_var(obj, "@context", context);
    return obj;
}

}
