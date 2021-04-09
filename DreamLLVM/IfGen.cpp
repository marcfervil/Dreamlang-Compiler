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

    void end_if(LLVMData * context, IfData * if_data, bool has_return){
        if(!has_return)context->builder->get.CreateBr(if_data->ifcont);
        context->currentBlock->getParent()->getBasicBlockList().push_back(if_data->ifcont);
        context->currentBlock = if_data->ifcont;
        context->builder->get.SetInsertPoint(context->currentBlock);
    }

    
    ForData * init_for(LLVMData * context, const char * var_name, Value * cond){

        ForData * for_data = new ForData();
       
        //Type * int32Type = Type::getInt32Ty(context->context);
        
       // LoadInst *equ_result = get_pointer_value(context, int32Type, cond);
            
       
        Value * null_dream_val = new LoadInst(dreamObjPtrTy, context->owner->getOrInsertGlobal("nullDream", dreamObjPtrTy), "null_dream", context->currentBlock);
        //log_llvm(context, null_dream_val);
        //CreateICmpEQ
        //equ_result
        
        /*
        cond->getType()->print(outs());
        printf("\n");
        null_dream_val->getType()->print(outs());
        printf("\n");*/
        
       // printf("yuh %s", cond->getType()->strin);
        
        Value *for_comp = context->builder->get.CreateICmpNE(cond, null_dream_val);
       
        
        
        //outs()<<(context->currentBlock->getParent()->getName());
       
        for_data->then = BasicBlock::Create(context->context, "then", context->currentBlock->getParent());
        for_data->forcont = BasicBlock::Create(context->context, "forcont");
        
        context->builder->get.CreateCondBr(for_comp, for_data->then, for_data->forcont);
        
        context->currentBlock = for_data->then;
        context->builder->get.SetInsertPoint(context->currentBlock);
        
        
        
        return for_data;
    }

    void end_for(LLVMData * context, ForData * for_data, bool has_return){
        //context->builder->get.CreateBr(if_data->ifcont);
        
        
        
        context->currentBlock->getParent()->getBasicBlockList().push_back(for_data->forcont);
        context->currentBlock = for_data->forcont;
        context->builder->get.SetInsertPoint(context->currentBlock);
    }


}
