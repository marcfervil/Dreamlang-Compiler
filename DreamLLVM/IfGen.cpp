//
//  IfGen.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/28/21.
//

#include "IfGen.hpp"

extern "C"{

    IfData * init_if(LLVMData * context, Value * drm_bool){
        //printf("hurr");
        IfData * if_data = new IfData();
       
        Type * int32Type = Type::getInt32Ty(context->context);
        
        LoadInst *equ_result = get_pointer_value(context, int32Type, drm_bool);
        
        Value *if_comp = context->builder->get.CreateICmpEQ(equ_result, llvmInt(context, 1));
       
        //outs()<<(context->currentBlock->getParent()->getName());
       
        if_data->then = BasicBlock::Create(context->context, "then", context->currentBlock->getParent());
        if_data->ifcont = BasicBlock::Create(context->context, "ifcont");
        
        context->builder->get.CreateCondBr(if_comp, if_data->then, if_data->ifcont);
        
        context->currentBlock = if_data->then;
        context->builder->get.SetInsertPoint(context->currentBlock);
        
        
        
        return if_data;
    }

    void end_if(LLVMData * context, IfData * if_data){
        context->builder->get.CreateBr(if_data->ifcont);
        context->currentBlock->getParent()->getBasicBlockList().push_back(if_data->ifcont);
        context->currentBlock = if_data->ifcont;
        context->builder->get.SetInsertPoint(context->currentBlock);
    }

    
}
