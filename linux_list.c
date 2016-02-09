#include <stdlib.h>

/* do not use built-in stuff */
#define offset_of(type, member)   (char *)(&((type *)0)->member)

#define container_of(ptr, type, member) \
 ((type *)((char*)ptr - offset_of(type, member)))


typedef struct list_node {
   struct list_node *prev;
   struct list_node *next;
} list_node_t; 

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

typedef struct list_entry {
          int         value_lentry;
          list_node_t link_lentry;
} list_entry_t;

typedef struct list_ctx
{
      int         count_lctx;
      list_node_t link_lctx;
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
   list_node_t  *head = &lctx->link_lctx;
   list_node_t  *iter;
   
   iter = head;
   
   while (iter->next != head)
   {
      list_entry_t * lentry ;
      
      /* get the structure base address from the member */
      lentry = container_of(iter, list_entry_t, link_lentry);
      
      printf("value %d\n", lentry->value_lentry);
      
      iter = iter->next;
   }
}

app_list_destroy(list_ctx_t *lctx)
{
  //assert(lctx->count_lctx == 0);
  
  free(lctx);
}

#define      app_list_entries_incr(lctx) ({ lctx->count_lctx++; })
list_entry_t * app_entry_alloc()
{
  list_entry_t *lentry;
  
  lentry = malloc(sizeof (list_entry_t));

  if (!lentry)
  {
     perror("malloc");
     return NULL;
  }
  
  return lentry;
}

int
app_entry_free(list_entry_t *lentry)
{
   LIST_INIT(&lentry->link_lentry);
   free(lentry);
}

int main(int argc, char *argv[])
{
   int value;
   list_ctx_t *lctx;
   
   lctx = app_list_create();
   
   for (value = 0; value < 50; value++)
   {
      list_entry_t *lentry;
      lentry = app_entry_alloc();
      lentry->value_lentry = value;
      LIST_INIT(&lentry->link_lentry);
      list_insert_head(&lctx->link_lctx, &lentry->link_lentry);
      app_list_entries_incr(lctx);
   }
   
   app_list_print(lctx);
   
   app_list_destroy(lctx);

  
  exit(0);
}
