

#include "count.h"
extern "C" {

dreamObj *count(dreamObj *num) {

    dreamObj *count_obj = dreamObject();
    add_native_func(count_obj, "iter", (void *) count_iter);
    set_var(count_obj, "num", num);

    return count_obj;
}

dreamObj *count_iter(dreamObj *scope) {
    dreamObj *self = scope->parent_scope;
    dreamObj *iter = dreamObject();
    set_var(iter, "max", get_var(self, "num"));


    set_var(iter, "index", dreamInt(0));

    dreamObj *next_func = dreamFunc((void *) count_iter_next);
    set_var(next_func, "@context", iter);
    set_var(iter, "next", next_func);

    return iter;
}

dreamObj *count_iter_next(dreamObj *scope) {

    dreamObj *self = scope->parent_scope;
    dreamObj *index = get_var(self, "index");
    dreamObj *max = get_var(self, "max");
    int val = (*(int *) (index->value));
    (*(int *) index->value) = val + 1;

    if (val >= (*(int *) (max->value)))return nullDream;

    return dreamInt(val);

}
}