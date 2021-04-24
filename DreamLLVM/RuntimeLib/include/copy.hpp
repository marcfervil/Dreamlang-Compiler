//
//  copy.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef copy_hpp
#define copy_hpp

#include "runtime.hpp"

extern "C"{
dreamObj* copy(dreamObj * obj);
dreamObj* medium_copy(dreamObj * obj);
dreamObj * smart_copy(dreamObj * obj);
dreamObj * shallow_copy(dreamObj * obj, bool parent=true);
dreamObj * deep_copy(dreamObj * obj);


/*Copy the value of a dreamObj * (this is used during a deep_copy)**/
void * copy_value(void * value, dreamObj *type);
}
#endif /* copy_hpp */
