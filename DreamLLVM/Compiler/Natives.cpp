//
//  Natives.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 3/20/21.
//

#include "Natives.hpp"

extern "C"{
    Value * native_add(LLVMData* context, Value *var1, Value *var2){
        
        /*
        Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
        Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
        return numVal(context, context->builder->get.CreateAdd(value1, value2));*/
       
        Value * result = context->builder->get.CreateAdd(var1, var2);
    
        return result;
        //return context->builder->get.CreateAdd(value1, value2);
    }

    Value * native_sub(LLVMData* context, Value *var1, Value *var2){
        
        /*
        Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
        Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
        return numVal(context, context->builder->get.CreateAdd(value1, value2));*/
       
        Value * result = context->builder->get.CreateSub(var1, var2);

        return result;
        //return context->builder->get.CreateAdd(value1, value2);
    }

    Value * native_mul(LLVMData* context, Value *var1, Value *var2){
        
        /*
        Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
        Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
        return numVal(context, context->builder->get.CreateAdd(value1, value2));*/
       
        Value * result = context->builder->get.CreateMul(var1, var2);

        return result;
        //return context->builder->get.CreateAdd(value1, value2);
    }


    Value * native_div(LLVMData* context, Value *var1, Value *var2){
        
        /*
        Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
        Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
        return numVal(context, context->builder->get.CreateAdd(value1, value2));*/
       
        Value * result = context->builder->get.CreateSDiv(var1, var2);

        return result;
        //return context->builder->get.CreateAdd(value1, value2);
    }
}
