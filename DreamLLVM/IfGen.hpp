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

    IfData * init_if(LLVMData * context, Value * drm_bool);
    void end_if(LLVMData * context, IfData * if_data);
}

//#include "IfGen.cpp"
#endif /* IfGen_hpp */
