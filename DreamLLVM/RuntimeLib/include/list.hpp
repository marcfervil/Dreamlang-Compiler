//
//  list.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/8/21.
//

#ifndef list_hpp
#define list_hpp

#include "runtime.hpp"

extern "C"{

/*Initialize dream list**/
dreamObj * dreamList(int num_args, ...);

dreamObj * list_get(dreamObj * self, dreamObj * index);
dreamObj * list_rep(dreamObj * self);

dreamObj * list_push(dreamObj * scope, dreamObj * index);
dreamObj * count_iter_next(dreamObj * scope);
dreamObj * list_set(dreamObj * scope, dreamObj * index, dreamObj * value);

dreamObj * list_push(dreamObj * scope, dreamObj * new_item);

dreamObj * count_iter(dreamObj * scope);
}



#endif /* list_hpp */
