//
//  memory.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "memory.hpp"

extern "C" {
dreamObj * get_var(dreamObj * obj, const char *name, int from_parent){

    if(strcmp(name, "null")==0){
        //undefined_allowed = !undefined_allowed;
        return nullDream;
    }
    
    dreamObj * found_obj;
    if((found_obj = find_var(obj, name, from_parent))!=nullDream)return found_obj;
    if(!undefined_allowed){
        
        char * str;
       // android_log(name);
       // dict(obj);
        asprintf(&str, "Variable '%s' is not defined", name);
        
        //log_error(str);
        nightmare(str);
    }
    return nullDream; // not found
}


dreamObj* find_var(dreamObj * obj, const char *s, int from_parent){

    if(obj == nullDream /*|| obj==temp*/){
        //printf("[Nightmare]: Cannot get property %s from undefined!\n",s);
        char * str;
        asprintf(&str, "Cannot get property %s from undefined", s);
        nightmare(str);
        //exit(1);
        //dream_log(dreamStr("ddd"));
    }
    if(strcmp(s, "parent") == 0)return obj->parent_scope;
    if(strcmp(s, "type") == 0)return obj->type;
    if(strcmp(s, "$name") == 0)return (obj->name != NULL) ? dreamStr(obj->name) : nullDream;
    if(strcmp(s, "line") == 0)return dreamInt(line);
    if(strcmp(s, "self") == 0)return obj;
    dreamObj * np;
   
    for (np = deref_var(obj->vars[hash_obj(s)]); np!=NULL; np = deref_var(np->next)){
   
       // printf("searching: %s\n",s);
        if (np->name != NULL && strcmp(s, np->name) == 0){
            
            /*
            if(np->type == dreamObjType){
                dreamObj * cp =  shallow_copy(np, false);
               cp->parent_scope = nullDream;
                return cp;
            }*/
           
            return np; // found
        }
    }
    
    if(from_parent!=-1 && obj->parent_scope != nullDream && strcmp(s,"scope")!=0 && s[0]!='@' ){
        //printf("up-get: %s\n",s);
        if(from_parent)
            return find_var(obj->parent_scope, s);
        else
            return find_var(obj->parent_scope, s, -1);
    }

    return nullDream;
}


unsigned hash_obj(const char *s){
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}



struct dreamObj *set_var(dreamObj *obj, const char *name, dreamObj *value){
    //printf("\x1B[31m open");
   // printf("Setting var type - %s", rep(value->type));
  
    
    if(obj == nullDream){
        nightmare("Cannot set property %s on undefined!", name);
        
        
    }
    
    if(strcmp(name, "undef")==0){
        undefined_allowed = *(int *)(value -> value);
        
    }
    
    if(strcmp(name, "this")!=0 && name[0]!='@' && obj->parent_scope != nullDream && find_var(obj->parent_scope, name)!=nullDream){
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



struct dreamObj *set_var_soft(dreamObj *obj, const char *name, dreamObj *value){
   
    unsigned hashval;
   
    hashval = hash_obj(name);
    
    //printf("setting %s onto %s\n",rep(value),rep(obj));
    
    /*
    if(temp!=NULL){
        printf("TEMP REASS\n");
    }*/
   
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
      //  if(strcmp(name, "other")==0)dict(obj);
        if(found->name != NULL && strcmp(found->name, name)!=0){
            
            printf("TODO: FIX HASH COLLISION POINTER BUG - %s COLLIDES WITH %s on %s\n", name, (*(obj->vars[hashval]))->name, obj->name);
            //exit(0);
            
            
            // THIS WAS WORKING FIX IT
            dreamObj * next = *((*(obj->vars[hashval]))->next);
           // printf("1\n");
            //print(1,next);
            
            dreamObj * new_obj = (value->type != dreamObjType) ? copy(value) : shallow_copy(value);
         
            
            
            new_obj -> name = strdup(name);
            *((*(obj->vars[hashval])))->next = new_obj;
            
            
            *((*(*(obj->vars[hashval]))->next)->next)= next;
           // printf("4\n");
            if((obj->vars[hashval])==(obj->last_var)){
                obj->last_var =(*(obj->vars[hashval]))->next ;
            }
            
           // printf("didntseg");
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
        if(found == nullDream){
            printf("nullre");
            dreamObj * new_value = copy(value);
            new_value ->name = strdup(name);
            *(obj->vars[hashval]) = new_value;
        }else if(value->type == dreamObjType){
          //  printf("\nstarting free1");
           // printf("TODO: Proper Object reassignment\n");
           // free((*(obj->vars[hashval]))->value);
           // printf("\nfreed 1");
            //dict(value );
            
            //LALALA
            
            (*(obj->vars[hashval])) -> value = value->value;
            (*(obj->vars[hashval])) -> name = strdup(name);
            
            (*(obj->vars[hashval])) -> type = value->type;
            ((*(obj->vars[hashval]))->first_var) = (value->first_var);
            
            for(int i=0; i<HASHSIZE; i++){
               // if()
               // set_var(np, strdup(var->name), (var));
                (*(obj->vars[hashval]))->vars[i] = (value->vars[i]);
                
                //value->vars[i];
                //pointer_init();
                //(obj->vars[i]);
             
            }
            
            /*
            for (dreamObj* var = deref_var(value->first_var); var!=NULL; var = deref_var(var->next)){
                if(var->name != NULL && strcmp(var->name, "this")==0)continue;
               set_var((*(obj->vars[hashval])), strdup(var->name), (var));
            }
            */
          //  (*(obj->vars[hashval])) = shallow_copy(value);
          //  (*(obj->vars[hashval])) ->name = name;
            
            //memcpy((*(obj->vars[hashval]))->vars, value->vars, sizeof((*(obj->vars[hashval]))->vars));
            //printf("here\n");
            
           //
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
           // printf("is null: %d\n",found==nullDream);
            if(found!=nullDream && found->type!=dreamObjType && found->type!=dreamFuncType){
                free(found->value);
            }
            found->value = copy_value(value->value,  value->type);

            found->name = strdup(name);
            (*(obj->vars[hashval]))->type = value->type;
            (*(obj->vars[hashval]))->first_var = value->first_var;
            for(int i=0; i<HASHSIZE; i++){
               // if()
               // set_var(np, strdup(var->name), (var));
                (*(obj->vars[hashval]))->vars[i] = (value->vars[i]);
                
                //value->vars[i];
                //pointer_init();
                //(obj->vars[i]);
             
            }
            
            /*
            for (dreamObj* var = deref_var(value->first_var); var!=NULL; var = deref_var(var->next)){
              
               set_var((*(obj->vars[hashval])), strdup(var->name), (var));
            }
           */
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
        }else if(name[0]=='@' || strcmp(name, "this")==0 || value->type==dreamObjType){
           // printf("deep_dish");
            
            dreamObj * new_value = shallow_copy(value) ;
            //if(by)printf("by %s", value->name);
            //apples
            
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
            
            dreamObj * new_value = copy(value);
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
        
        *(obj->first_var) = *(obj->vars[hashval]) ;
 
      //  printf("Assigned frist var on %s to %s\n", rep(obj), rep(*(obj->first_var)));
    }
    if(deref_var(obj->last_var) != NULL){
        *(( *(obj->last_var ))-> next) =  *(obj->vars[hashval]) ;
    }
    *(obj->last_var) =  *(obj->vars[hashval] );
    
 
        //obj->last_var ->next = nullDream;
        //obj->last_var = nullDream;
        //(*(obj->vars[hashval]))->name = "fewfe";

    return nullptr;
}

}
