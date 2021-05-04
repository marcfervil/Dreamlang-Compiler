

#include "count.h"
extern "C" {


typedef struct countInfo{

    dreamObj * index;

    int max;

} countInfo;

dreamObj *count(dreamObj *num) {

    dreamObj *count_obj = dreamObject();
    add_native_func(count_obj, "iter", (void *) count_iter);
    countInfo * count_data = (countInfo *) malloc(sizeof (struct countInfo));

    count_data -> index = dreamInt(-1);
    count_data -> max = to_int(num)-1;

    //using an int so that I don't malloc anything...I will create a "native" type later
    dreamObj * count_data_obj = dreamInt(0);
    count_data_obj -> value = count_data;
    count_data_obj -> type = dreamObjType;

    set_var(count_obj, "countInfo", count_data_obj);

    return count_obj;
}

dreamObj *count_iter(dreamObj *scope) {
    dreamObj *self = scope->parent_scope;
    dreamObj *iter = dreamObject();
   // set_var(iter, "max", get_var(self, "num"));

    // printf("got iter\n");
    set_var(iter, "countInfo", get_var(self, "countInfo"));

    dreamObj *next_func = dreamFunc((void *) count_iter_next);
    set_var(next_func, "@context", iter);
    set_var(iter, "next", next_func);

    return iter;
}

dreamObj *count_iter_next(dreamObj *scope) {
   // printf("next iter\n");
    dreamObj *self = scope->parent_scope;
    countInfo *count_info = (countInfo *) get_var(self, "countInfo")->value;

    dreamObj *index = count_info->index;
    //dreamObj *max = get_var(self, "max");
    int val = (*(int *) (index->value));
    //printf("%s ",rep(index));
    (*(int *) index->value) += 1;

    if (val >= count_info->max)return nullDream;
    //printf("%s\n ",rep(index));
    return index;
    //return dreamInt(val);

}
}