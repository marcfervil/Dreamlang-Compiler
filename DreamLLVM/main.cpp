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


typedef struct LLVMData{
    LLVMContext context;
    Module * module;
    Function *mainFunc;
    BasicBlock *currentBlock;
    ExecutionEngine* engine;
    
    std::unique_ptr<Module> owner;
} LLVMData;

StructType * dreamObjTy;
PointerType * dreamObjPtrTy;
PointerType * voidPointerTy;


LLVMData * llvm_init(){
    InitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

   // LLVMContext Context;
    
   // LLVMData * new_context = (LLVMData *) malloc(sizeof(struct LLVMData));
    LLVMData * new_context = new LLVMData();

//    new_context -> context = Context;
    
    new_context->owner = std::make_unique<Module>("main", new_context -> context);
    Module *M = new_context->owner.get();

    
   
    new_context -> module = M;
    //context -> module->getNamedMDList().back();
    
    voidPointerTy  = PointerType::get(PointerType::getVoidTy(new_context -> context), 0);
    //PointerType* StructPointerTy = PointerType::get(StructType::get(Context), 0);
    
    dreamObjTy = StructType::create(new_context -> context, "dreamObj");
    dreamObjTy->setBody({ Type::getInt32Ty(new_context -> context) , dreamObjTy , voidPointerTy });
    dreamObjPtrTy = PointerType::get(dreamObjTy, 0);
    
    
    Type * int32Type = Type::getInt32Ty(new_context -> context);
    new_context -> mainFunc = Function::Create(FunctionType::get(int32Type, {}, false), Function::ExternalLinkage, "main_func", new_context->module);
    
    
   // BasicBlock *currentBlock =
    new_context -> currentBlock = BasicBlock::Create(new_context -> context, "EntryBlock", new_context->mainFunc);

    //new_context -> mainFunc -> print(outs());
    
    return new_context;
}

void llvm_run(LLVMData * context){
    context -> engine = EngineBuilder(std::move(context->owner)).create();
    std::vector<GenericValue> noargs;
    GenericValue gv = context->engine->runFunction(context->mainFunc, noargs);
    outs() << "We just constructed this LLVM module:\n\n" << *context->module;
    outs() << "Result: " << gv.IntVal << "\n";
}



int main(){

    LLVMData * llvmData = llvm_init();
   
    
    IRBuilder<> builder(llvmData->currentBlock);
    Value * num = builder.getInt32(30);
    builder.CreateRet(num);
    
    llvm_run(llvmData);

}
