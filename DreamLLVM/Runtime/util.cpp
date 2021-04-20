//
//  util.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "util.hpp"

extern "C"{


char * cat(const char * left, const char * right, const char * sep){
    char * str;
    if(sep == NULL)
        asprintf(&str, "%s%s", left, right);
    else
        asprintf(&str, "%s%s%s", left, sep, right);
    return str;
}


dreamObj * deref_var(dreamObj ** obj){
    if(obj==NULL){
     //   printf("NADDA");
        
    }
    return (obj==NULL) ? NULL : *obj;
}

}
