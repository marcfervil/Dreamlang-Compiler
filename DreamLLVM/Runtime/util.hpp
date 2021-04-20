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
dreamObj * deref_var(dreamObj ** obj);

//Quick way to concatonate two strings
char * cat(const char * left, const char * right, const char * sep=NULL);

}
#endif /* util_hpp */
