//
//  main.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/15/21.
//

#ifndef main_hpp
#define main_hpp

#include <stdio.h>
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
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#define HASHSIZE 1022
extern "C"{


    using namespace llvm;
    using namespace std;


    typedef struct FuncData{
        Function *func;
        BasicBlock *startingBlock;
        Value * scope;
        const char * name;
        bool is_class;
        FuncData(Function *init_func):func(init_func){};
    } FuncData;

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

    Value * call_standard(LLVMData* context, const char * funcName, ArrayRef<Value *> args);

    LoadInst* get_pointer_value(LLVMData* context, Type * type, Value * obj );

    Value * llvmInt(LLVMData* context, int value);
    Value * retVal(LLVMData* context, Value * value );
    void llvm_link(LLVMData * context, const char * fileName );
    Value * set_parent_c(LLVMData* context, Value* obj, Value* new_parent);
}
#include "IfGen.hpp"
#include "Natives.hpp"

#endif /* main_hpp */
