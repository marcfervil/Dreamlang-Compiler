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

//struct that represents llvm data
typedef struct LLVMData{
    LLVMContext context;
    Module * module;
    Function *mainFunc;
    BasicBlock *currentBlock;
    ExecutionEngine* engine;
    std::unique_ptr<Module> owner;
} LLVMData;


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

    return new_context;
}


//run our llvm code
void llvm_run(LLVMData * context){
    context -> engine = EngineBuilder(std::move(context->owner)).create();
    std::vector<GenericValue> noargs;
    GenericValue gv = context->engine->runFunction(context->mainFunc, noargs);
    outs() << "We just constructed this LLVM module:\n\n" << *context->module;
    outs() << "Result: " << gv.IntVal << "\n";
    delete context -> engine;
    llvm_shutdown();
}



int main(){

    LLVMData * llvmData = llvm_init();
   
    IRBuilder<> builder(llvmData->currentBlock);
    Value * num = builder.getInt32(30);
    builder.CreateRet(num);
    
    llvm_run(llvmData);

}
