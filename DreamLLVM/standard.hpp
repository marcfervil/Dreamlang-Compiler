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

#include <string.h>
extern "C" {

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
    //
    // etc..
} dreamObj;


    dreamObj * make_dream(void * value, dreamObj * type = nullptr);

    void nightmare(const char * message){
        printf("\x1B[31m[Nightmare]: ");
        printf("%s", message);
        printf("\n\033[0m");
        exit(1);
    }




    void nullCall(){
        nightmare("Cannot call undefined!");
        exit(0);
    }
    
    dreamObj * dreamFunc(void * value);
    dreamObj * dreamBool(int value);
    dreamObj * copy(dreamObj * obj);
    dreamObj * shallow_copy(dreamObj * obj);
    dreamObj * deep_copy(dreamObj * obj);
    dreamObj * dreamInt(int value);
    dreamObj * dreamStr(const char * value);
    dreamObj * deref_var(dreamObj ** obj);
    const char * rep(dreamObj* obj);
    void print(int num_args, ...);

    dreamObj * dreamType = make_dream(nullptr);
    dreamObj * nullDream = make_dream((void *) nullCall, nullDream);
    dreamObj * dreamObjType = make_dream((void *)"<Obj Type>",dreamType);
    dreamObj * dreamStrType = make_dream((void *)"<Str Type>",dreamType);
    dreamObj * dreamIntType = make_dream((void *)"<Int Type>",dreamType);
    dreamObj * dreamBoolType = make_dream((void *)"<Bool Type>",dreamType);
    dreamObj * dreamFuncType = make_dream((void *)"<Func Type>",dreamType);
    dreamObj * dreamPointerType = make_dream((void *)"<Pointer Type>",dreamType);
    
    dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value);
    dreamObj* temp = nullptr;

    dreamObj ** pointer_init(){
        dreamObj ** ptr = (dreamObj **)malloc(sizeof(struct dreamObj *));
        *(ptr) = NULL;
        return ptr;
    }

    dreamObj * make_dream(void * value, dreamObj * type){
        if (type == nullptr)type = dreamObjType;
        dreamObj *new_obj = (dreamObj *) malloc(sizeof(struct dreamObj));
        
        
        new_obj -> value = value;
        new_obj -> type = type;
        new_obj -> name =  NULL;
        new_obj -> first_var = pointer_init();
        new_obj -> next = pointer_init();
        new_obj -> last_var = pointer_init();
        new_obj -> parent_scope = nullDream;
        
        new_obj->pointer = 0;
        
       
        for(int i=0; i<HASHSIZE; i++){
            (new_obj->vars[i]) =  (dreamObj **)malloc(sizeof(struct dreamObj *));
            *(new_obj->vars[i])= NULL;
            //(dreamObj **)malloc(sizeof(struct dreamObj *));
           // (new_obj->vars[i]) = NULL;
            //(dreamObj **)malloc(sizeof(struct dreamObj *));
            //
        }
        
      //  new_obj->vars = new dreamObj**[HASHSIZE];
        
        //(new_obj->vars) = &vars;
        
        //vars2= vars;
        return new_obj;
    }
    


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

    const char * rep(dreamObj* obj);
            
    dreamObj * dreamStr(const char * value){
        
        char* result = (char*) malloc( strlen(value) + 1 );
       // printf("Length () %zu \n",strlen(value));
        strcpy(result, value);
        
        dreamObj * obj = make_dream((void *) result, dreamStrType);
      
        set_var(obj, "equals", dreamFunc((void *) str_equals));
       // printf("yo\n");
       // print(1, *(obj->first_var));
        set_var(obj, "add", dreamFunc((void *)str_add));
     //   set_var(obj, "add", dreamInt(4));

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

    

    dreamObj * get_var(dreamObj * obj, const char *s);
    
    dreamObj * dreamFunc(void * value){

        dreamObj * obj = make_dream((void *)value, dreamFuncType);
        
        return obj;
    }

    const char * rep(dreamObj* obj){
        if (obj==nullDream) return "<Undefined>";
        if (obj->type==NULL) return "<NULL type>";
        //if (obj==NULL)return "<NULL>";
        dreamObj * type = obj->type;
        
       // if (type==NULL)
        //if (obj==temp) return "<Temp>";
       // if (type==temp) return "<Temp type>";
      
        
        
        void * value = obj->value;
        

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
        
        }else if(type == dreamObjType){
            return("<Object>");
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
            printf("<NULL dict>\n");
            return;
        }else if(obj==nullDream){
            printf("<Undefined dict>\n");
            return;
        }
        printf("\n{");
        for (var = deref_var(obj->first_var); var!=NULL; var = deref_var(var->next)){
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

    dreamObj * deref_var(dreamObj ** obj){
        if(obj==NULL){
         //   printf("NADDA");
            
        }
        return (obj==NULL) ? NULL : *obj;
    }

    dreamObj * get_var(dreamObj * obj, const char *s){
        //printf("get var %s\n",s);
        //print(1,  obj->vars[0]);
        //dict(obj);
        if(obj == nullDream || obj==temp){
            printf("[Nightmare]: Cannot get property %s from undefined!\n",s);
            exit(0);
        }
        if(strcmp(s, "parent") == 0)return obj->parent_scope;
        dreamObj * np;
        //printf("%s parent is %s\n",s,rep(obj));
       // dict(obj);
       // if(obj->parent_scope != NULL)dict(obj->parent_scope);
        /*
        for(int i=0;i<HASHSIZE; i++){
            if(obj->vars[i]!=NULL){
                print(1, obj->vars[i]);
            }
        }*/
       // dict(obj);
    //    dict(obj);
        
      //  printf("cafe\n");
        //printf("%s VS %s\n",np->name, s);
       
        for (np = deref_var(obj->vars[hash_obj(s)]); np!=NULL; np = deref_var(np->next)){
           // dreamObj * t = obj->vars[hash_obj(s)];
            //if(np->name != NULL)printf("looking %s\n",np->name);
            
         //   print(1, np);
               // rep( np);
            //print(1, np->type);
            //printf("%s\n",rep(np));
           //     printf("%s\n",np->name);
            
            if (np->name != NULL && strcmp(s, np->name) == 0){
                //copy
                //if(np->pointer==1){
                    //printf("got pointer named %s\n",s);
                //}
               
                return np; // found
            }
        }
        if(obj->parent_scope != nullDream && strcmp(s,"scope")!=0 && s[0]!='@'){
           // printf("up-get: %s\n",s);
           
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
    print(1,value->type);
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
    if(temp!=NULL){
        printf("TEMP REASS\n");
    }
   
    dreamObj** value_pointer;
   
    if(deref_var(obj->vars[hashval])!=NULL){
       
        //void * new_val = copy_value(value->value, (*(obj->vars[hashval]))->type);
       // printf("wooboo\n");
       // printf("\n\nREASSIGNED %s \n\n", rep(deref_var(obj->vars[hashval])));
        /*
        if(obj->pointer){
            (*((*(obj->vars[hashval])))).type = value ->type;
            //copy_value(value->value,  value->type);
            printf("Here\n");
            return NULL;
        }*/
        
        if(value->name!= NULL && strcmp(value->name,"scope")==0){
            printf("reset scope\n");
        }
        
        //printf("name: %s, pointer %d\n",name,(*(obj->vars[hashval]))->pointer);
       // if((*(obj->vars[hashval]))->type==dreamIntType || (*(obj->vars[hashval]))->type==dreamBoolType){
        dreamObj * found = (*(obj->vars[hashval]));
       // char * val =(char *)value->value;
        if(found!=NULL){
           // dreamObj * l = NULL;
            //dreamObj ** g = &l ;
          //  printf("%d\n", (*g)==NULL);
          //  printf("found not null %s ?\n",rep(found));
        }
        if(found->name != NULL && strcmp(found->name, name)!=0){
           
            printf("TODO: FIX HASH COLLISION POINTER BUG - %s COLLIDES WITH %s\n", name, (*(obj->vars[hashval]))->name);
            //exit(0);
            
            
            // THIS WAS WORKING FIX IT
            dreamObj ** next = (*(obj->vars[hashval]))->next;
            //print(1,next);
            *((*(obj->vars[hashval])))->next = copy(value);
           ( (*((*(obj->vars[hashval])))->next) ) ->name = strdup(name);
            
          //
            
           (*(*(*(obj->vars[hashval]))->next)->next)->next = next;
            if(obj->vars[hashval]==obj->last_var){
                obj->last_var =(*(obj->vars[hashval]))->next ;
            }
            
            
            //cat
            //  tabby

           
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
        }/*
        if(found->type == dreamPointerType){
       

            *(((void **)found->value)) = copy_value(value->value,  value->type);

        }*/
        
        
        
    //    if((*(obj->vars[hashval]))->type == dreamObjType){
           
         
      //  }
       // if(value->name!=NULL && value->name[0]=='@'){
        if(value->type == dreamObjType){
          //  printf("\nstarting free1");
           // printf("TODO: Proper Object reassignment\n");
           // free((*(obj->vars[hashval]))->value);
           // printf("\nfreed 1");
            //dict(value );
            
            //LALALA
            
            (*(obj->vars[hashval]))->value = value->value;
            (*(obj->vars[hashval])) ->name = strdup(name);
            
            (*(obj->vars[hashval]))->type = value->type;
            //(*(obj->vars[hashval]))->first_var = value->first_var;
            
            for(int i=0; i<HASHSIZE; i++){
               // if()
               // set_var(np, strdup(var->name), (var));
                (*(obj->vars[hashval]))->vars[i] = value->vars[i];
                
                //value->vars[i];
                //pointer_init();
                //(obj->vars[i]);
             
            }
            
            
            for (dreamObj* var = deref_var(value->first_var); var!=NULL; var = deref_var(var->next)){
               set_var((*(obj->vars[hashval])), strdup(var->name), (var));
            }
            
          //  (*(obj->vars[hashval])) = shallow_copy(value);
          //  (*(obj->vars[hashval])) ->name = name;
            
            //memcpy((*(obj->vars[hashval]))->vars, value->vars, sizeof((*(obj->vars[hashval]))->vars));
            //printf("here\n");
            
           // printf("HERE WTF?????\n");
            /*
            free(found->value);
            found->value = value->value;
            found->name = value->name;
            found->type = value->type;
            for(int i=0;i<HASHSIZE;i++){
                if(found->vars[i]!=NULL && found->vars[i]->type != dreamFuncType){
                    //print(1,found->vars[i]);
                    //printf("======\n");
                    free(found->vars[i]);
                    found->vars[i] = value->vars[i];
                }
                //found->vars[i] = shallow_copy(value->vars[i]);
            }*/
           
        }else{
           
           // print(1, found);
            if(!(found->type==dreamObjType || found->type==nullDream )){
                free(found->value);
            }
            found->value = copy_value(value->value,  value->type);

            found->name = strdup(name);
            (*(obj->vars[hashval]))->type = value->type;
            
          
           
        }
        
       // char * temp = (char*) realloc( (*(obj->vars[hashval]))->value, strlen(val + 1 ));
       
        
        
       // temp = val;
        //free((*(obj->vars[hashval]))->value);
        
      
      //  strcpy((char *)(*(obj->vars[hashval]))->value, val);
        //printf("\ntemp: %s; \nog: %s\n",temp, (char *)(*(obj->vars[hashval]))->value);
        //(*(obj->vars[hashval]))->value= temp;
        //free((*(obj->vars[hashval]))->value);
        
   
        
        
        return NULL;
    }else{
        
        /*
        if(value->pointer==2){

            value->pointer = 0;

            (*(obj->vars[hashval])) = pointer(value);
            (*(obj->vars[hashval])) -> pointer = 1;
 
        }else if(value->pointer==1){
            (*(obj->vars[hashval])) = copy(value);
            (*(obj->vars[hashval])) -> pointer = 1;
        }else{
            (*(obj->vars[hashval])) = copy(value);
        }*/
        if(strcmp(name,"scope")==0 ){
            (*(obj->vars[hashval])) = value;
        }else if(name[0]=='@' || value->type==dreamObjType){
           // printf("deep_dish");
            dreamObj * new_value = shallow_copy(value);
            new_value ->name = strdup(name);
            (*(obj->vars[hashval])) = new_value;
            
            //(*(obj->vars[hashval]))= dreamStr("oopsies");
            //printf("dd");
            //shallow_copy(value);
            //printf("@ name is %s", name);
            
        }else{

          //  dreamObj * live = var_copy = copy(value);
        //    value->name =
            /*
            dreamObj** value_pointer = (dreamObj **)malloc(sizeof(struct dreamObj *));
            *(value_pointer) = copy(value);
            (*value_pointer) ->name = strdup(name);
            obj->vars[hashval] = value_pointer;*/
            
            dreamObj * new_value =copy(value);
            new_value ->name = strdup(name);
            *(obj->vars[hashval]) = new_value;
            
         //   printf("Assigned type %s\n", rep((*(&var_copy))->type));
    
            
            
            
        }
        if(value->name!=NULL && value->name[0]=='@'){
            //printf("changing context to %s\n",name);
           // name = "@context";
        }
        //UNCOMMENT NAME REST
    //    (*(obj->vars[hashval]))->name = strdup(name);
       
       // (*(obj->vars[hashval]))->value = copy_value(value->value,  value->type);
        //(*(obj->vars[hashval]))->type = value->type;
    }
    
    if(deref_var(obj->first_var) == NULL){
       
        obj->first_var = obj->vars[hashval] ;
      //  printf("Assigned frist var on %s to %s\n", rep(obj), rep(*(obj->first_var)));
    }
    if(deref_var(obj->last_var) != NULL){
        ( *(obj->last_var ))-> next =  obj->vars[hashval] ;
    }
    obj->last_var =  obj->vars[hashval] ;
    
 
        //obj->last_var ->next = nullDream;
        //obj->last_var = nullDream;
        //(*(obj->vars[hashval]))->name = "fewfe";

    return nullptr;
}

    

    struct dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value){
        //printf("\x1B[31m open");
       // printf("Setting var type - %s", rep(value->type));
      
        
        if(obj == nullDream){
            printf("\x1B[31m[Nightmare]: Cannot set property %s on undefined! \n\033[0m", name);
            
            exit(0);
        }
        
        
        if(strcmp(name, "scope")!=0 && name[0]!='@' && obj->parent_scope != nullDream && get_var(obj->parent_scope, name)!=nullDream){
               // printf("up-set: (%s: %s)\n", name, rep(value));
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

    dreamObj * someStuff(dreamObj * obj){
        
       // printf("%d\n", obj->first_var == NULL);
      //  print(1, *(obj->first_var));
        return NULL;
    }
    
    dreamObj* copy(dreamObj * obj){
        //printf("COPYING OBJ OF TYPE %s\n", rep(obj->type));
        if(obj==NULL || obj==nullDream || obj->type == dreamObjType ||obj->type == dreamFuncType)return obj;
        
        
        
        dreamObj *np = make_dream(copy_value(obj->value,  obj->type), obj->type);
        

        dreamObj *var;
       // print(1, deref_var(obj->first_var));
        for (var = deref_var(obj->first_var); var!=NULL; var = deref_var(var->next)){
          
            set_var(np, strdup(var->name), copy(var));
        }
        
       // print(1, np->type);
        return np;
    }

    dreamObj * shallow_copy(dreamObj * obj){
        //return obj;
       // printf("shallowm");
      //  dict(obj);
        if(obj==NULL || obj==nullDream)return obj;

       dreamObj *np = make_dream(obj->value, obj->type);
       // dreamObj** new_type = &(obj->type);
       // np->type = (dreamObj *)new_type;
        
      //  (np->value) = &obj->value;
      

        np->parent_scope = obj->parent_scope;
        np->first_var = obj->first_var;
        np->last_var = obj->last_var;
        np->next = obj->next;
      
        //obj->first_var = obj
        
        /*
         dreamObj *var;
        for (var = obj->first_var; var!=NULL; var = var->next){
           // print(2, dreamStr(var->name), var);
            set_var(np, (var->name), (var) );
        }*/
       
       
        
        for(int i=0; i<HASHSIZE; i++){
           // if()
           // set_var(np, strdup(var->name), (var));
           np->vars[i] = obj->vars[i];
            //(obj->vars[i]);
         
        }
        
        /*
        dreamObj *var;
        for (var = deref_var(obj->first_var); var!=NULL; var = deref_var(var->next)){
          
            set_var(np, strdup(var->name), (var));
        }*/
        //dict(np);
       
       // obj->vars[2] = (dreamObj **)malloc(sizeof(struct dreamObj *));
     
        
        //set_var(np, "shallow", dreamBool(1));
        //memcpy(np->vars, obj->vars, sizeof(obj->vars));
       // dict(np);
//dict(np);
//set_var(np, "meme", dreamStr("scream"));
       // print(1, get_var(np, "meme"));
        return np;
    }


    dreamObj * deep_copy(dreamObj * obj){
       // return obj;
        //return obj;
        
        if(obj==NULL || obj==nullDream)return obj;

        dreamObj *np = make_dream(obj->value, obj->type);
        if(obj->name!=NULL){
            np->name =strdup( obj->name);
        }

        dreamObj *var;
        
       // np->vars = obj->vars;
        np->parent_scope = obj->parent_scope;
        //memcpy(np->vars, obj->vars, sizeof(obj->vars));
        
        
        for (var = deref_var(obj->first_var); var!=NULL; var = deref_var(var->next)){
            //printf("name: %s\n", var->name);
            if(var->name!=NULL && (var->name[0]=='@'))continue;
            //char * new_name=strdup(var->name);
            //new_name[0] = 'e';
            set_var(np, strdup(var->name), deep_copy(var));
            
        }
        
      //  set_var(np, "shallow", dreamBool(1));
        return np;
    }

    dreamObj * set_parent(dreamObj * obj, dreamObj * new_parent){
        obj->parent_scope = new_parent;
        return obj;
    }

    dreamObj * new_scope(dreamObj * obj, int nested_scope){
       // printf("%d",obj->parent_scope == &nullDream);
        //printf("scope bitch %d", nested_scope);
        
        if((obj->parent_scope == nullDream || nested_scope)){
            //nested scope
            dreamObj * new_scope = make_dream(nullDream);

            new_scope->parent_scope = (obj);
            

            return new_scope;
        }else{
            //lateral scope
            dreamObj * new_scope  = make_dream(nullDream);
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
        if(var1 == nullDream) {
           
            return dreamBool(var1==nullDream && var2==nullDream);
        }
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
    
    void merge(dreamObj * var1, dreamObj * var2){
        dreamObj * var;
        for (var = deref_var(var2->first_var); var!=NULL; var = deref_var(var->next)){

            set_var(var1, strdup(var->name), (var));
           // printf("merged %s\n", var->name);
        }
    }

    void unmerge(dreamObj * var1, dreamObj * var2){
        dreamObj * var;
        for (var = deref_var(var2->first_var); var!=NULL; var = deref_var(var->next)){

            dreamObj* remove = get_var(var1, var->name);
            remove->name = "_";
           // printf("merged %s\n", var->name);
        }
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


