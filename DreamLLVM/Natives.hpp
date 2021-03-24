//
//  Natives.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 3/20/21.
//

#ifndef Natives_hpp
#define Natives_hpp

#include "main.hpp"
extern "C"{
    Value * native_add(LLVMData* context, Value *var1, Value *var2);
    Value * native_sub(LLVMData* context, Value *var1, Value *var2);
    Value * native_mul(LLVMData* context, Value *var1, Value *var2);
    Value * native_div(LLVMData* context, Value *var1, Value *var2);
}   

#include <stdio.h>

#endif /* Natives_hpp */
