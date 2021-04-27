//
//  dreamFunc.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef dreamFunc_hpp
#define dreamFunc_hpp

#include "runtime.hpp"

extern "C" {
/*Initialize dream function**/
dreamObj * dreamFunc(void * value);
dreamObj * dreamFuncWithContext(void * value, dreamObj * context);
}
#endif /* dreamFunc_hpp */
