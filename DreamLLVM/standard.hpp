//
//  standard.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/15/21.
//

#ifndef standard_hpp
#define standard_hpp

#include <stdio.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
#include <unistd.h>
#define HASHSIZE 101
extern "C" {
    typedef struct dreamObj{
        
        
       
        char * name;
        dreamObj *next;
        
        void * value;
        
     //
        dreamObj * type ;
        dreamObj * vars [HASHSIZE];
        // etc..
    } dreamObj;


    dreamObj * make_dream(void * value, dreamObj * type = nullptr);
}
#endif /* standard_hpp */




