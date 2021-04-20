//
//  dreamInt.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef dreamInt_hpp
#define dreamInt_hpp


#include "runtime.hpp"

/*Check if two nums are equal**/
dreamObj * num_equals(dreamObj * me, dreamObj * other);

/*add two nums**/
dreamObj * num_add(dreamObj * me, dreamObj * other);

/*initialize dreamInt**/
dreamObj * dreamInt(int value);


#endif /* dreamInt_hpp */
