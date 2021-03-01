/*
//
//  standard.hpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/15/21.
//

#ifndef standard_hpp
#define standard_hpp

#include <stdio.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
#include <unistd.h>

#define HASHSIZE 101
extern "C" {
    typedef struct dreamObj{
        
        
       
        char * name;
        dreamObj *next;
        
        void * value;
        
     //
        dreamObj * type ;
        dreamObj * vars [HASHSIZE];
        // etc..
    } dreamObj;


    dreamObj * make_dream(void * value, dreamObj * type = nullptr);

}
#endif  standard_hpp */


//
//  standard.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/15/21.
//

#include <stdio.h>

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
#define HASHSIZE 101
#include <string.h>
extern "C" {

typedef struct dreamObj{
    
    
   
    const char * name;
    dreamObj *next;
    
    void * value;

    dreamObj * type ;
    
    dreamObj *first_var;
    dreamObj *last_var;
    dreamObj * parent_scope;
    int pointer;
    
    dreamObj * vars [HASHSIZE];
    //
    // etc..
} dreamObj;


    dreamObj * make_dream(void * value, dreamObj * type = nullptr);


    
    dreamObj * dreamType = make_dream(nullptr);
    dreamObj * nullDream = make_dream((void *)"<Undefined>", nullDream);
    dreamObj * dreamObjType = make_dream((void *)"<Obj Type>",dreamType);
    dreamObj * dreamStrType = make_dream((void *)"<Str Type>",dreamType);
    dreamObj * dreamIntType = make_dream((void *)"<Int Type>",dreamType);
    dreamObj * dreamBoolType = make_dream((void *)"<Bool Type>",dreamType);
    dreamObj * dreamFuncType = make_dream((void *)"<Func Type>",dreamType);
    dreamObj * dreamPointerType = make_dream((void *)"<Pointer Type>",dreamType);
    
    dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value);

    dreamObj * make_dream(void * value, dreamObj * type){
        if (type == nullptr)type = dreamObjType;
        dreamObj *new_obj = (dreamObj *) malloc(sizeof(struct dreamObj));
        
        new_obj -> value = (value);
        new_obj -> type = type;
        new_obj -> first_var = NULL;
        new_obj -> next = NULL;
        new_obj -> name = NULL;
        new_obj -> last_var = NULL;
        new_obj -> parent_scope = nullDream;
        
        new_obj->pointer = 0;
       
        for(int i=0; i<HASHSIZE; i++){
            new_obj -> vars[i] = NULL;
        }
        
        return new_obj;
    }
    
    dreamObj * dreamFunc(void * value);
    dreamObj * dreamBool(int value);
    dreamObj * copy(dreamObj * obj);
    dreamObj * dreamInt(int value);
    dreamObj * dreamStr(const char * value);

    dreamObj * num_equals(dreamObj * me, dreamObj * other){
        return dreamBool((other->type==dreamIntType && (*(int*) me->value)==(*(int*) other->value)));
    }

    dreamObj * num_add(dreamObj * me, dreamObj * other){
        return dreamInt( (*(int*) me->value) + (*(int*) other->value));
    }

    //TODO: FREE CONCATONATED STRING
    dreamObj * str_add(dreamObj * me, dreamObj * other){
        const char * me_val = (const char *) me->value;
        const char * other_val = (const char *) other->value;
        char* result;
        result = ( char*) calloc(strlen(me_val)+strlen(other_val)+1, sizeof(char));
        strcpy(result, me_val);
        strcat(result, other_val);
        return dreamStr(( char * )result);
    }

    dreamObj * str_equals(dreamObj * me, dreamObj * other){
        const char * me_val = (const char *) me->value;
        const char * other_val = (const char *) other->value;
        
        return dreamBool(other->type==dreamStrType && strcmp(me_val, other_val)==0);
    }

    const char * rep(dreamObj* obj, dreamObj* type=NULL);
            
    dreamObj * dreamStr(const char * value){
        
        char* result = (char*) malloc( strlen(value) + 1 );
       // printf("Length () %zu \n",strlen(value));
        strcpy(result, value);
        
        dreamObj * obj = make_dream((void *) result, dreamStrType);
      
        set_var(obj, "equals", dreamFunc((void *) str_equals));
        set_var(obj, "add", dreamFunc((void *)str_add));
        return obj;
    }

    

    dreamObj * dreamInt(int value){
        int *num = (int *)malloc(sizeof *num);
        *num = value;
        dreamObj * obj = make_dream((void *)num, dreamIntType);
        set_var(obj, "equals", dreamFunc((void *)num_equals));
        set_var(obj, "add", dreamFunc((void *)num_add));
        return obj;
    }

    dreamObj * dreamBool(int value){
        int *num = (int *)malloc(sizeof *num);
        *num = value;
        dreamObj * obj = make_dream((void *)num, dreamBoolType);
        set_var(obj, "equals", dreamFunc((void *)num_equals));
        return obj;
    }

void print(int num_args, ...);

dreamObj * get_var(dreamObj * obj, const char *s);
    
    dreamObj * dreamFunc(void * value){

        dreamObj * obj = make_dream((void *)value, dreamFuncType);
        
        return obj;
    }

    const char * rep(dreamObj* obj, dreamObj* type){
        if (obj==NULL)return "NULL";
        if (type==NULL) type = obj->type;
        if (obj==nullDream) return "<Undefined>";
        
        void * value =obj->value;
        
    start:
        if(type == dreamStrType || type == dreamType){
            
           // printf("str: %s\n",   (( char *)obj->value) );
           
   
           // printf("\n");
            return (( char *)value);
        }else if(type == dreamIntType){
            int * num = (int *)value;
            int length = snprintf( NULL, 0, "%d", *num);
            char* str_ref = (char *)malloc(length + 1);
            snprintf(str_ref, length + 1, "%d", *num);
            
            const char * str = strdup(str_ref);
            free(str_ref);
            return str;
        }else if(type == dreamBoolType){
            int * num = ((int *)value);
            if (*num == 1) return "true";
            else if (*num == 0) return "false";
            //printf("%d")
            return "<Invalid Bool Data>";
        }else if(type == dreamFuncType){
             char * name = ( char *)obj->name;
            //printf("%s", name);
            int length = snprintf( NULL, 0, "<Function %s>", name);
            char* str_ref = (char *)malloc(length + 1);
            
            snprintf(str_ref, length + 1, "<Function %s>", name);
            char * str = strdup(str_ref);
            free(str_ref);
            return str;
        }else if(type == dreamPointerType){
           
            value = *((void **)obj->value);
           // printf("\n\n%s",(char *) *((void **)obj->value));
           type = get_var(obj, "ptr_type");
            //TODO: FIX THIS BECAUSE IT IS DUMB
            if(strcmp(rep(type),rep(dreamStrType))==0)type = dreamStrType;
            else if(strcmp(rep(type),rep(dreamIntType))==0)type = dreamIntType;
            else if(strcmp(rep(type),rep(dreamBoolType))==0)type = dreamBoolType;
            //type = dreamStrType;
            //printf("\ntype %s\n",rep(type));
            goto start;
            //return rep( ,);
        
            
        }else{
            return("<Invalid Type>");
        }
    }

    void print(int num_args, ...){
        va_list valist;
        va_start(valist, num_args);

        printf("[Dream]: ");
        for (int i = 0; i < num_args; i++) {
            const char* str = rep(va_arg(valist, dreamObj *));
            const char* ending = " ";
            if(i==num_args-1) ending = "\n";
            printf("%s%s", str, ending);
        }

       
       va_end(valist);
    }


    void print2(dreamObj *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        while (fmt != NULL) {
           
            const char* str = rep(va_arg(args, dreamObj*));
            printf("[Dream]: %s\n", str);
            ++fmt;
        }

        va_end(args);
    }


    void dict(dreamObj* obj){
        dreamObj * var;
        if(obj==NULL){
            printf("<Undefined dict>\n");
            return;
        }
        printf("\n{");
        for (var = obj->first_var; var!=NULL; var = var->next){
            printf("\n\t%s: %s", var->name, rep(var));
        }
        printf("\n}\n");
    }

    unsigned hash_obj(const char *s){
        unsigned hashval;
        for (hashval = 0; *s != '\0'; s++)
          hashval = *s + 31 * hashval;
        return hashval % HASHSIZE;
    }



    dreamObj * get_var(dreamObj * obj, const char *s){
        dreamObj *np;
        //printf("%s parent is %s\n",s,rep(obj));
       // dict(obj);
       // if(obj->parent_scope != NULL)dict(obj->parent_scope);
        
        for (np = obj->vars[hash_obj(s)]; np!=NULL; np = np->next){
           // dreamObj * t = obj->vars[hash_obj(s)];
            //if(np->name != NULL)printf("looking %s\n",np->name);
            if (np->name != NULL && strcmp(s, np->name) == 0){
                //copy
                //if(np->pointer==1){
                    //printf("got pointer named %s\n",s);
                //}
                return (np); // found
            }
        }
        if(obj->parent_scope != nullDream /*&& get_var(obj->parent_scope, s)!=nullDream*/){
            //printf("up-get: %s\n",s);
           
            return get_var(obj->parent_scope, s);
        }
        return nullDream; // not found
    }

dreamObj* ptr(dreamObj* value){
   // dreamObj* ref = value;
    
    value->pointer = 2;
    
    //set_var(ptr, "equals", dreamFunc((void *) str_equals));
    return value;
    
}

dreamObj* pointer(dreamObj* value){
   // dreamObj* ref = value;
    
   // printf("fopkwe?");
    dreamObj* ptr = make_dream(&value->value, dreamPointerType);
    dreamObj* ty;
    //print(1,value->type);
    if(value->type == dreamStrType){
        //printf("fewofkewp");
        ty = dreamStrType;
    }
    set_var(ptr, "ptr_type", value->type);
    //set_var(ptr, "equals", dreamFunc((void *) str_equals));
    return ptr;
    
}
  
void * copy_value(void * value, dreamObj *type);
void * copy_value_re(void * value, dreamObj *obj);

struct dreamObj *set_var_soft(dreamObj *obj, const char *name, dreamObj *value){
   
    unsigned hashval;
   
    hashval = hash_obj(name);
    
    //printf("setting %s onto %s\n",rep(value),rep(obj));
    
    
    
    if(obj->vars[hashval] != NULL){
        //void * new_val = copy_value(value->value, obj->vars[hashval]->type);
        
        
        //printf("name: %s, pointer %d\n",name,obj->vars[hashval]->pointer);
       // if(obj->vars[hashval]->type==dreamIntType || obj->vars[hashval]->type==dreamBoolType){
        dreamObj * found = obj->vars[hashval];
       // char * val =(char *)value->value;
        if(found->name != NULL && strcmp(found->name, name)!=0){
            //printf("TODO: FIX HASH COLLISION POINTER BUG - %s COLLIDES WITH %s\n", name, obj->vars[hashval]->name);
            //exit(0);
            dreamObj * next = obj->vars[hashval]->next;
            print(1, next);
            
            obj->vars[hashval]->next = copy(value);
            obj->vars[hashval]->next ->name = strdup(name);
            
            obj->vars[hashval]->next ->next = next;
            
           
            return NULL;
            /*
            for (found = found; found!=NULL; found = found->next){
               // dreamObj * t = obj->vars[hash_obj(s)];
                //if(np->name != NULL)printf("looking %s\n",np->name);
                if (found->name == NULL && strcmp(s, np->name) == 0){
                    //copy
                    //if(np->pointer==1){
                        //printf("got pointer named %s\n",s);
                    //}
                    return (np); // found
                }
            }*/
        }
        if(found->type == dreamPointerType){
            //printf("living the dream");

            *(((void **)found->value)) = copy_value(value->value,  value->type);
            //ptr_obj = copy_value(value->value,  value->type);
            //=copy_value(value->value,  value->type);
        }else{
           
            free(found->value);
            found->value = copy_value(value->value,  value->type);

            found->name = strdup(name);
            
            found->type = value->type;
        }
        
       // char * temp = (char*) realloc( obj->vars[hashval]->value, strlen(val + 1 ));
       
        
        
       // temp = val;
        //free(obj->vars[hashval]->value);
        
      
      //  strcpy((char *)obj->vars[hashval]->value, val);
        //printf("\ntemp: %s; \nog: %s\n",temp, (char *)obj->vars[hashval]->value);
        //obj->vars[hashval]->value= temp;
        //free(obj->vars[hashval]->value);
        
   
        
        
        return NULL;
    }else{
        //if(value->name ==NULL)value->name = strdup(name);
        //bool ptr = false;
        if(value->pointer==2){
          //  printf("making pointer");
        //if(strcmp(name, "new_test")==0){
            //printf("poiterrr");
            value->pointer = 0;
//            (char*) malloc( strlen(value) + 1 );
           // free(obj->vars[hashval]);
            obj->vars[hashval] = pointer(value);
            obj->vars[hashval] -> pointer = 1;
            //printf("POINTER");
           // obj->vars[hashval] -> pointer = 1;
            //return NULL;
            //obj->vars[hashval]->type = value->type;
            
        }else if(value->pointer==1){
          //  free(obj->vars[hashval]);
            //printf("wooop %s\n",name);
            obj->vars[hashval] = copy(value);
            obj->vars[hashval] -> pointer = 1;
          //  printf("chosen one %s\n",name);
        }else{
            obj->vars[hashval] = copy(value);
        }
        
       
        
        obj->vars[hashval]->name = strdup(name);
       
       // obj->vars[hashval]->value = copy_value(value->value,  value->type);
        //obj->vars[hashval]->type = value->type;
    }
    
    if(obj->first_var == NULL){

        obj->first_var = obj->vars[hashval];
    }
    if(obj->last_var != NULL){
        obj->last_var ->next = obj->vars[hashval];
    }
    obj->last_var = obj->vars[hashval];
 
        //obj->last_var ->next = nullDream;
        //obj->last_var = nullDream;
        //obj->vars[hashval]->name = "fewfe";

    return nullptr;
}


   

    struct dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value){
       
      
        
        if(obj->parent_scope != nullDream && get_var(obj->parent_scope, name)!=nullDream){
         //  printf("up-set: (%s: %s)\n", name, rep(value));
            //print(value);
         //   dict(obj->parent_scope);
            
            set_var(obj->parent_scope, strdup(name), (value));
           
          //  dict(obj->parent_scope);
            return nullptr;
        }
        //printf("name %s\n",name);
       //
        
        return set_var_soft(obj, name, value);
    }

 

    struct dreamObj *set_var_c(dreamObj *obj, dreamObj *name_obj, dreamObj *value){
       
        char * name = ( char *)name_obj->value;
        //printf("ofkwpok %s", name);
        return set_var(obj, name, value);
    }

    void * copy_value(void * value, dreamObj *type){
    // print(type);
         if(type == dreamIntType || type == dreamBoolType){
             int *num = (int *)malloc(sizeof *num);
             *num = *(int *)value;
             //printf("Copied int to %d\n", (int *)value);
             return num;
         }else if(type == dreamStrType){
             //printf("copy %s %zu\n ",(char *)value, strlen((char *)value));
           //  printf("copy %s %zu\n",(char *)value,strlen((char *)value));
            // printf("")
  
             char* result = (char*) malloc( strlen((char *)value) + 1 );
         
             strncpy(result, (char *)value, strlen((char *)value) + 1);
             
              //strdup((char *)value);
             return result;
         }else if(type == dreamFuncType){
             return value;
         }
         return value;
    }

    void fuck(dreamObj * obj){
        for(int i=0;i< strlen((char *)obj->value); i++){
         //   result[i] = ((char *)obj->value)[i];
            printf("%c",((char *)obj->value)[i]);
        }
        printf("\n");
    }


    void * copy_value_re(void * value, dreamObj *obj){
    // print(type);
         if(obj->type == dreamIntType || obj->type == dreamBoolType){
             int *num = (int *)malloc(sizeof *num);
             *num = *(int *)value;
             //printf("Copied int to %d\n", (int *)value);
             return num;
         }else if(obj->type == dreamStrType){
             char* result = (char*) malloc(strlen((char *)value) + 1 );
          
             strcpy(result, (char *)value);
             return result;
         }
         return value;
    }


    dreamObj * copy(dreamObj * obj){
//return obj;
        //print(obj->type);
        //dreamObj * np = make_dream((void *)"ijdoq", dreamStrType);
        
        if(obj==NULL )return obj;
        //printf("copying %s\n",rep(obj));
        dreamObj *np = make_dream(copy_value(obj->value,  obj->type), obj->type);
        //np->type = dreamStrType;
        if(obj->name!=NULL){
            np->name =strdup( obj->name);
        }
       // np->parent_scope = copy(obj->parent_scope);
        
        //printf("fwlew");
     
       // np->next = obj->next;
        dreamObj *var;
        //printf("yeaa:%d\n", np->first_var == NULL);
        for (var = obj->first_var; var!=NULL; var = var->next){
           // printf("copied field: %s\n",obj->name);
            set_var(np, strdup(var->name), (var));
        }
        
       // printf("copied: ");
       // print(np);
        return np;
    }



    dreamObj * new_scope(dreamObj * obj, bool nested_scope){
       // printf("%d",obj->parent_scope == &nullDream);
        //printf("scope bitch %d", nested_scope);
        if(obj->parent_scope == nullDream || nested_scope){
            dreamObj * new_scope = dreamStr("[scope]");

            new_scope->parent_scope = (obj);
            

            return new_scope;
        }else{
            
            dreamObj * new_scope = dreamStr("[sub scope]");
            new_scope->parent_scope = (obj->parent_scope);
            
            //dict(new_scope);
           // print(obj->parent_scope);
            //printf("%d",obj->parent_scope->first_var==NULL);
            //print(obj->parent_scope->first_var);
            //dict(obj->parent_scope);
            return new_scope;
        }
        return NULL;
    }


    
    dreamObj * equals_c(dreamObj * var1, dreamObj * var2){
        dreamObj * equ;
        if((equ = get_var(var1, "equals")) != NULL){
            
            dreamObj * b = ((dreamObj* (*)(dreamObj *, dreamObj *)) equ->value)(var1, var2);

            return b;
        }
        printf("[Nightmare]: <Undefined Equals Operation>\n");
        return dreamBool(-1);
    }
    
     void unexposed(){
        
    }


    dreamObj * add_c(dreamObj * var1, dreamObj * var2){
        
        dreamObj * equ;
        if((equ = get_var(var1, "add")) != NULL){
            dreamObj * result = ((dreamObj* (*)(dreamObj *, dreamObj *)) equ->value)(var1, var2);
            
            return result;
        }
        printf("[Nightmare]: <Undefined Add Operation>\n");
        return dreamBool(-1);
    }


}


