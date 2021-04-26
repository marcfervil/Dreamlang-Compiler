//
//  util.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef util_hpp
#define util_hpp

#include "runtime.hpp"

extern "C" {

//char * re();

dreamObj * deref_var(dreamObj ** obj);

//Quick way to concatonate two strings
//char * dog(const char * left, const char * right, const char * sep=NULL);
char * cat(const char * left, const char * right, const char * sep=NULL);

void add_native_func(dreamObj * obj, const char * name, void * func);

int to_int(dreamObj * obj);

char * to_char(dreamObj * obj);

}
#endif /* util_hpp */
