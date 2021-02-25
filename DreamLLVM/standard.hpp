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

#define HASHSIZE 101

extern "C" {
#include <string.h>
typedef struct dreamObj{
    
    
   
    const char * name;
    dreamObj *next;
    
    void * value;

    dreamObj * type ;
    
    dreamObj *first_var;
    dreamObj *last_var;
    dreamObj * parent_scope;
    dreamObj * vars [HASHSIZE];
    //
    // etc..
} dreamObj;


    dreamObj * make_dream(void * value, dreamObj * type = nullptr);


    dreamObj * nullDream = NULL;
    dreamObj * dreamType = make_dream(nullptr);
    dreamObj * dreamObjType = make_dream((void *)"<Obj Type>",dreamType);
    dreamObj * dreamStrType = make_dream((void *)"<Str Type>",dreamType);
    dreamObj * dreamIntType = make_dream((void *)"<Int Type>",dreamType);
    dreamObj * dreamBoolType = make_dream((void *)"<Bool Type>",dreamType);
    dreamObj * dreamFuncType = make_dream((void *)"<Func Type>",dreamType);
    
    dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value);

    dreamObj * make_dream(void * value, dreamObj * type){
        if (type == nullptr)type = dreamObjType;
        dreamObj *new_obj = (dreamObj *) malloc(sizeof(struct dreamObj));
        
        new_obj -> value = (value);
        new_obj -> type = type;
        new_obj -> first_var = NULL;
        new_obj -> next = NULL;
        new_obj -> last_var = NULL;
        new_obj -> parent_scope = NULL;
        for(int i=0; i<HASHSIZE; i++){
            new_obj -> vars[i] = NULL;
        }
        
        return new_obj;
    }
    
    dreamObj * dreamFunc(void * value);
    dreamObj * dreamBool(int value);

    dreamObj * num_equals(dreamObj * me, dreamObj * other){
        return dreamBool((other->type==dreamIntType && (*(int*) me->value)==(*(int*) other->value)));
    }

    dreamObj * str_equals(dreamObj * me, dreamObj * other){
        const char * me_val = (const char *) me->value;
        const char * other_val = (const char *) me->value;
        
        return dreamBool(other->type==dreamStrType && strcmp(me_val, other_val)==0);
    }
        
    dreamObj * dreamStr(const char * value){
        dreamObj * obj = make_dream((void *) value, dreamStrType);
        set_var(obj, "equals", dreamFunc((void *) str_equals));
        return obj;
    }

    

    dreamObj * dreamInt(int value){
        int *num = (int *)malloc(sizeof *num);
        *num = value;
        dreamObj * obj = make_dream((void *)num, dreamIntType);
        set_var(obj, "equals", dreamFunc((void *)num_equals));
        return obj;
    }

    dreamObj * dreamBool(int value){
        int *num = (int *)malloc(sizeof *num);
        *num = value;
        dreamObj * obj = make_dream((void *)num, dreamBoolType);
        set_var(obj, "equals", dreamFunc((void *)num_equals));
        return obj;
    }

    dreamObj * dreamFunc(void * value){

        dreamObj * obj = make_dream((void *)value, dreamFuncType);
        
        return obj;
    }

    const char * rep(dreamObj* obj){
        
        if (obj == NULL) return "<Undefined>";
            
        
        if(obj->type == dreamStrType || obj->type == dreamType){
            return (const char *)obj->value;
        }else if(obj->type == dreamIntType){
            int * num = (int *)obj->value;
            int length = snprintf( NULL, 0, "%d", *num);
            char* str_ref = (char *)malloc(length + 1);
            snprintf(str_ref, length + 1, "%d", *num);
            
            const char * str = strdup(str_ref);
            free(str_ref);
            return str;
        }else if(obj->type == dreamBoolType){
            int * num = ((int *)obj->value);
            if (*num == 1) return "true";
            else if (*num == 0) return "false";
            //printf("%d")
            return "<Invalid Bool Data>";
        }else if(obj->type == dreamFuncType){
            const char * name = (const char *)obj->name;
            int length = snprintf( NULL, 0, "<Function %s>", name);
            char* str_ref = (char *)malloc(length + 1);
            
            snprintf(str_ref, length + 1, "<Function %s>", name);
            const char * str = strdup(str_ref);
            free(str_ref);
            return str;
        }else{
            return("<Invalid Type>");
        }
    }

    void print(dreamObj* obj){
        const char* str = rep(obj);
        printf("[Dream]: %s\n", str);
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
        //printf("%d",obj->parent_scope== &nullDream);
        if(obj->parent_scope != NULL && get_var(obj->parent_scope, s)!=NULL){
            //printf("up-get: %s\n",s);
           
            return get_var(obj->parent_scope, s);
        }
        for (np = obj->vars[hash_obj(s)]; np!=NULL; np = np->next){
           // dreamObj * t = obj->vars[hash_obj(s)];
            
            if (np->name != NULL && strcmp(s, np->name) == 0){
                return np; // found
            }
        }
        
        return NULL; // not found
    }



    dreamObj * copy(dreamObj * obj);

    
struct dreamObj *set_var_soft(dreamObj *obj, const char *name, dreamObj *value){
   
    unsigned hashval;
   
    hashval = hash_obj(name);
    
    value->name = strdup(name);
    if(obj->vars[hashval]!=NULL){
     //   free(obj->vars[hashval]);
      //  obj->vars[hashval] = value; /* (dreamObj *) realloc(value,sizeof(struct dreamObj));*/
        //obj->vars[hashval] ->name ="fewf";
        //printf("fefw");
        //free(obj->vars[hashval]->value);
        obj->vars[hashval]->value = value->value;
        obj->vars[hashval]->type = value->type;
        return NULL;
    }else{
        obj->vars[hashval] = (value);
    }
    
    if(obj->first_var == NULL){

        obj->first_var = obj->vars[hashval];
    }
    if(obj->last_var != NULL){
        obj->last_var ->next = obj->vars[hashval];
    }
    obj->last_var = obj->vars[hashval];
    
    return nullptr;
}


    struct dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value){
       
        unsigned hashval;
       
        if(obj->parent_scope != NULL && get_var(obj->parent_scope, name)!=NULL){
         //  printf("up-set: (%s: %s)\n", name, rep(value));
            //print(value);
         //   dict(obj->parent_scope);
            set_var(obj->parent_scope, strdup(name), (value));
          //  dict(obj->parent_scope);
            return nullptr;
        }
        set_var_soft(obj, name, value);
        
        return nullptr;
    }


    dreamObj * copy(dreamObj * obj){
//return obj;
        //print(obj->type);
        //dreamObj * np = make_dream((void *)"ijdoq", dreamStrType);
        if(obj->type==dreamFuncType)return obj;
        
        dreamObj *np = make_dream((void *)obj->value, obj->type);
        //np->type = dreamStrType;
        if(obj->name!=NULL){
            np->name =strdup( obj->name);
        }
        
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



    dreamObj * new_scope(dreamObj * obj){
       // printf("%d",obj->parent_scope == &nullDream);
        if(obj->parent_scope == nullDream){
            dreamObj * new_scope = dreamStr("[scope]");
         //   dreamObj ** ref = & new_scope;
            new_scope->parent_scope = (obj);
          //  scope(new_scope);
           // printf("yuh, %d",new_scope->parent_scope == &nullDream);
          //  (*obj).parent_scope =obj;
            return new_scope;
        }else{
            dreamObj * new_scope = dreamStr("[sub scope]");
            new_scope->parent_scope = obj->parent_scope;
           // dict(obj->parent_scope);
           // print(obj->parent_scope);
            //printf("%d",obj->parent_scope->first_var==NULL);
            //print(obj->parent_scope->first_var);
            //dict(obj->parent_scope);
            return new_scope;
        }
        return NULL;
    }


    
    dreamObj * equals(dreamObj * var1, dreamObj * var2){
        dreamObj * equ;
        if((equ = get_var(var1, "equals")) != NULL){
            dreamObj * b = ((dreamObj* (*)(dreamObj *, dreamObj *)) equ->value)(var1, var2);

            return b;
        }
        printf("[Nightmare]: <Undefined Equals>\n");
        return dreamBool(-1);
    }


}


