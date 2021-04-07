//
//  main.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/15/21.
//

#include "main.hpp"
#include "standard.hpp"

#include <iostream>
#include <fstream>

extern "C" {


StructType * dreamObjTy;
PointerType * dreamObjPtrTy;
PointerType * dreamObjDoublePtrTy;
PointerType * voidPointerTy;







map<string, FunctionCallee> functions;

Type * strType;
Type * intType;
Type * voidTy ;
Type * voidPtrTy;


//expose standard.cpp functions to LLVM
void loadStandard(LLVMData* context){
    
    /*
    strType = PointerType::get(Type::getInt8Ty(context->context), 0);
    intType = IntegerType::getInt32Ty(context->context);
    voidTy =  PointerType::getVoidTy(context->context);
    voidPtrTy = Type::getInt8PtrTy(context -> context);*/
    
    
    //PointerType::get(Type::getInt8Ty(context->context), 0);
    
    //PointerType::get(Type::getInt8Ty(context->context), 0);
    
    //strType;
    //PointerType::get(PointerType::getVoidTy(context->context),0);
    
    functions["print"] = context->owner->getOrInsertFunction("print", FunctionType::get(voidTy,{intType,dreamObjPtrTy}, true));
    functions["dream_log"] = context->owner->getOrInsertFunction("dream_log", FunctionType::get(voidTy, dreamObjPtrTy, false));
    functions["printx"] = context->owner->getOrInsertFunction("printx", FunctionType::get(voidTy,{intType,strType,dreamObjPtrTy}, true));
    functions["pointer"] = context->owner->getOrInsertFunction("pointer", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy}, false));
    functions["ptr"] = context->owner->getOrInsertFunction("ptr", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy}, false));
    functions["printf"] = context->owner->getOrInsertFunction("printf", FunctionType::get(intType, strType, true));
    functions["object"] = context->owner->getOrInsertFunction("dreamObject", FunctionType::get(dreamObjPtrTy, false ));
    functions["set_var"] = context->owner->getOrInsertFunction("set_var", FunctionType::get(dreamObjPtrTy, {
        dreamObjPtrTy, strType, dreamObjPtrTy}, false ));
    functions["set_var_c"] = context->owner->getOrInsertFunction("set_var_c", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy, strType}, false ));
    functions["get_var"] = context->owner->getOrInsertFunction("get_var", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy, strType},true ));
    functions["equals_c"] = context->owner->getOrInsertFunction("equals_c", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy, dreamObjPtrTy}, false ));
    functions["contains_c"] = context->owner->getOrInsertFunction("contains_c", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy, dreamObjPtrTy}, false ));
    functions["str"] = context->owner->getOrInsertFunction("dreamStr", FunctionType::get(dreamObjPtrTy, PointerType::get(Type::getInt8Ty(context->context), 0), false));
    functions["int"] = context->owner->getOrInsertFunction("dreamInt", FunctionType::get(dreamObjPtrTy, Type::getInt32Ty(context->context), 0));
    functions["bool"] = context->owner->getOrInsertFunction("dreamBool", FunctionType::get(dreamObjPtrTy, PointerType::get(Type::getInt32Ty(context->context), 0), false));
    functions["func"] = context->owner->getOrInsertFunction("dreamFunc", FunctionType::get(dreamObjPtrTy, PointerType::get(Type::getInt8Ty(context->context), 0), false));
    functions["obj"] = context->owner->getOrInsertFunction("make_dream", FunctionType::get(dreamObjPtrTy, {voidPtrTy, dreamObjPtrTy}, false));
    //functions["test"] = context->owner->getOrInsertFunction("testing", FunctionType::get(PointerType::getVoidTy(context->context), false));
    functions["new_scope"] = context->owner->getOrInsertFunction("new_scope", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy, intType}, false));
    functions["copy"] = context->owner->getOrInsertFunction("copy", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy}, false));
    functions["deep_copy"] = context->owner->getOrInsertFunction("deep_copy", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy}, false));
    functions["shallow_copy"] = context->owner->getOrInsertFunction("shallow_copy", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy}, false));
    functions["dict"] = context->owner->getOrInsertFunction("dict", FunctionType::get(dreamObjPtrTy, false));
    functions["add_c"] = context->owner->getOrInsertFunction("add_c", FunctionType::get(dreamObjPtrTy,{dreamObjPtrTy,dreamObjPtrTy}, false));
    functions["set_parent"] = context->owner->getOrInsertFunction("set_parent", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy, dreamObjPtrTy}, false));
    functions["merge"] = context->owner->getOrInsertFunction("merge", FunctionType::get(dreamObjPtrTy, {dreamObjPtrTy, dreamObjPtrTy}, false));
    functions["ctype"] = context->owner->getOrInsertFunction("ctype", FunctionType::get(strType,  dreamObjPtrTy, false));
    functions["display"] = context->owner->getOrInsertFunction("display", FunctionType::get(voidTy, false));
    
    functions["native_int"] = context->owner->getOrInsertFunction("native_int", FunctionType::get(voidTy, {intType},false));
    functions["native_test"] = context->owner->getOrInsertFunction("native_test", FunctionType::get(voidTy, {intType}, false));
    functions["check"] = context->owner->getOrInsertFunction("check", FunctionType::get(voidTy, {dreamObjPtrTy, dreamObjPtrTy}, false));
}


//initialize llvm & return context struct
LLVMData * llvm_init(){
    //initialize llvm
    InitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    

   // llvm::sys::DynamicLibrary::LoadLibraryPermanently("lib/dream.a");
    //llvm::sys::DynamicLibrary::LoadLibraryPermanently("lib/hopes.o");
  //  llvm::sys::DynamicLibrary::LoadLibraryPermanently("dream.so");
    //llvm::sys::DynamicLibrary::LoadLibraryPermanently("lib/hopes.o");
   // llvm::sys::DynamicLibrary::LoadLibraryPermanently("lib/dream.a");
   // llvm::sys::DynamicLibrary::LoadLibraryPermanently("hopes.o");

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
    dreamObjDoublePtrTy = PointerType::get(dreamObjPtrTy, 0);
    Type * int32Type = Type::getInt32Ty(new_context -> context);
    
    //Type::getInt8PtrTy(new_context -> context)
    strType = PointerType::get(Type::getInt8Ty(new_context->context), 0);
    intType = IntegerType::getInt32Ty(new_context->context);
    voidTy =  PointerType::getVoidTy(new_context->context);
    voidPtrTy = strType;
    //PointerType::get(PointerType::getVoidTy(new_context->context),0);
    
    //Type::getInt8PtrTy(new_context -> context);
    
    dreamObjTy->setBody({
        Type::getInt8PtrTy(new_context -> context) , //const char * name;
        
        dreamObjDoublePtrTy , // dreamObj ** next;
        
        voidPtrTy, //  void * value;
       // voidPointerTy,
        //Type::getInt8PtrTy(new_context -> context),
        dreamObjPtrTy,//dreamObj * type ;
        dreamObjDoublePtrTy,// dreamObj ** first_var;
        dreamObjDoublePtrTy,//dreamObj ** last_var;
        dreamObjPtrTy, //  dreamObj * parent_scope;
        int32Type, // bool * pointer;
        ArrayType::get(dreamObjDoublePtrTy, HASHSIZE) // dreamObj ** vars [HASHSIZE];
        
    });
    
    
    //create main function & block
   
    new_context -> mainFunc = Function::Create(FunctionType::get(int32Type, {}, false), Function::ExternalLinkage, "main", new_context->module);
    new_context -> currentBlock = BasicBlock::Create(new_context -> context, "EntryBlock", new_context->mainFunc);
    
    new_context -> builder = new LLVMBuilder(new_context->currentBlock);
    
    //load standard functions
    loadStandard(new_context);
  //
    
    
    
  //  loadStandard(new_context);
   // llvm_link(new_context, "hopes.o");
    
    return new_context;
}


void llvm_inspect(LLVMData * context, const char * fileName ){
   
    std::string objectFileName(fileName);
       

    ErrorOr<std::unique_ptr<MemoryBuffer>> buffer =
      MemoryBuffer::getFile(objectFileName.c_str());

    if (!buffer) {
      // handle error
        printf("Couldnt find file ??");
    }
  
    Expected<std::unique_ptr<llvm::object::ObjectFile>> objectOrError = llvm::object::ObjectFile::createObjectFile(buffer.get()->getMemBufferRef());
    //OfbectcreateObjectFile(buffer.get()->getMemBufferRef());
  
    if (!objectOrError) {
      // handle error
        printf("????");
    }
   
    std::unique_ptr<llvm::object::ObjectFile> objectFile(std::move(objectOrError.get()));

    auto owningObject = llvm::object::OwningBinary<llvm::object::ObjectFile>(std::move(objectFile),std::move(buffer.get()));
    if( context->engine == NULL){
        printf("null engine\n");
        context -> engine = EngineBuilder(std::move(context->owner)).create();
        
        //return;
    }
    context->engine->addObjectFile(std::move(owningObject));
}

void llvm_link(LLVMData * context, const char * fileName ){
    // context->engine->add
     
    llvm::sys::DynamicLibrary::LoadLibraryPermanently(fileName);
     
  
    
    //lcontext->engine->
}

int build(LLVMData * context){
    
  //  printf("HERE???");
    
    InitializeAllTargetInfos();
      InitializeAllTargets();
      InitializeAllTargetMCs();
      InitializeAllAsmParsers();
      InitializeAllAsmPrinters();

    
  //  outs() << "CPU" << sys::getHostCPUName() << "\n";
    
    string TargetTriple =sys::getDefaultTargetTriple();
    //"aarch64-linux-android21";
    //sys::getDefaultTargetTriple();
    //"aarch64-linux-android21";
    //"armv7-none-linux-androideabi";
    //sys::getDefaultTargetTriple();
   
   // outs() << TargetTriple << "\n";
   // target
  //  sys::
    //armv7-none-linux-androideabi
    
      context->module->setTargetTriple(TargetTriple);

      std::string Error;
      auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

      // Print an error and exit if we couldn't find the requested target.
      // This generally occurs if we've forgotten to initialise the
      // TargetRegistry or we have a bogus target triple.
      if (!Target) {
        errs() << "Couldnt hit target " << Error;
        return 1;
      }

    auto CPU  = sys::getHostCPUName();
    //"x86";
    //sys::getHostCPUName();
    
      auto Features = "";

      TargetOptions opt;
      auto RM = Optional<Reloc::Model>();
      auto TheTargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
    
    
    //
    context->module->setDataLayout(TheTargetMachine->createDataLayout());

      auto Filename = "lib/dream_output.o";
      std::error_code EC;
      raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

      if (EC) {
        errs() << "Could not open file: " << EC.message();
        return 1;
      }

      legacy::PassManager pass;
      auto FileType = CGFT_ObjectFile;

      if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
      }

      pass.run(*context->module);
      dest.flush();

      outs() << "Wrote " << Filename << "\n";

    
   // outs()
    
    
    std::error_code EC2;
    raw_fd_ostream dest2("output/program_output.ll", EC2, sys::fs::OF_None);
    
    //llvm_inspect(context, "./lib/hopes_lib.so");
    dest2 <<*context->module;
    dest2.close();
    return 0;
}

//run our llvm code
void llvm_run(LLVMData * context, bool link_obj=true, bool print_module = false){
    
    context -> engine = EngineBuilder(std::move(context->owner)).create();
    //llvm_link(context, "hopes_lib.so");
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

Value * llvmStrConst(LLVMData* context, const char * value){
    return context->builder->get.CreateGlobalStringPtr(StringRef(value));
}

Value * num(LLVMData* context, int value){
    Value* builtInt = context->builder->get.getInt32(value);
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["int"], builtInt);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "int", context->currentBlock);
    return object;
}

Value * num_llvm(LLVMData* context, Value* value){
    Value* builtInt = value;
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["int"], builtInt);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "int", context->currentBlock);
    return object;
}

Value * bool_(LLVMData* context, bool value){
    Value* builtInt = context->builder->get.getInt32(value);
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "int_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["bool"], builtInt);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "int", context->currentBlock);
    return object;
}


bool isBuiltinFunc(const char * key){
    
    return !(functions.find(string(key)) == functions.end());
}

Value * get_var_llvm(LLVMData* context,  Value * scope, const char * key){
    //printf("getting var %s\n",key);
    return call_standard(context, "get_var", {scope, llvmStrConst(context, key)});
}

Value * set_var_llvm(LLVMData* context,  Value * scope, const char * key, Value * value){
    return call_standard(context, "set_var", {scope, llvmStrConst(context, key), value});
}

Value* get_value(LLVMData* context, Type * type, Value * obj );
Value * call_standard(LLVMData* context, const char * funcName, ArrayRef<Value *> args){
    //printf("calling standard %s\n",funcName);
    Value * callResult;
    if (isBuiltinFunc(funcName)){
        
        callResult = context->builder->get.CreateCall(functions[funcName], args);
    }else{
        //Value * var = call_standard(context, "get_var", {scope, llvmStrConst(context, "few")});
        //callResult = context->builder->get.CreateCall
        
        
        Value * var = get_var_llvm(context, args[0], funcName);
        //TODO: IF THE PROGRAM RANDOMLY SEGFAULTS IT WAS PROBABLY CAUSED BY THIS LINE (I didn't pass in the correct number of args, but it doesn't seem to matter...)
        FunctionType * func_ty = FunctionType::get(dreamObjPtrTy, { }, true);
        Type * func_ptr_ty = PointerType::get(func_ty, 0);
        Value * func_ptr = get_value(context, func_ptr_ty, var);
        
        callResult = context->builder->get.CreateCall(func_ty, func_ptr, args);
    }
    
    return callResult;
}

Value * call_standard_c(LLVMData* context, const char * funcName, int size, Value * c_args[size]){
    vector<Value *> args;
    for(int i=0;i<size;i++)args.push_back(c_args[i]);
    
    return call_standard(context, funcName, args);
   // return v;
}

Value * llvmInt(LLVMData* context, int value){
    return context->builder->get.getInt32(value);
}

Value * llvmStr(LLVMData* context, const char * value){
    Value * v = context->builder->get.CreateGlobalStringPtr(StringRef(value));
    return v;
}

Value * call(LLVMData* context, Value * var,  int size, Value * c_args[size]){
    vector<Value *> args;
    for(int i=0;i<size;i++)args.push_back(c_args[i]);
    
    FunctionType * func_ty = FunctionType::get(dreamObjPtrTy, true);
    Type * func_ptr_ty = PointerType::get(func_ty, 0);
    
    
    std::vector<llvm::Value*> indices(2);
    indices[0] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 0, true));
    indices[1] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 2, true));
    //var->mutateType(dreamObjPtrTy);
    
    
   // Value *valuePointer = context->builder->get.CreateGEP(dreamObjTy ,var, indices,  "GEP_STORE");

    Value *func_ptr = get_value(context, func_ptr_ty, var);
    
    
    
    
    
    //printf("cast party\n");
   //LoadInst *value = new LoadInst(func_ptr_ty,  valuePointer, "value_temp", context->currentBlock);

    
    //Value * casted_func = context->builder->get.CreatePointerCast(func_ptr, func_ptr_ty);
    
    
    
    
    Instruction * var_inst = dyn_cast<Instruction>(var);
    
    
    if(var_inst->hasMetadata()){
      
        StringRef var_args = cast<MDString>(var_inst->getMetadata("var_args")->getOperand(0))->getString();
        if(var_args=="1")args.insert(args.begin(), llvmInt(context, size));
    }
    
    
    
    return context->builder->get.CreateCall(func_ty, func_ptr, args);
    
   // return v;
}



Value * str(LLVMData* context, const char * value/*, const char * name="str"*/){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "str_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["str"], context->builder->get.CreateGlobalStringPtr(StringRef(value)));
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "str", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}






LoadInst * func_init(LLVMData* context, Value * value){
    //printf("THIS IS CAST\n");
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "func_stack", context->currentBlock);
    
    Value * casted_func = context->builder->get.CreatePointerCast(value, voidPtrTy);
    
    Value * callResult = context->builder->get.CreateCall(functions["func"], casted_func);
    //printf("casted");
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "func", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}

LoadInst * obj_init(LLVMData* context, Value * value){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "obj_stack", context->currentBlock);
    

    Value * obj_type = context->owner->getOrInsertGlobal("dreamObjType", intType);
    
    Value * casted_func = context->builder->get.CreatePointerCast(value, voidPtrTy);
    Value * casted_type = context->builder->get.CreatePointerCast(obj_type, dreamObjPtrTy);
    
    Value * callResult = context->builder->get.CreateCall(functions["obj"], {casted_func, casted_type});
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "obj", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}

LoadInst * null_obj_init(LLVMData* context){
    
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "obj_stack", context->currentBlock);
    Value * callResult = context->builder->get.CreateCall(functions["object"]);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "obj", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}

Value * func_init_value(LLVMData* context, Value * value){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "func_stack", context->currentBlock);
    
    Value * casted_func = context->builder->get.CreatePointerCast(value, voidPtrTy);
    
   // Value * callResult = context->builder->get.CreateCall(functions["func"], casted_func);
    
    Value * callResult = context->builder->get.CreateCall(functions["func"], casted_func);
    new StoreInst(callResult, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "func", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}

Value * load_store(LLVMData* context, Value * value){
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "func_stack", context->currentBlock);
    new StoreInst(value, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "func", context->currentBlock);
    
  
  //  context->builder->get.CreateCall(functions["printf"], load2);
    return object;
}

Value * log_llvm(LLVMData * context, Value * value){
    
    return call_standard(context, "print", {llvmInt(context, 1), value});
}


Value * dict_llvm(LLVMData * context, Value * value){
    
    return call_standard(context, "dict", {value});
}

FuncData * func(LLVMData* context, Value* obj, const char * funcName, bool is_class, int arg_size, const char * arg_names[arg_size]){

    vector<Type *> args = {dreamObjPtrTy};
    std::vector<Metadata*> meta_args;
    
    for(int i=0;i<arg_size;i++)args.push_back(dreamObjPtrTy);
    
  
    
    //intitilize function data struct pointer and set starting block so we know what our preivous function is
    Function *new_func = Function::Create(FunctionType::get(dreamObjPtrTy, args, false), Function::ExternalLinkage, funcName, context->module);
    FuncData * func_data = new FuncData(new_func);
    func_data -> startingBlock = std::move((context->currentBlock));
    func_data -> is_class = is_class;
    
    //saving the arg names into metada was unnecessary in hindsight, but it might come in handy later...
    for(int i=0;i<arg_size;i++)meta_args.push_back(MDString::get(new_func->getContext(), arg_names[i]));
    new_func->setMetadata("arg_names", MDNode::get(new_func->getContext(),  meta_args));
    
    //name scope arg to prevent seggy
    Argument *context_arg = &*new_func->arg_begin();
    context_arg->setName("scope");

    
    //create and enter method block
    context -> currentBlock = BasicBlock::Create(context -> context, "EntryBlock", new_func);
    context->builder->get.SetInsertPoint(context->currentBlock);
    
    //save arguments into scope "obj"
    int i = 0;
    
    for (Argument& arg : new_func->args()) {
        //skip first context argument because we want the value of the variables (for now...)
        if(i==0){
            i = 1;
            continue;
        }
        
        //get and store the value of each argument and set the name respectively
        llvm::AllocaInst* alloc = context->builder->get.CreateAlloca(dreamObjPtrTy, 0, "alloctmp");
        new StoreInst(&arg, alloc, context->currentBlock);
        LoadInst * arg_ref = new LoadInst(dreamObjPtrTy, alloc, "varName", context->currentBlock);
        (&arg)->setName(arg_names[(i++)-1]);
        
        //log_llvm(context, arg_ref);
        //printf("setting params\n");
        
        //set_var_llvm(context, context_arg, a rg_names[i-2], arg_ref);
        set_var_llvm(context, context_arg, arg_names[i-2], arg_ref);
        
    }
    
    //store function name & scope so they can be used outside of this function
    func_data -> scope = load_store(context, context_arg);
    func_data -> name = (new std::string(funcName))->c_str();
  
    //if(is_class)log_llvm(context, get_var_llvm(context, context_arg, "x"));
   // if(is_class)dict_llvm(context, context_arg);
    //dict_llvm(context, get_var_llvm(context, context_arg, "parent"));
    return func_data;
}

void end_func(LLVMData* context, Value * scope, FuncData * func_data){
    
    //retVal(context, str(context, "nada"));
    context->builder->get.SetInsertPoint(func_data->startingBlock);
    context->currentBlock = func_data->startingBlock;
    Value * funcPointer;
   
    funcPointer = (func_data->is_class) ?  obj_init(context, func_data->func) : func_init_value(context, func_data->func);
 
    //printf("yuh");
    set_var_llvm(context, scope, func_data->name, funcPointer);
    set_var_llvm(context, funcPointer, "@context", scope);
    
    if(func_data->is_class){
     //   Value * set_parent_c(LLVMData* context, Value* obj, Value* new_parent){
       // set_parent_c(context, get_var_llvm(context, funcPointer, "@context"), get_var_llvm(context, scope, "null"));
    }
                                                                                                
    //set_var_llvm(context, funcPointer, "@context", scope);

}

Value * save(LLVMData* context, Value* obj, const char * varName, Value * value){
    return call_standard(context, "set_var", {obj, llvmStrConst(context, varName), value} );
}


Value * load(LLVMData* context, Value* obj, const char * varName, bool from_parent ){
    //log_llvm(context, llvmStr(context, "dd"));
    
    if(isBuiltinFunc(varName)){
     //   log_llvm(context, str(context, "woow"));
        //printf("got builtin %s\n", varName);
        FunctionCallee func = functions[varName];
        LoadInst * func_inst = func_init(context, func.getCallee());
        
        //set_var_llvm(context, func, "var_arg", bool_(context, functions[varName].getFunctionType()->isVarArg()));
        LLVMContext& func_context = func_inst->getContext();
        MDNode* metadata = MDNode::get(func_context, MDString::get(func_context, to_string(func.getFunctionType()->isVarArg())));
        
        func_inst->setMetadata("var_args",  metadata);
        
        return func_inst;
    }
    //printf("loading var %s : %d\n",varName,from_parent);
    //return call_standard(context, "get_var",  {obj, llvmStrConst(context, varName)} ) ;
     
    return (from_parent) ? call_standard(context, "get_var",  {obj, llvmStrConst(context, varName)} ) : call_standard(context, "get_var",  {obj, llvmStrConst(context, varName), llvmInt(context, from_parent)} );
}

Value * init_scope(LLVMData* context, Value* scope, int nested_scope){
  
    Value * res = call_standard(context, "new_scope", {scope, llvmInt(context, nested_scope)});
    
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "scope_stack", context->currentBlock);
    
    new StoreInst(res, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "new_scope", context->currentBlock);
    //call_standard(context, "print", res);
    //call_standard(context, "print", str(context,"lowkey"));
    //call_standard(context, "print", load(context, res, "dog"));
    return object;
}

void set_line(LLVMData* context, int line){
    
    new StoreInst(llvmInt(context, line), context->owner->getOrInsertGlobal("line", intType), context->currentBlock);
    
   // LoadInst * object = new LoadInst(intType, context->owner->getOrInsertGlobal("line", intType), "line_num", context->currentBlock);
   // call_standard(context, "printf", {llvmStr(context, "%d"), object});
    //new StoreInst(context->owner->getOrInsertGlobal("line", intType), llvmInt(context, line), context->currentBlock);
}

Value * set_parent_c(LLVMData* context, Value* obj, Value* new_parent){
  
    Value * res = call_standard(context, "set_parent", {obj, new_parent});
    
    Value *objStore = new AllocaInst(dreamObjPtrTy, 0, "parent_stack", context->currentBlock);
    
    new StoreInst(res, objStore, context->currentBlock);
    LoadInst * object = new LoadInst(dreamObjPtrTy, objStore, "new_parentee", context->currentBlock);
    //call_standard(context, "print", res);
    //call_standard(context, "print", str(context,"lowkey"));
    //call_standard(context, "print", load(context, res, "dog"));
    return object;
}



Type * int_type(LLVMData* context){
    
    return Type::getInt32Ty(context->context);
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
    Value *valuePointer = context->builder->get.CreateGEP( obj, indices,  "memberptr");
    
    //context->builder->get.GEP
    Value * load_val = new LoadInst(voidPtrTy,  valuePointer, "value_temp", context->currentBlock);
  
    Value * casted_value = context->builder->get.CreatePointerCast(load_val, type);
    
  //  valuePointer->mutateType(type);
    //load
    //%dreamObj* (...)*,
    //%dreamObj* (...)*
    
   // LoadInst *value = new LoadInst(casted_value->getType(),  casted_value, "value_temp", context->currentBlock);
    

    return casted_value;
}

LoadInst* get_pointer_value(LLVMData* context, Type * type, Value * obj ){
    std::vector<llvm::Value*> indices(2);
    indices[0] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 0, true));
    //2 is passed in because the value struct member is the 3rd member
    indices[1] = llvm::ConstantInt::get(context->context, llvm::APInt(32, 2, true));
    Value *valuePointer = context->builder->get.CreateGEP(obj, indices,  "memberptr");

    LoadInst *value = new LoadInst(PointerType::get(type, 0), valuePointer, "temp", context->currentBlock);

    return new LoadInst(type, value, "temp", context->currentBlock);
}




Value * add(LLVMData* context, Value *var1, Value *var2){
    
    /*
    Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
    Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
    return numVal(context, context->builder->get.CreateAdd(value1, value2));*/
    Value * result = call_standard(context, "add_c", {var1, var2});
    return result;
    //return context->builder->get.CreateAdd(value1, value2);
}

Value * mul(LLVMData* context, Value *var1, Value *var2){
    Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
    Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
    return numVal(context, context->builder->get.CreateMul(value1, value2));
    //return context->builder->get.CreateAdd(value1, value2);
}

Value * divi(LLVMData* context, Value *var1, Value *var2){
    Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
    Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
    //return numVal(context, context->builder->get.CreateSDiv(value1, value2));
    return numVal(context, context->builder->get.CreateSDiv(value1, value2));
    //return context->builder->get.CreateAdd(value1, value2);
}

Value * sub(LLVMData* context, Value *var1, Value *var2){
    Value* value1 = get_pointer_value(context, Type::getInt32Ty(context->context), var1);
    Value* value2 = get_pointer_value(context, Type::getInt32Ty(context->context), var2);
    return numVal(context, context->builder->get.CreateSub(value1, value2));
    //return context->builder->get.CreateAdd(value1, value2);
}



Value * equals(LLVMData* context, Value *var1, Value *var2){
    return  call_standard(context, "equals_c", {var1, var2});
}

Value * contains(LLVMData* context, Value *var1, Value *var2){
    return  call_standard(context, "contains_c", {var1, var2});
}

Value * retVal(LLVMData* context, Value * value ){

    
   

    return  context->builder->get.CreateRet(value);;
}

Value * funcScope(FuncData * data){
    return data -> scope;
}


int main(){
    //dreamObj * heyo = dreamStr("hey");
    
   // printx(4,"sdx","new format",2001,heyo);
   // return 0;
    LLVMData * context = llvm_init();
   Value * scope = str(context, "wokd");
    
    //log_llvm(context, scope);
    
  //  llvm_inspect(context, "hopes_lib.so");
    //hopes_lib.so
    
    FuncData *new_func2 = func(context, scope, "cat", 0, false, new const char * []{});
    
   
    
        retVal(context, str(context,"cat return"));
   
    
    end_func(context, scope, new_func2);
    
    //Value * home = call_standard_c(context, "cat", 1, new Value*[]{init_scope(context, scope,1)});
   
    
    // context->builder->get.CreateRet(context->builder->get.getInt32(0));

    context->builder->get.CreateRet(context->builder->get.getInt32(0));
    llvm_run(context, true, false);
    //build(context);
    return 0;

}

};
