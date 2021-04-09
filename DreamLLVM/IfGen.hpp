//
//  IfGen.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/28/21.
//

#ifndef IfGen_hpp
#define IfGen_hpp


#include "main.hpp"


extern "C"{
#include <stdio.h>
    using namespace llvm;
    using namespace std;

    typedef struct IfData{
        
        BasicBlock *then;
        BasicBlock* ifcont;
        
    } IfData;


    typedef struct ForData{
        
        BasicBlock *then;
        BasicBlock* forcont;
        
    } forData;

    IfData * init_if(LLVMData * context, Value * drm_bool);
    void end_if(LLVMData * context, IfData * if_data, bool has_return);

    ForData * init_for(LLVMData * context, const char * var_name, Value * cond);
    void end_for(LLVMData * context, forData * for_data, bool has_return);
}

#include "IfGen.cpp"
#endif /* IfGen_hpp */
