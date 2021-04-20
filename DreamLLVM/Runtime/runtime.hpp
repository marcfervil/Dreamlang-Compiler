//
//  runtime.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef runtime_hpp
#define runtime_hpp

#include <stdio.h>

#include <stdlib.h>
#include <stdarg.h>

#include <algorithm>
#include <cassert>

#include <memory>
#include <vector>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <cstdarg>
#include <string.h>
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include <string.h>

#define HASHSIZE 1022



extern "C" {

/*Line number that the code is on**/
extern int line = 1;

/*flag that allows undefined vars (It should be false, it's only true during debugging)**/
extern bool undefined_allowed = false;

/*This struct represents every object in dreamlang**/
typedef struct dreamObj{
    
    const char * name;
    dreamObj ** next;
    
    void * value;

    dreamObj * type ;
    
    dreamObj ** first_var;
    dreamObj ** last_var;
    dreamObj * parent_scope;
    
    int pointer;
    
    dreamObj ** vars [HASHSIZE];
    
    int is_list;
    dreamObj * ref;

} dreamObj;

/*Malloc double pointer**/
dreamObj ** pointer_init();

/*Initilize dreamObj**/
dreamObj * make_dream(void * value, dreamObj * type = nullptr);


/*Types for dreamObjs **/
dreamObj * dreamType = make_dream(nullptr);
dreamObj * dreamObjType = make_dream((void *)"<Obj Type>",dreamType);
dreamObj * dreamStrType = make_dream((void *)"<Str Type>",dreamType);
dreamObj * dreamIntType = make_dream((void *)"<Int Type>",dreamType);
dreamObj * dreamListType = make_dream((void *)"<List Type>",dreamType);
dreamObj * dreamBoolType = make_dream((void *)"<Bool Type>",dreamType);
dreamObj * dreamFuncType = make_dream((void *)"<Func Type>",dreamType);
dreamObj * dreamPointerType = make_dream((void *)"<Pointer Type>",dreamType);

/*function for when you call a null object*/
void nullCall();

/*Dreamlang's null**/
dreamObj * nullDream = make_dream((void *) nullCall, nullDream);

/*Function that is used to create runtime errors**/
void nightmare(const char * message, ...);


/*Platform specific error logging **/
void log_error(const char * error);

/*Code the returns the string representation of a dreamObj**/
const char * rep(dreamObj* obj);

/*Platform specific printing**/
void print(int num_args, ...);

/*Function that prints out the JSON/dictionary representation of a dreamObj**/
void dict(dreamObj* obj);

/*Function that is used to merge two dreamObjs together by
 adding the variables from var2 onto var1 **/
void merge(dreamObj * var1, dreamObj * var2);


/*Initialize dream object with no value and with a basic object type*/
dreamObj * dreamObject();

/*Re parents dream object**/
dreamObj * set_parent(dreamObj * obj, dreamObj * new_parent);

/*returns a new scope object given a dreamObj**/
dreamObj * new_scope(dreamObj * obj, int nested_scope);
}



#include "nativetypes.hpp"
#include "copy.hpp"
#include "memory.hpp"

#endif /* runtime_hpp */
