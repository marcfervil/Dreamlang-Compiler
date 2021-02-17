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
    dreamObjTy->setBody({ Type::getInt32Ty(new_context -> context) , dreamObjTy , voidPointerTy });
    dreamObjPtrTy = PointerType::get(dreamObjTy, 0);
    
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
    std::vector<GenericValue> noargs;
    GenericValue gv = context->engine->runFunction(context->mainFunc, noargs);
    
    outs() << "Result: " << gv.IntVal << "\n";
    delete context -> engine;
    llvm_shutdown();
}

Value * str(LLVMData* context, string value){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "sr_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["str"], context->builder->get.CreateGlobalStringPtr(StringRef(value)));
    //builder.CreateGlobalStringPtr(StringRef("(Love laiojfweojfewofjefworugh?)"))->getType()->print(outs());
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "str", context->currentBlock);
    return object;
}

Value * call_standard(LLVMData* context, string funcName, ArrayRef<Value *> args = None){
    Value * callResult = context->builder->get.CreateCall(functions[funcName], args);
    return callResult;
}

int main(){

    LLVMData * llvmData = llvm_init();
    
   
    call_standard(llvmData, "print",  str(llvmData, "more lyfe"));
    
    llvmData->builder->get.CreateRet(llvmData->builder->get.getInt32(69));
    llvm_run(llvmData);

}
