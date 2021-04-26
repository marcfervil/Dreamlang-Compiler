//
//  dreamStr.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef dreamStr_hpp
#define dreamStr_hpp

#include "runtime.hpp"

extern "C" {
/*Initialize dream str object**/
dreamObj * dreamStr(const char * value);

/*String concatenation**/
dreamObj * str_add(dreamObj * me, dreamObj * other);

/*String comparison**/
dreamObj * str_equals(dreamObj * me, dreamObj * other);

/*get character from string**/
dreamObj * str_get(dreamObj * self, dreamObj * index);

dreamObj * str_len(dreamObj * self);

}
#endif /* dreamStr_hpp */
