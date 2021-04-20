//
//  dreamStr.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef dreamStr_hpp
#define dreamStr_hpp

#include "runtime.hpp"

/*Initialize dream str object**/
dreamObj * dreamStr(const char * value);

/*String concatination**/
dreamObj * str_add(dreamObj * me, dreamObj * other);

/*String comparison**/
dreamObj * str_equals(dreamObj * me, dreamObj * other);

#endif /* dreamStr_hpp */
