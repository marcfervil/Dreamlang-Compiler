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

#define HASHSIZE 100


extern "C" {



/*Line number that the code is on (only used for debugging)**/
extern int line;

/*flag that allows undefined vars (It should be false, it's only true during debugging)**/
extern bool undefined_allowed;

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
extern dreamObj * dreamType;
extern dreamObj * dreamObjType;
extern dreamObj * dreamStrType;
extern dreamObj * dreamIntType;
extern dreamObj * dreamListType;
extern dreamObj * dreamBoolType;
extern dreamObj * dreamFuncType;
extern dreamObj * dreamPointerType;


/*function for when you call an undefined object*/
void nullCall();

/*Dreamlang's null**/
extern dreamObj * nullDream;

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
