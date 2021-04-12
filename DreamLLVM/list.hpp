//
//  list.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/8/21.
//

#ifndef list_hpp
#define list_hpp

#include <stdio.h>

extern "C"{

dreamObj * list_get(dreamObj * self, dreamObj * index);
dreamObj * list_rep(dreamObj * self);

dreamObj * list_push(dreamObj * scope, dreamObj * index);
dreamObj * iter_next(dreamObj * scope);
}

#include "list.cpp"

#endif /* list_hpp */
