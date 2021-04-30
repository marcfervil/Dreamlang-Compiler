//
//  IfGen.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/28/21.
//

#include "IfGen.hpp"

extern "C"{

    IfData * init_if(LLVMData * context, Value * drm_bool){

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


    void continue_for(LLVMData * context, ForData * for_data){
        context->builder->get.CreateBr(for_data->start);
    }

    void break_for(LLVMData * context, ForData * for_data){
        context->builder->get.CreateBr(for_data->forcont);
    }

    ForData * init_for(LLVMData * context, const char * var_name, Value * scope, Value * iter_func, Value * iter_func_call_scope){


        ForData * for_data = new ForData();
        for_data -> iter_func = iter_func;
        for_data -> iter_func_call_scope = iter_func_call_scope;
        for_data -> var_name = (char *)var_name;
        for_data -> scope = scope;
        
        Value * null_dream_val = new LoadInst(dreamObjPtrTy, context->owner->getOrInsertGlobal("nullDream", dreamObjPtrTy), "null_dream", context->currentBlock);
        
        
      //  for_data->last_iter_call = call(context, iter_func, 1, new Value*[]{iter_func_call_scope});
        //Value *for_comp = context->builder->get.CreateICmpNE(for_data->last_iter_call, null_dream_val);
        
        for_data->start = BasicBlock::Create(context->context, "for_start");


        context->builder->get.CreateBr(for_data->start);
        context->currentBlock->getParent()->getBasicBlockList().push_back(for_data->start);
        context->currentBlock = for_data->start;
        context->builder->get.SetInsertPoint(context->currentBlock);

        for_data->last_iter_call = call(context, iter_func, 1, new Value*[]{iter_func_call_scope});
        set_var_llvm(context,  for_data->scope, for_data->var_name, for_data->last_iter_call);
        
        
        for_data->then = BasicBlock::Create(context->context, "then" );
        for_data->forcont = BasicBlock::Create(context->context, "forcont" );
        //for_data->for_end = BasicBlock::Create(context->context, "forend" );

        Value *for_comp = context->builder->get.CreateICmpNE(for_data->last_iter_call, null_dream_val);
        context->builder->get.CreateCondBr(for_comp, for_data->then, for_data->forcont);
        context->currentBlock->getParent()->getBasicBlockList().push_back(for_data->then);
        context->currentBlock = for_data->then;
        context->builder->get.SetInsertPoint(context->currentBlock);



        return for_data;
    }
//
    void end_for(LLVMData * context, ForData * for_data, bool has_return){
        //context->builder->get.CreateBr(if_data->ifcont);


        if (!has_return) {
           // for_data->last_iter_call = call(context, for_data->iter_func, 1, new Value*[]{for_data->iter_func_call_scope});
          //  set_var_llvm(context,  for_data->scope, for_data->var_name, for_data->last_iter_call);
            Value * null_dream_val = new LoadInst(dreamObjPtrTy, context->owner->getOrInsertGlobal("nullDream", dreamObjPtrTy), "null_dream", context->currentBlock);
            Value *for_comp = context->builder->get.CreateICmpNE(for_data->last_iter_call, null_dream_val);

            context->builder->get.CreateCondBr(for_comp, for_data->start, for_data->forcont);
        }

        context->currentBlock->getParent()->getBasicBlockList().push_back(for_data->forcont);
        context->currentBlock = for_data->forcont;
        context->builder->get.SetInsertPoint(context->currentBlock);
    }


}
