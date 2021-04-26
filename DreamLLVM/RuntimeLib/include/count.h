//
// Created by Marc Fervil on 4/25/21.
//

#ifndef count_hpp
#define count_hpp

#include "runtime.hpp"

extern "C"{

dreamObj * count(dreamObj * num);
dreamObj * count_iter_next(dreamObj * scope);
dreamObj * count_iter(dreamObj * scope);

}



#endif /* count_hpp */
