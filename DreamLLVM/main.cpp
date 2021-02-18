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
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
#include <unistd.h>



extern "C" {
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


int testing(int f){
    printf("IT WORKS!!");
    return 0;

}

//expose standard.cpp functions to LLVM
void loadStandard(LLVMData* context){
    
   
    
    functions["print"] = context->owner->getOrInsertFunction("print", FunctionType::get(PointerType::getVoidTy(context->context),dreamObjPtrTy, false));
    functions["printf"] = context->owner->getOrInsertFunction("printf", FunctionType::get(IntegerType::getInt32Ty(context->context), PointerType::get(Type::getInt8Ty(context->context), 0), true));
    functions["object"] = context->owner->getOrInsertFunction("make_dream", FunctionType::get(dreamObjPtrTy, voidPointerTy, false ));
    functions["str"] = context->owner->getOrInsertFunction("dreamStr", FunctionType::get(dreamObjPtrTy, PointerType::get(Type::getInt8Ty(context->context), 0), false));
    functions["int"] = context->owner->getOrInsertFunction("dreamInt", FunctionType::get(dreamObjPtrTy, PointerType::get(Type::getInt32Ty(context->context), 0), false));
    functions["test"] = context->owner->getOrInsertFunction("testing", FunctionType::get(IntegerType::getInt32Ty(context->context), Type::getInt32Ty(context->context), false));
}


//initialize llvm & return context struct
LLVMData * llvm_init(){
    //initialize llvm
    InitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    llvm::sys::DynamicLibrary::LoadLibraryPermanently("dream.so");
    llvm::sys::DynamicLibrary::LoadLibraryPermanently("dream.o");

    //initialize context struct
    LLVMData * new_context = new LLVMData();
    
    //create main module
    new_context->owner = std::make_unique<Module>("maindream", new_context -> context);
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
    
    //load standard functions
    loadStandard(new_context);
    return new_context;
}


void llvm_link(LLVMData * context, string fileName ){
    // context->engine->add
     
     //llvm::sys::DynamicLibrary::LoadLibraryPermanently("./lib/"+fileName);
     
     std::string objectFileName("./lib/"+fileName);

     ErrorOr<std::unique_ptr<MemoryBuffer>> buffer =
       MemoryBuffer::getFile(objectFileName.c_str());

     if (!buffer) {
       // handle error
     }

     Expected<std::unique_ptr<llvm::object::ObjectFile>> objectOrError = llvm::object::ObjectFile::createObjectFile(buffer.get()->getMemBufferRef());
     //OfbectcreateObjectFile(buffer.get()->getMemBufferRef());

     if (!objectOrError) {
       // handle error
     }

     std::unique_ptr<llvm::object::ObjectFile> objectFile(std::move(objectOrError.get()));

     auto owningObject = llvm::object::OwningBinary<llvm::object::ObjectFile>(std::move(objectFile),std::move(buffer.get()));

     context->engine->addObjectFile(std::move(owningObject));
}

//run our llvm code
void llvm_run(LLVMData * context, bool link_obj=true, bool print_module = false){
    context -> engine = EngineBuilder(std::move(context->owner)).create();
    
    if(link_obj){
        //for some reason library functions need to be exposed like this
        for (auto const& x : functions)
        {
            string funcName = x.first;
            Function *f = Function::Create(functions[funcName].getFunctionType(),Function::ExternalLinkage, funcName, context->module);
            context->engine->addGlobalMapping(f, functions[funcName].getCallee());
        }
    }
    
    if(print_module){
        outs() << "We just constructed this LLVM module:\n\n" << *context->module;
        printf("\n\n");
    }
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
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "str", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}

Type * int_type(LLVMData* context){
    
    return Type::getInt32Ty(context->context);
}

Value * num(LLVMData* context, int value){
    Value* builtInt = context->builder->get.getInt32(value);
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["int"], builtInt);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "int", context->currentBlock);
    return object;
}


Value * test0(LLVMData* context, int value){
    Value* builtInt = context->builder->get.getInt32(value);
   
    
    return builtInt;
}


Value * test1(LLVMData* context, int value){
    Value* builtInt = context->builder->get.getInt32(value);
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    
    return objStore;
}

void  test2(LLVMData* context){
    outs()<<"SIZE: "<<functions.size();
    Value* builtInt = context->builder->get.getInt32(30);
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    outs() << "\n";
    functions["inte"].getFunctionType()->print(outs());
    Value * callResult = context->builder->get.CreateCall(functions["inte"], builtInt);
    outs() << "\n";
  
}

void  test2a(LLVMData* context){
    Value* str = context->builder->get.CreateGlobalStringPtr(StringRef("The night me and your mama met"));
    context->builder->get.CreateCall(functions["printf"], str);
  
}

void  test2b(LLVMData* context){
    Value* str =context->builder->get.getInt32(30);
    context->builder->get.CreateCall(functions["test"], str);
  
}

Value * test3(LLVMData* context, int value){
    Value* builtInt = context->builder->get.getInt32(value);
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["int"], builtInt);
    Value * e = new StoreInst(callResult, objStore, context->currentBlock);
    return e;
}


Value * numVal(LLVMData* context, Value * value){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["int"], value);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "int", context->currentBlock);
    return object;
}



Value* get_value(LLVMData* context, Type * type, Value * obj ){
    std::vector<llvm::Value*> indices(2);
    indices[0] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 0, true));
    indices[1] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 2, true));
    Value *valuePointer = context->builder->get.CreateGEP(obj, indices,  "memberptr");
    LoadInst *value = new LoadInst(type, valuePointer, "temp", context->currentBlock);
    

    return value;
}

Value * add(LLVMData* context, Value *var1, Value *var2){
    Value* value1 = get_value(context, Type::getInt32Ty(context->context), var1);
    Value* value2 = get_value(context, Type::getInt32Ty(context->context), var2);
    return numVal(context, context->builder->get.CreateAdd(value1, value2));
    //return context->builder->get.CreateAdd(value1, value2);
}

Value * retVal(LLVMData* context, Value * value ){
    return context->builder->get.CreateRet(value);
}


Value * llvmInt(LLVMData* context, int value){
    return context->builder->get.getInt32(value);
}

Value * llvmStr(LLVMData* context, char * value){
    return context->builder->get.CreateGlobalStringPtr(StringRef(value));
}

void luv(LLVMData * context){
    test2b(context);
   
    
    retVal(context, llvmInt(context, 59));
}

int main(){

    LLVMData * context = llvm_init();
   // Value * int1 = num(context, 400);

    //Value * int1 = num(context, 400);
    /*
    
    Value * int1 = num(context, 400);

    Value * twenty = add(context, num(context, 10), num(context, 10));
    call_standard(context, "print", add(context, int1, twenty));

    
    context->builder->get.CreateRet(context->builder->get.getInt32(0));
        */
    luv(context);
    
    llvm_run(context, false);
    return 0;

}

};
