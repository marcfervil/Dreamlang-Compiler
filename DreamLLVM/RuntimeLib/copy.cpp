//
//  copy.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "copy.hpp"
extern "C" {

dreamObj* copy(dreamObj * obj){
    //printf("COPYING OBJ OF TYPE %s\n", rep(obj->type));
    if(obj->type == dreamFuncType)return shallow_copy(obj);
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

dreamObj* medium_copy(dreamObj * obj){
    //printf("COPYING OBJ OF TYPE %s\n", rep(obj->type));
    if(obj->type == dreamFuncType)return shallow_copy(obj);
    if(obj==NULL || obj==nullDream ||obj->type == dreamFuncType)return obj;
    
    
    
    dreamObj *np = make_dream(copy_value(obj->value,  obj->type), obj->type);
    

    dreamObj *var;
   // print(1, deref_var(obj->first_var));
    for (var = deref_var(obj->first_var); var!=NULL; var = deref_var(var->next)){
      
        set_var(np, strdup(var->name), copy(var));
    }
    
   // print(1, np->type);
    return np;
}

dreamObj * smart_copy(dreamObj * obj ){
    return (obj->type == dreamObjType)? shallow_copy(obj) : deep_copy(obj);
    
}

dreamObj * shallow_copy(dreamObj * obj, bool parent ){
    //return obj;
   // printf("shallowm");
  //  dict(obj);
   // print(1,obj);
  //  if(obj->is_list)printf("oooh we got a list printer\n");
    if(obj==NULL || obj==nullDream)return obj;
    if(obj->name !=NULL && !(strcmp(obj->name,"this")==0 || obj->name[0]=='@')){
        //printf("THEM: \n");
        //dict(obj);
    }
    
    
    
    dreamObj *np = make_dream(obj->value, obj->type);
    np->is_list = obj -> is_list;
    np->ref = (obj->ref == nullDream) ? obj : obj->ref;
    /*
    if(obj->is_list){
        int len = * ((int *)(get_var(obj, "len") -> value));
     //   dreamObj** list = (dreamObj **)calloc(len + 1, sizeof(struct dreamObj *));
        for(int i=0;i<len;i++){
            ((dreamObj **)np->value)[i] = ((dreamObj **)obj->value)[i];
        }
    }*/
   // memcpy(void *__dst, const void *__src, size_t __n)
   // dreamObj** new_type = &(obj->type);
   // np->type = (dreamObj *)new_type;
    
  //  (np->value) = &obj->value;
  

    if(parent)np->parent_scope = obj->parent_scope;
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
    if(obj->name !=NULL && !(strcmp(obj->name,"this")==0 || obj->name[0]=='@')){
        //printf("ME: \n");
        //dict(obj);
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
    //print(1, obj->type);
    //printf("%s\n",obj->name);
    dreamObj *np = make_dream(copy_value(obj->value,  obj->type), obj->type);
    if(obj->name!=NULL){
        np->name = strdup(obj->name);
    }

    dreamObj *var;
    
   // np->vars = obj->vars;
//  np->parent_scope = deep_copy(obj->parent_scope);
    //memcpy(np->vars, obj->vars, sizeof(obj->vars));
    
    
    for (var = deref_var(obj->first_var); var!=NULL; var = deref_var(var->next)){
        //printf("name: %s\n", var->name);
        if(strcmp(var->name, "this")==0)continue;
        //printf("copying: %s\n", var->name);
        if(var->name!=NULL && var->name[0]=='@'/* || strcmp(var->name, "this")==0)*/){
           // printf("red flag ");
            set_var(np, strdup(var->name), medium_copy(var));
            continue;
        }
            
        //char * new_name=strdup(var->name);
        //new_name[0] = 'e';
        set_var(np, strdup(var->name), deep_copy(var));
        //printf("");
    }
    
  //  set_var(np, "shallow", dreamBool(1));
    return np;
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
}
