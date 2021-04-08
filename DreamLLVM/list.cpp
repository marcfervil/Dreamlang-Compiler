//
//  list.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/8/21.
//


#ifndef standard_hpp
#define HASHSIZE 1022
#include "standard.hpp"
#endif

extern "C"{

    dreamObj * dreamList(int num_args, ...){
       // printf("ddd");
        va_list valist;
        va_start(valist, num_args);
        
     
        for (int i = 0; i < num_args; i++) {
            const char * str= rep(va_arg(valist, dreamObj *));
            android_log(str);
            printf("List arg: %s\n",  str);
        }

       
        va_end(valist);
        return nullDream;
    }


}
