//===-- examples/HowToUseJIT/HowToUseJIT.cpp - An example use of the JIT --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This small program provides an example of how to quickly build a small
//  module with two functions and execute it with the JIT.
//
// Goal:
//  The goal of this snippet is to create in the memory
//  the LLVM module consisting of two functions as follow:
//
// int add1(int x) {
//   return x+1;
// }
//
// int foo() {
//   return add1(10);
// }
//
// then compile the module via JIT, then execute the `foo'
// function and return result to a driver, i.e. to a "host program".
//
// Some remarks and questions:
//
// - could we invoke some code using noname functions too?
//   e.g. evaluate "foo()+foo()" without fears to introduce
//   conflict of temporary function name with some real
//   existing function name?
//
//===----------------------------------------------------------------------===//

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



extern "C" {


    #define HASHSIZE 101

typedef struct dreamObj{
    
        
       
        char * name;
        dreamObj *next;
      
        
        
        void * value;
        
        dreamObj * vars [HASHSIZE];
        dreamObj * type = NULL;
    
        // etc..
    } dreamObj;
 
/*
    int tester(dreamObj* f){
        printf("LLVM To C function: %s\n", f->a);
        return  0;
    }*/

       
   // dreamObj * global_scope = ;


    dreamObj * nullDream = NULL;

    dreamObj * make_dream(void * value, dreamObj * type = nullptr);
    dreamObj * dreamStrType = make_dream((void *)"<Str Type>");
    dreamObj * dreamIntType = make_dream((void *)"<Int Type>");

    dreamObj * dreamType = make_dream(nullptr);


    dreamObj * make_dream(void * value, dreamObj * type){
        if (type == nullptr)type = nullDream;
        dreamObj *new_obj;
        
        new_obj = (dreamObj *) malloc(sizeof(struct dreamObj));
        new_obj -> value = (value);
        new_obj -> type = type;
      //  new_obj->name = 404;
        
        for(int i=0; i<HASHSIZE; i++){
            new_obj -> vars[i] = NULL;
        }
        
        return new_obj;
    }




dreamObj * dreamStr(const char * value){
    //printf("NEW STRING: %s",value);
   // const char * v
   // void * s = static_cast<void* >(value);
    dreamObj * obj = make_dream((void *)value, dreamStrType);
    return obj;
}



//dreamObj * dreamType = make_dream(nullptr);
//dreamObj * dreamNull = make_dream(nullptr);






  

    void print(dreamObj* obj, bool raw = false){
        if (obj == NULL){
            printf("[Dream]: <Undefined> \n");
            return;
        }
        if(obj->type == dreamStrType || obj->type == dreamType){
            if(raw){
                printf("[Dream]: %s\n", (char *)obj->value);
            }else{
                printf("%s\n", (char *)obj->value);
            }
        }else{
            printf("[Dream]: didn't print type \n");
         //   print(obj->type);
        }
    }
    
  //  static struct dreamObj *hashtab[HASHSIZE];


    unsigned hash_obj(const char *s)
    {
        unsigned hashval;
        for (hashval = 0; *s != '\0'; s++)
          hashval = *s + 31 * hashval;
        return hashval % HASHSIZE;
    }

    dreamObj * get_var(dreamObj * obj, const char *s)
    {
        dreamObj *np;
      //  outs() << hash_obj(s) ;
        dreamObj * tt = obj->vars[hash_obj(s)];
        
     //   tt->name;
        //if(!(tt->name)){
        
        
        for (np = obj->vars[hash_obj(s)]; np!=NULL; np = np->next){
           // dreamObj * t = obj->vars[hash_obj(s)];
            
            if (np->name != NULL && strcmp(s, np->name) == 0){
                return np; // found
            }
        }
        return NULL; // not found
    }


    struct dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value)
    {
        dreamObj *np;
        unsigned hashval;
        if ((np = get_var(obj, name)) == NULL) { // not found
            np = ( dreamObj *) malloc(sizeof(*np));
            if (np == NULL || (np->name = strdup(name)) == NULL){
               
              return NULL;
            }
            hashval = hash_obj(name);
            value -> next = obj->vars[hashval];
            value ->name = (char *)name;
          //  printf("good check %d\n",hashval);
            obj->vars[hashval] = value;
        } else //already there
            free((void *) np->value); //free previous defn
        if ((np->value = value) == NULL){
            printf("null check");
           return NULL;
        }
        return np;
    }

}

int main() {
 
   
    /*
    //for(int i = 0; i<2000000; i++){
        dreamObj * r = dreamStr("test");
        set_var(r, "wow", dreamStr("wooah"));
        print(get_var(r, "wow"));

        delete r;
 //   }
    
    return 0;*/
    
    
  InitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();

  LLVMContext Context;
    
   
    
    //char * ij ="rr";
    //int a =30;
//    dreamObj * global_scope = make_dream((void *)"dd");

  //  dreamObj * str = dreamStr("hello");
   // set_var(str, "teskkt", dreamStr("world"));
   // print(str);
    
  //  printf("Test: %s", (char *)global_scope->value);
    
    
    
    //outs() << "string stuff: hh" << ((char *)test-> value);
    
  // Create some module to put our function into it.
  std::unique_ptr<Module> Owner = std::make_unique<Module>("testing", Context);
  Module *M = Owner.get();
    /*
    vector<Type*> members ;
    members.push_back( Type::getInt32Ty(Context) );
    StructType *const llvm_S = StructType::create( Context, "obj" );
       llvm_S->setBody( members );*/
    
  // Create the add1 function entry and insert this entry into module M.  The
  // function will have a return type of "int" and take an argument of "int".
  
  
/*
  Function *Add1F =
      Function::Create(FunctionType::get(Type::getInt32Ty(Context),
                                         {Type::getInt32Ty(Context)}, false),
                       Function::ExternalLinkage, "add1", M);*/
    /*
    typedef struct dreamObj
    {
  
        dreamObj *next;
        const char * name;
        
        void * value;
        
    
        // etc..
    } dreamObj;*/
    
    PointerType* VoidPointerTy = PointerType::get(PointerType::getVoidTy(Context), 0);
    PointerType* StructPointerTy = PointerType::get(StructType::get(Context), 0);
    
    //auto myStructType = StructType::create(Context, "myStruct"); // Create opaque type
    //auto myStructPtrType = PointerType::get(myStructType, 0); // Initialise the pointer type now
    //myStructType->setBody({ intType, myStructPtrType }, /* packed */ false); // Set the body of the aggregate
    
    auto intType = IntegerType::get(Context, 32); // 32 bits integer
    auto myStructType = StructType::create(Context, "dreamObjRef"); // Create opaque type
    auto myStructPtrType = PointerType::get(myStructType, 0);
    
    /*
    StructType* obj = StructType::create("dreamObj"
                                   
                                         ,Type::getInt32Ty(Context)
                                         ,StructType::get(Context)
                       
                                        
                                         ,Type::getInt8PtrTy(Context)
                    
                                         );*/
   
    /*typedef struct dreamObj{
        
            
           
            int name;
            dreamObj *next;
          
            
            
            void * value;
            
       //     dreamObj * vars [HASHSIZE];
       //    dreamObj * type;
        
            // etc..
        } dreamObj;*/
    
    StructType * obj = StructType::create(Context, "dreamObj"); // Create opaque type
    PointerType * objPtrType = PointerType::get(obj, 0); // Initialise the pointer type now
   // obj->setBody({ Type::getInt32Ty(Context), objPtrType, Type::getInt8PtrTy(Context) }, /* packed */ false); // Set the body of the aggregate
   
    obj->setBody({ Type::getInt32Ty(Context) , objPtrType , VoidPointerTy }, /* packed */ false);
    
   // obj = Owner->getTypeByName("dream");
  //  Type* obj = Owner->getOrInsertGlobal("dream", objs)->getType();
   // Owner->getOr
    // Create a basic block builder with default parameters.  The builder will
    // automatically append instructions to the basic block `BB'.

      
      
    // Get pointers to the constant `1'.

    
   
    
    Function *Add1F =
        Function::Create(FunctionType::get(obj, {PointerType::get(obj,0)}, false),
                         Function::ExternalLinkage, "add1", M);

   // at->
    
    // Add a basic block to the function. As before, it automatically inserts
    // because of the last argument.
    BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", Add1F);

  // Create a basic block builder with default parameters.  The builder will
  // automatically append instructions to the basic block `BB'.
  IRBuilder<> builder(BB);
    
    
  // Get pointers to the constant `1'.
  Value *One = builder.getInt32(1);
   // new AllocaInst(obj, "var_name", BB);
  //  Instruction* ptr_value1 = GetElementPtrInst::Create(ptr_rt, ptr_value1_indices, "value1", label_entry);
    
    
  // Get pointers to the integer argument of the add1 function...
  assert(Add1F->arg_begin() != Add1F->arg_end()); // Make sure there's an arg
  Argument *ArgX = &*Add1F->arg_begin();          // Get the arg
  ArgX->setName("obj");            // Give it a nice symbolic name for fun.

  // Create the add instruction, inserting it into the end of BB.
  //Value *Add = builder.CreateAdd(One, ArgX);

    Value *Add = builder.getInt32(30);
    
    /*
    std::vector<llvm::Value*> indices(2);
    indices[0] = llvm::ConstantInt::get(Context, llvm::APInt(32, 0, true));
    indices[1] = 0;
    llvm::AllocaInst* alloc = builder.CreateAlloca(obj, 0, "alloctmp");
    Value *alloc = new AllocaInst(obj, 0, "alloctmp", builder);
    builder.CreateStore(One, alloc);
   // builder.CreateLoad
    llvm::Value* member_ptr = builder.CreateGEP(obj, alloc, indices, "memberptr");
    llvm::Value* loaded_member = builder.CreateLoad(member_ptr, "loadtmp");*/
    
   // Value *Tester = new AllocaInst(obj, 0, "dreamint", BB);
    FunctionCallee print = Owner->getOrInsertFunction("print",
                                        FunctionType::get(
                                                          PointerType::getVoidTy(Context), PointerType::get(obj, 0), false
                                )
                                                       );
    
    FunctionCallee printeff = Owner->getOrInsertFunction("printf",
                                     FunctionType::get(IntegerType::getInt32Ty(Context), PointerType::get(Type::getInt8Ty(Context), 0), true /* this is var arg func type*/)
                                     );
                                                       
    FunctionCallee init_dream = Owner->getOrInsertFunction("make_dream",
                                                          
                                                       FunctionType::get(PointerType::get(obj, 0),
                                                                         VoidPointerTy,
                                                                         false )
                                                       );
    
    FunctionCallee init_dreamStr = Owner->getOrInsertFunction("dreamStr",
                                                          
                                                       FunctionType::get(PointerType::get(obj, 0),
                                                                         PointerType::get(Type::getInt8Ty(Context), 0),
                                                                         false )
                                                       );
    

    
    //Owner->getO
    
   // Tester->getType()->print(outs());
    outs() << "\n";
    obj->print(outs());
    outs() << "\n";
    
    {
    llvm::AllocaInst* alloc = builder.CreateAlloca(objPtrType, 0, "alloctmp");
    StoreInst *str2 = new StoreInst(ArgX, alloc, BB);
    LoadInst * lod = new LoadInst(objPtrType, alloc, "v3", BB);
    std::vector<llvm::Value*> indices(2);
        
        LoadInst *fin = nullptr;
    {
        /*
        int index = 2;
        indices[0] = llvm::ConstantInt::get(Context, llvm::APInt(32, 0, true));
        indices[1] = llvm::ConstantInt::get(Context, llvm::APInt(32, index, true));
        Value *valuePointer = builder.CreateGEP(alloc, indices,  "memberptr2");
        LoadInst *load3 = new LoadInst(obj, valuePointer, "v2", BB);*/
        
     //   LoadInst *load2 = new LoadInst(obj, alloc, "v2", BB);
      //  fin = load2;
        //builder.CreateGlobalStringPtr(StringRef("hi"))
     //   new StoreInst(builder.getInt32(69), valuePointer, BB);
        
        
        builder.CreateCall(print, lod);
    }
        
        
        
    {
        
        int index = 2;
        indices[0] = llvm::ConstantInt::get(Context, llvm::APInt(32, 0, true));
        indices[1] = llvm::ConstantInt::get(Context, llvm::APInt(32, index, true));
        Value *valuePointer = builder.CreateGEP(lod, indices,  "memberptr3");
        LoadInst *load2 = new LoadInst(Type::getInt8PtrTy(Context), valuePointer, "v3", BB);
        builder.CreateCall(printeff, load2);
        //builder.CreateCall(tester, load2);
       // builder.CreateCall(tester, alloc);
       
    }
    
        
   
  //  Value *valuePointer = builder.CreateGEP(alloc, indices,  "memberptr");
   //load->
   //     LoadInst *load2 = new LoadInst(Type::getInt32Ty(Context), valuePointer, "v2", BB);
    
   // Value *valuePointer = builder.CreateGEP(alloc, indices,  "memberptr");
   // LoadInst *str2 = new LoadInst()
   //LoadInst(Type::getInt32Ty(Context), valuePointer, "fwe", BB);
    
 //   Value *d = builder.CreateStore(ArgX, alloc);
   // builder.CreateGEP(obj, alloc, indices, "memberptr");
    //GetElementPtrInst::Create(obj, ArgX, indices, "dfkdjf", BB);

  // Create the return instruction and add it to the basic block
        builder.CreateRet(fin);
    }
  // Now, function add1 is ready.

  // Now we're going to create function `foo', which returns an int and takes no
  // arguments.
  Function *FooF =
      Function::Create(FunctionType::get(Type::getInt32Ty(Context), {}, false),
                       Function::ExternalLinkage, "foo", M);

  // Add a basic block to the FooF function.
  BB = BasicBlock::Create(Context, "EntryBlock", FooF);

  // Tell the basic block builder to attach itself to the new basic block
  builder.SetInsertPoint(BB);

  // Get pointer to the constant `10'.
 // Value *Ten = builder.getInt32(10);
    
    //auto x = new AllocaInst(obj, "var_name", &BB);
    
    
    
    Value *Ten = new AllocaInst(objPtrType, 0, "dreamint", BB);
    
   // StoreInst *str = new StoreInst(builder.getInt32(69), Ten, BB); // i -> current instruction pointer which represents %add ( source of store instruction ), pa -> destination. i.e., temp1
    Value * num = builder.getInt32(69);
    
    //Value * casted_num =  llvm::CastInst::CreatePointerCast(num, VoidPointerTy, "intvoid", BB);
   // Value * tt = builder.CreateCall(init_dreamStr, builder.CreateGlobalStringPtr(StringRef("we???")));
    Value * tt = builder.CreateCall(init_dreamStr, builder.CreateGlobalStringPtr(StringRef("IT WORKS!!")));
    //builder.CreateGlobalStringPtr(StringRef("(Love laiojfweojfewofjefworugh?)"))->getType()->print(outs());
    Value * st =  new StoreInst(tt, Ten, BB);
    LoadInst * lod = new LoadInst(objPtrType, Ten, "v3", BB);
    
     CallInst *Add1CallRes = builder.CreateCall(Add1F, lod);
     Add1CallRes->setTailCall(true);

    
    //builder.CreateCall(print, lod);
    
    /*
    int index = 0;
    
    std::vector<llvm::Value*> indices(2);
   
    indices[0] = llvm::ConstantInt::get(Context, llvm::APInt(32, 0, true));
    indices[1] = llvm::ConstantInt::get(Context, llvm::APInt(32, index, true));
    Value *valuePointer = builder.CreateGEP(Ten, indices,  "memberptr");
    Value * strr = builder.CreateGlobalStringPtr(StringRef("Hello, world!"));
    new StoreInst(strr, valuePointer, BB);
    
    
    
    std::vector<llvm::Value*> indices2(2);
    indices2[0] = llvm::ConstantInt::get(Context, llvm::APInt(32, 0, true));
    indices2[1] = llvm::ConstantInt::get(Context, llvm::APInt(32, index+1, true));
    Value *valuePointer2 = builder.CreateGEP(Ten, indices2,  "memberptr");
    new StoreInst(builder.getInt32(300),valuePointer2, BB);*/
   
    /*
     LoadInst *load2 = new LoadInst(Type::getInt32Ty(Context), valuePointer, "v2", BB);
    builder.CreateRet(load2);*/
    
   // LoadInst *load2 = new LoadInst(obj, Ten, "v3", BB);
    
    // builder.CreateG
   // StoreInst *str2 = new StoreInst(Ten,  builder.getInt32(2), BB);
    //  BB->getInstList().insert(FooF, str);
  //  BB->moveBefore(BasicBlock *MovePos)
    //bulder.get
    //builder.get
    
    //builder.struc

//    Value *Ten2 =  builder.CreateInBoundsGEP(Ten*, llvm::ArrayRef<llvm::Value*>, llvm::Twine);


  // Pass Ten to the call to Add1F
   // Value * str  = builder.CreateGlobalStringPtr(StringRef("Hello, world!"));
   // builder.CreateCall(print, str);
    
    
 // CallInst *Add1CallRes = builder.CreateCall(Add1F, Ten);
  //Add1CallRes->setTailCall(true);

  // Create the return instruction and add it to the basic block.
  builder.CreateRet(num);

  // Now we create the JIT.
  ExecutionEngine* EE = EngineBuilder(std::move(Owner)).create();

  outs() << "We just constructed this LLVM module:\n\n" << *M;
  outs() << "\n\nRunning foo: ";
  outs().flush();

  // Call the `foo' function with no arguments:
  std::vector<GenericValue> noargs;
   
  GenericValue gv = EE->runFunction(FooF, noargs);

  // Import result of execution:
  outs() << "Result: " << gv.IntVal << "\n";

  delete EE;
  llvm_shutdown();
  return 0;
}
