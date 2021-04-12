//
//  mac_compat.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/12/21.
//

#ifndef mac_compat_hpp
#define mac_compat_hpp

#include <stdio.h>
#include "window.h"
#include "standard.hpp"
#include "mac_compat.cpp"
extern "C"{

dreamObj * app(dreamObj * name);

}

#endif /* mac_compat_hpp */
