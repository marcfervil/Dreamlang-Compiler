//
//  main.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/15/21.
//

#include "main.hpp"
#include "standard.hpp"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
#include <unistd.h>




using namespace llvm;
using namespace std;


StructType * dreamObjTy;
PointerType * dreamObjPtrTy;
PointerType * voidPointerTy;

class LLVMBuilder
{
    // Access specifier
    public:
  
    // Data Members
    IRBuilder<> get;
  
    // Member Functions()
    LLVMBuilder(BasicBlock *block) : get(block) {
  
    }
 
};

//struct that represents llvm data
typedef struct LLVMData{
    LLVMContext context;
    Module * module;
    Function *mainFunc;
    BasicBlock *currentBlock;
    ExecutionEngine* engine;
    LLVMBuilder * builder;
    std::unique_ptr<Module> owner;
    
} LLVMData;



map<string, FunctionCallee> functions;


//expose standard.cpp functions to LLVM
void loadStandard(LLVMData* context){
    functions["print"] = context->owner->getOrInsertFunction("print", FunctionType::get(PointerType::getVoidTy(context->context),dreamObjPtrTy, false));
    functions["printf"] = context->owner->getOrInsertFunction("printf", FunctionType::get(IntegerType::getInt32Ty(context->context), PointerType::get(Type::getInt8Ty(context->context), 0), true));
    functions["object"] = context->owner->getOrInsertFunction("make_dream", FunctionType::get(dreamObjPtrTy, voidPointerTy, false ));
    functions["str"] = context->owner->getOrInsertFunction("dreamStr", FunctionType::get(dreamObjPtrTy, PointerType::get(Type::getInt8Ty(context->context), 0), false));
    functions["int"] = context->owner->getOrInsertFunction("dreamInt", FunctionType::get(dreamObjPtrTy, PointerType::get(Type::getInt32Ty(context->context), 0), false));
}


//initialize llvm & return context struct
LLVMData * llvm_init(){
    //initialize llvm
    InitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    //initialize context struct
    LLVMData * new_context = new LLVMData();
    
    //create main module
    new_context->owner = std::make_unique<Module>("main", new_context -> context);
    Module *M = new_context->owner.get();
    new_context -> module = M;
    
    //initialize type variables
    voidPointerTy = PointerType::get(PointerType::getVoidTy(new_context -> context), 0);
    dreamObjTy = StructType::create(new_context -> context, "dreamObj");
    dreamObjPtrTy = PointerType::get(dreamObjTy, 0);
    dreamObjTy->setBody({ Type::getInt8PtrTy(new_context -> context) , dreamObjPtrTy , voidPointerTy, dreamObjPtrTy });
    
    
    //create main function & block
    Type * int32Type = Type::getInt32Ty(new_context -> context);
    new_context -> mainFunc = Function::Create(FunctionType::get(int32Type, {}, false), Function::ExternalLinkage, "main_func", new_context->module);
    new_context -> currentBlock = BasicBlock::Create(new_context -> context, "EntryBlock", new_context->mainFunc);
    
    new_context -> builder = new LLVMBuilder(new_context->currentBlock);
    
    
    loadStandard(new_context);
    return new_context;
}


//run our llvm code
void llvm_run(LLVMData * context){
    
    context -> engine = EngineBuilder(std::move(context->owner)).create();
    outs() << "We just constructed this LLVM module:\n\n" << *context->module;
    printf("\n\n");
    std::vector<GenericValue> noargs;
    GenericValue gv = context->engine->runFunction(context->mainFunc, noargs);
    printf("\n");
    outs() << "Result: " << gv.IntVal << "\n";
    delete context -> engine;
    llvm_shutdown();
}

Value * call_standard(LLVMData* context, string funcName, ArrayRef<Value *> args = None){
    Value * callResult = context->builder->get.CreateCall(functions[funcName], args);
    return callResult;
}

Value * str(LLVMData* context, string value){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "str_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["str"], context->builder->get.CreateGlobalStringPtr(StringRef(value)));
    //builder.CreateGlobalStringPtr(StringRef("(Love laiojfweojfewofjefworugh?)"))->getType()->print(outs());
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "str", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}

Value * num(LLVMData* context, int value){
    Value* builtInt = context->builder->get.getInt32(value);
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["int"], builtInt);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "int", context->currentBlock);
    return object;
}

Value * num(LLVMData* context, Value * value){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["int"], value);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "int", context->currentBlock);
    return object;
}



Value* get_value(LLVMData* context, Type * type, Value * obj ){
    //LoadInst * lod = new LoadInst(dreamObjPtrTy, obj, "v3",  context->currentBlock);
    std::vector<llvm::Value*> indices(2);
    indices[0] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 0, true));
    indices[1] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 2, true));
    Value *valuePointer = context->builder->get.CreateGEP(obj, indices,  "memberptr");
    LoadInst *value = new LoadInst(type, valuePointer, "temp", context->currentBlock);
    
    /*
    Value* valueStr =  context->builder->get.CreateGlobalStringPtr(StringRef("out: (%d)\n"));
    call_standard(context, "printf", {valueStr, value});*/
    
    //printf(const char *, ...)
    return value;
}

Value * add(LLVMData* context, Value *var1, Value *var2){
    Value* value1 = get_value(context, Type::getInt32Ty(context->context), var1);
    Value* value2 = get_value(context, Type::getInt32Ty(context->context), var2);
    return num(context, context->builder->get.CreateAdd(value1, value2));
    //return context->builder->get.CreateAdd(value1, value2);
}

int main(){

    LLVMData * context = llvm_init();
    
    
    Value * int1 = num(context, 400);

    Value * twenty = add(context, num(context, 10), num(context, 10));
    call_standard(context, "print", add(context, int1, twenty));

    
    context->builder->get.CreateRet(context->builder->get.getInt32(0));
    llvm_run(context);

}
