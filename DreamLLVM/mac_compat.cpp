//
//  mac_compat.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/12/21.
//

#include "mac_compat.hpp"

extern "C"{

dreamObj * app(dreamObj * name){

    mac_init(((char *)name->value));
    dreamObj * obj = make_dream((void *) 1, dreamObjType);
    
    //add_native_func(obj, "get", (void *) list_get);
    
   
    
    
    return obj;
}

}
