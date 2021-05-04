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

/*Initialize dream func with function context**/
dreamObj * dreamFuncWithContext(void * value, dreamObj * context);

typedef dreamObj* (*dreamCall) (...) ;
dreamCall to_call(dreamObj * obj);
}
#endif /* dreamFunc_hpp */
