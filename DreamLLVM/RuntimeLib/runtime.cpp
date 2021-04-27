//
//  runtime.cpp
//  DreamLLVM
//
//  Created by Marc Fervil on 4/15/21.
//

#include "runtime.hpp"

extern "C" {

dreamObj * dreamType = make_dream(nullptr);
dreamObj * dreamObjType = make_dream((void *)"<Obj Type>",dreamType);
dreamObj * dreamStrType = make_dream((void *)"<Str Type>",dreamType);
dreamObj * dreamIntType = make_dream((void *)"<Int Type>",dreamType);
dreamObj * dreamListType = make_dream((void *)"<List Type>",dreamType);
dreamObj * dreamBoolType = make_dream((void *)"<Bool Type>",dreamType);
dreamObj * dreamFuncType = make_dream((void *)"<Func Type>",dreamType);
dreamObj * dreamPointerType = make_dream((void *)"<Pointer Type>",dreamType);
dreamObj * nullDream = make_dream((void *) nullCall, nullDream);


int line = 1;


bool undefined_allowed = false;

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

    new_obj -> pointer = 0;
    new_obj -> is_inherited = 0;
   
    for(int i=0; i<HASHSIZE; i++){
        (new_obj->vars[i]) = (dreamObj **)malloc(sizeof(struct dreamObj *));
        *(new_obj->vars[i]) = NULL;
    }
    
    return new_obj;
}

dreamObj ** pointer_init(){
    dreamObj ** ptr = (dreamObj **)malloc(sizeof(struct dreamObj *));
    *(ptr) = NULL;
    return ptr;
}

void nullCall(){
    nightmare("Cannot call undefined!");
}

void nightmare(const char * message, ...){
  
    va_list arglist2;
    va_start( arglist2, message );
    char* str_ref1;
    vasprintf(&str_ref1, message, arglist2);
    va_end( arglist2 );
 
    
    char * str ;
    asprintf (&str, "%s on line %i", str_ref1, line);
    log_error(str);
    
}

void log_error(const char * error){
    printf("\x1B[31m[Nightmare]: %s \n\033[0m",error);
    exit(1);
}

void super(dreamObj *context){
    dreamObj * self = context->parent_scope;
    dreamObj * og_parent =  get_var(self, "obj")->parent_scope;
    dreamObj * me = new_scope(get_var(self, "obj"), 1);

            //get_var(self, "obj");
    merge(get_var(self, "obj"),
          (((dreamObj* (*)(dreamObj *))get_var(self, "parent")->value)(me)),
          false);

  //  get_var(self, "parent")->parent_scope = og_parent;
   // *(get_var(self, "obj")->parent_scope) = *(((dreamObj* (*)(dreamObj *))get_var(self, "parent")->value)(og_parent->parent_scope));
}

void inherit(dreamObj * obj, dreamObj * parent){
   // obj->is_inherited = 1;
    //parent->parent_scope = obj->parent_scope;
//    obj->parent_scope = parent;

    dreamObj * inherit_data = dreamObject();
    set_var(inherit_data, "parent", parent);
    set_var(inherit_data, "obj", obj);
    dreamObj * inherit_super = dreamFuncWithContext((void *) super, inherit_data);
    set_var(obj, "super", inherit_super);
}



const char * rep(dreamObj* obj){

    if (obj==nullDream) return "<Undefined>";
    if (obj->type==NULL) return "<NULL type>";
 
    dreamObj * type = obj->type;
    void * value = obj->value;
    if(type == dreamStrType || type == dreamType){
        
        return (( char *)value);
    }else if(type == dreamIntType){
        int * num = (int *)value;
        
        char * str;
        asprintf(&str, "%i", *num);
        
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
        char * str;
        asprintf(&str, "<Function %s>", name);
       // free(str_ref);
        return str;
    
    }else if(type == dreamObjType){
        dreamObj* rep_func;
        
        if((rep_func = find_var(obj, "repr"))!=nullDream){
            dreamObj * rep_call = ((dreamObj* (*)(dreamObj *)) rep_func->value)(obj);
            
            return ( char *)rep_call->value;
        }else{
            return("<Object>");
        }
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

dreamObj * set_parent(dreamObj * obj, dreamObj * new_parent){
    obj->parent_scope = new_parent;
    return obj;
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

void dict2(dreamObj* obj){

    dict(obj);

}


void merge(dreamObj * var1, dreamObj * var2, bool override){
    dreamObj * var;

    for (var = deref_var(var2->first_var); var!=NULL; var = deref_var(var->next)){
        if(!override && find_var(var1, var->name)!=nullDream)continue;

        dreamObj * new_var = set_var(var1, strdup(var->name), var);
        /*when merging functions onto another object,
         * make sure to supply them with the proper context of the object they were merged onto*/
        if(new_var->type == dreamFuncType){
            set_var(new_var, "@context", var1);
        }

    }
}

dreamObj * dreamObject(){
    return make_dream(NULL, dreamObjType);
}



dreamObj * new_scope(dreamObj * obj, int nested_scope){
   // printf("%d",obj->parent_scope == &nullDream);
 

    if((obj->parent_scope == nullDream || nested_scope)){
        //nested scope
        dreamObj * new_scope = make_dream(nullDream);

        new_scope->parent_scope = (obj);
      //  print(2, dreamStr("scope: "), obj);

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


dreamObj * contains_c(dreamObj * var1, dreamObj * var2){
    //printf("here!\n");
   // if(var1 == nullDream)return dreamBool(var1==nullDream && var2==nullDream);
    
    dreamObj * cont;
    if((cont = find_var(var1, "contains")) != nullDream){
        
        dreamObj * b = ((dreamObj* (*)(dreamObj *, dreamObj *)) cont->value)(var1, var2);

        return b;
    }
    //TODO - some sort of equals inheritance for objects.  Deffering because I dont want to allocate more memory.
    else if(var1->type==dreamStrType) return dreamBool(find_var(var2, (const char *)var1->value)!=nullDream);
    nightmare("Undefined contains operation (or something like that, TODO: better error msg)");
    return dreamBool(-1);
}

dreamObj * equals_c(dreamObj * var1, dreamObj * var2){
    if(var1 == nullDream)return dreamBool(var1==nullDream && var2==nullDream);
    
    dreamObj * equ;
    if((equ = find_var(var1, "equals")) != nullDream){

        dreamObj * b = ((dreamObj* (*)(dreamObj *, dreamObj *)) equ->value)(var1, var2);

        return b;
    }
    //TODO - some sort of equals inheritance for objects.  Deffering because I dont want to allocate more memory.
    else if(var1->type==dreamObjType) return dreamBool(var1->value == var2->value);
    printf("[Nightmare]: <Undefined Equals Operation>\n");
    return dreamBool(-1);
}


dreamObj * add_c(dreamObj * var1, dreamObj * var2){
    
    dreamObj * equ;
    if((equ = get_var(var1, "add")) != NULL){
      //  dict(var1);
        //dict(var2);
        dreamObj * result = ((dreamObj* (*)(dreamObj *, dreamObj *)) equ->value)(var1, var2);
        
        return result;
    }
    printf("[Nightmare]: <Undefined Add Operation>\n");
    return dreamBool(-1);
}

}
