//
//  memory.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#ifndef memory_hpp
#define memory_hpp

#include "runtime.hpp"


extern "C" {

/*Function that is used to get a var, if it returns undefined it will throw a fatal error **/
dreamObj * get_var(dreamObj * obj, const char *name, int from_parent=1);

/*This function is used to find a var, this can return undefined,
 and can be used to check for the existance of a var**/
dreamObj * find_var(dreamObj * obj, const char *s, int from_parent=1);

/*Set a variable onto a dreamObj**/
struct dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value);

/*This function is used to hash a var name to store it in the hash table**/
unsigned hash_obj(const char *s);

/*this function derefences a dreamObj** and returns null if either the pointer
 or the derefed pointer is null*/
dreamObj * deref_var(dreamObj ** obj);


struct dreamObj *set_var_soft(dreamObj *obj, const char *name, dreamObj *value);

}
#endif /* memory_hpp */

