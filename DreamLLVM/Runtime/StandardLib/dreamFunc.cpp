//
//  dreamFunc.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "dreamFunc.hpp"




dreamObj * dreamFunc(void * value){
    
    dreamObj * obj = make_dream((void *)value, dreamFuncType);
    
    return obj;
}
