#ifndef VM_LIST_H__
#define VM_LIST_H__

/*
 * List declarations.
 */
#define LIST_HEAD(name, type)                                           \
struct name {                                                           \
        struct type *lh_first;  /* first element */                     \
}

#define LIST_HEAD_INITIALIZER(head)                                     \
        { NULL }

#define LIST_ENTRY(type)                                                \
struct {                                                                \
        struct type *le_next;   /* next element */                      \
        struct type **le_prev;  /* address of previous next element */  \
}


/*
 * List functions.
 */

#define LIST_EMPTY(head)        ((head)->lh_first == NULL)

#define LIST_FIRST(head)        ((head)->lh_first)

#define LIST_FOREACH(var, head, field)                                  \
        for ((var) = LIST_FIRST((head));                                \
            (var);                                                      \
            (var) = LIST_NEXT((var), field))

#define LIST_INIT(head) do {                                            \
        LIST_FIRST((head)) = NULL;                                      \
} while (0)
#define LIST_INSERT_AFTER(listelm, elm, field) do {                     \
        if ((LIST_NEXT((elm), field) = LIST_NEXT((listelm), field)) != NULL)\
                LIST_NEXT((listelm), field)->field.le_prev =            \
                    &LIST_NEXT((elm), field);                           \
        LIST_NEXT((listelm), field) = (elm);                            \
        (elm)->field.le_prev = &LIST_NEXT((listelm), field);            \
} while (0)

#define LIST_INSERT_BEFORE(listelm, elm, field) do {                    \
        (elm)->field.le_prev = (listelm)->field.le_prev;                \
        LIST_NEXT((elm), field) = (listelm);                            \
        *(listelm)->field.le_prev = (elm);                              \
        (listelm)->field.le_prev = &LIST_NEXT((elm), field);            \
} while (0)

#define LIST_INSERT_HEAD(head, elm, field) do {                         \
        if ((LIST_NEXT((elm), field) = LIST_FIRST((head))) != NULL)     \
                LIST_FIRST((head))->field.le_prev = &LIST_NEXT((elm), field);\
        LIST_FIRST((head)) = (elm);                                     \
        (elm)->field.le_prev = &LIST_FIRST((head));                     \
} while (0)

#define LIST_NEXT(elm, field)   ((elm)->field.le_next)

#define LIST_REMOVE(elm, field) do {                                    \
        if (LIST_NEXT((elm), field) != NULL)                            \
                LIST_NEXT((elm), field)->field.le_prev =                \
                    (elm)->field.le_prev;                               \
        *(elm)->field.le_prev = LIST_NEXT((elm), field);                \
} while (0)

#endif /* VM_LIST_H__ */
