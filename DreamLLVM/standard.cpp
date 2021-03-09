/*
//
//  standard.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 2/15/21.
//

#include "standard.hpp"


extern "C" {


    dreamObj * nullDream = NULL;
    dreamObj * dreamStrType = make_dream((void *)"<Str Type>");
    dreamObj * dreamIntType = make_dream((void *)"<Int Type>");

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

    void print(dreamObj* obj){
        if (obj == NULL){
            printf("[Dream]: <Undefined> \n");
            return;
        }
        if(obj->type == dreamStrType || obj->type == dreamType){
            printf("[Dream]: %s\n", (char *)obj->value);
        }else if(obj->type == dreamIntType){
            printf("[Dream]: %d\n", (int *)obj->value);
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
            if (np == NULL || (np->name = strdup(name)) == NULL){
               
              return NULL;
            }
            hashval = hash_obj(name);
            value -> next = *(obj->vars[hashval]);
            value ->name = (char *)name;
          //  printf("good check %d\n",hashval);
            *(obj->vars[hashval]) = value;
        } else //already there
            free((void *) np->value); //free previous defn
        if ((np->value = value) == NULL){
            printf("null check");
           return NULL;
        }
        return np;
    }

    


}
*/
