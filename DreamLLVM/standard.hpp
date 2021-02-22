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

#define HASHSIZE 101

extern "C" {
typedef struct dreamObj{
    
    
   
    const char * name;
    dreamObj *next;
    
    void * value;
    
 //
    dreamObj * type ;
    dreamObj * vars [HASHSIZE];
    // etc..
} dreamObj;


dreamObj * make_dream(void * value, dreamObj * type = nullptr);


    dreamObj * nullDream = NULL;
    dreamObj * dreamStrType = make_dream((void *)"<Str Type>");
    dreamObj * dreamIntType = make_dream((void *)"<Int Type>");
    dreamObj * dreamFuncType = make_dream((void *)"<Func Type>");
    dreamObj * dreamType = make_dream(nullptr);


    dreamObj * make_dream(void * value, dreamObj * type){
        if (type == nullptr)type = nullDream;
        dreamObj *new_obj;
        
        new_obj = (dreamObj *) malloc(sizeof(struct dreamObj));
        new_obj -> value = (value);
        new_obj -> type = type;

        
        for(int i=0; i<HASHSIZE; i++){
            new_obj -> vars[i] = NULL;
        }
        
        return new_obj;
    }



    dreamObj * dreamStr(const char * value){
        
        dreamObj * obj = make_dream((void *)value, dreamStrType);
        return obj;
    }

    dreamObj * dreamInt(int value){

        dreamObj * obj = make_dream((void *)value, dreamIntType);
        
        return obj;
    }

    dreamObj * dreamFunc(void * value){

        dreamObj * obj = make_dream((void *)value, dreamFuncType);
        
        return obj;
    }

    void print(dreamObj* obj){
        if (obj == NULL){
            printf("[Dream]: <Undefined> \n");
            return;
        }
        if(obj->type == dreamStrType || obj->type == dreamType){
            printf("[Dream]: %s\n", (const char *)obj->value);
        }else if(obj->type == dreamIntType){
            printf("[Dream]: %d\n", (int *)obj->value);
        }else if(obj->type == dreamFuncType){
                printf("[Dream]: <Function %s>\n", obj->name);
        }else{
            printf("[Dream]: didn't print type \n");
        }
    }


    unsigned hash_obj(const char *s){
        unsigned hashval;
        for (hashval = 0; *s != '\0'; s++)
          hashval = *s + 31 * hashval;
        return hashval % HASHSIZE;
    }

    dreamObj * get_var(dreamObj * obj, const char *s){
        dreamObj *np;
  
        for (np = obj->vars[hash_obj(s)]; np!=NULL; np = np->next){
           // dreamObj * t = obj->vars[hash_obj(s)];
            
            if (np->name != NULL && strcmp(s, np->name) == 0){
                return np; // found
            }
        }
        return NULL; // not found
    }


    struct dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value){
        dreamObj *np;
        unsigned hashval;
        if ((np = get_var(obj, name)) == NULL) { // not found
            np = ( dreamObj *) malloc(sizeof(*np));
            //np = new dreamObj();
           
            hashval = hash_obj(name);
           // printf("%d",value->next);
            value -> next = obj->vars[hashval];
            value->name = strdup(name);
           // value -> name =  name;
          //  printf("good check %d\n",hashval);
            obj->vars[hashval] = value;
        } else{ //already there
            //free[](np->vars);
            // np->name = "e";
            
            free(np); //free previous defn
            np = ( dreamObj *) malloc(sizeof(*np));
            //np = new dreamObj();
    
            hashval = hash_obj(name);
            value->name = strdup(name);
           // printf("%d",value->next);
            value -> next = obj->vars[hashval];
            obj->vars[hashval] = value;
           // value -> name =  name;
        }
        return np;
    }

    


}


