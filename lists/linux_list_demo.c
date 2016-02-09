#include <stdlib.h>

/* do not use built-in stuff */
#define offset_of(type, member)   (char *)(&((type *)0)->member)

#define container_of(ptr, type, member) \
 ((type *)((char*)ptr - offset_of(type, member)))

#define list_entry(ptr, type, member) container_of(ptr, type, member)

typedef struct list_head {
   struct list_head *prev;
   struct list_head *next;
} list_head_t; 

#define LIST_INIT(_node) \
do {    \
 (_node)->prev = (_node); \
 (_node)->next = (_node); \
} while (0)


#define list_insert_internal(_prev, _next, _new)      \
do {                                                  \
  (_new)->prev = (_prev);                             \
  (_new)->next = (_next);                             \
  (_prev)->next = (_new);                             \
  (_next)->prev = (_new);                             \
} while (0)

#define list_insert_head(_head, _new)  \
        list_insert_internal((_head), (_head)->next, (_new))

#define list_insert_tail(_head, _new)  \
        list_insert_internal((_head)->prev, (_head), (_new))

#define list_delete_internal(_prev, _next) \
do {                                     \
   (_prev)->next = (_next);                \
   (_next)->prev = (_prev);                \
} while (0)
  
#define list_delete(_node)                                  \
do {                                                        \
        list_delete_internal((_node)->prev, (_node)->next) ;  \
        LIST_INIT((_node));                                  \
} while (0)
typedef struct list_node {
          int         value_lnode;
          list_head_t link_lnode;
} list_node_t;

typedef struct list_ctx
{
      int         count_lctx;
      list_head_t link_lctx;
} list_ctx_t;
  
list_ctx_t *app_list_create()
{
  list_ctx_t *lctx;
  
  lctx = malloc(sizeof(list_ctx_t));
  if (!lctx)
  {
     perror("malloc");
     return NULL;
  }
  
  lctx->count_lctx = 0;
  LIST_INIT(&lctx->link_lctx);
  
  return lctx;
}

void
app_list_print(list_ctx_t *lctx)
{
   list_head_t  *head = &lctx->link_lctx;
   list_head_t  *iter;
   
   iter = head->next;
   
   while (iter != head)
   {
      list_node_t * lnode ;
      
      /* get the structure base address from the member */
      lnode = container_of(iter, list_node_t, link_lnode);
     
      printf("value %d\n", lnode->value_lnode);

#ifdef DEBUG
      printf("prev %x next %x\n", iter->prev, iter->next);
#endif /* DEBUG */
      
      iter = iter->next;
   }
}
app_list_destroy(list_ctx_t *lctx)
{
  list_head_t *head = &lctx->link_lctx;
  list_head_t *iter;

  //assert(lctx->count_lctx == 0);

  iter = head->next;
  while (iter != head)
  {
    list_node_t *lnode;
    list_head_t *iter_next;

    iter_next = iter->next;

    list_delete(iter);

    lnode = list_entry(iter, list_node_t, link_lnode);

    app_entry_free(lnode);

    iter = iter_next;
  }

  free(lctx);
}

#define      app_list_entries_incr(lctx) ({ lctx->count_lctx++; })



list_node_t * app_entry_alloc()
{
  list_node_t *lnode;
  
  lnode = malloc(sizeof (list_node_t));

  if (!lnode)
  {
     perror("malloc");
     return NULL;
  }
  
  return lnode;
}

int
app_entry_free(list_node_t *lnode)
{

   printf("freeing value %d\n", lnode->value_lnode);

   LIST_INIT(&lnode->link_lnode);
   free(lnode);
}

int main(int argc, char *argv[])
{
   int value;
   list_ctx_t *lctx;
   
   lctx = app_list_create();
   
   for (value = 0; value < 50; value++)
   {
      list_node_t *lnode;
      lnode = app_entry_alloc();
      lnode->value_lnode = value;
      LIST_INIT(&lnode->link_lnode);
      list_insert_head(&lctx->link_lctx, &lnode->link_lnode);
      app_list_entries_incr(lctx);
   }
   
   app_list_print(lctx);
   
   app_list_destroy(lctx);

  exit(0);
}
