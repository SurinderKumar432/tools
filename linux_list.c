
typedef struct list_node {
   struct list_node *prev;
   struct list_node *next;
} list_node_t; 

#define LIST_INIT(node)  { node->prev = &node; node->next = &node; }

#define list_insert_internal(prev, next, new) \
do {                                          \
  prev->next = new;                             \
  next->prev = new;                             \
  new->prev = prev;                             \
  new->next = next;                             \
} while(0)

#define list_insert_head(list_node_t *head, list_node_t *new)  list_insert_internal(head, head->next, new)
#define list_insert_tail(list_node_t *head, list_node_t *new)  list_insert_internal(head->prev, head, new)

#define list_delete_internal(prev, next) \
do {                                      \
   prev->next = next;                    \
   next->prev = prev;                     \
   } while (0)
  
#define list_delete(list_node_t *node)  \
do {                                   \
        list_delete_internal(node->prev, node->next) ;
        list_node_init(node);
} while (0) 



typedef struct list_entry {
          int         value_lentry;
          link_node_t link_lentry;
} list_entry_t;

typedef struct list_ctx
{
      int         count_lctx;
      link_node_t link_lctx;
} list_ctx_t;
  
list_ctx_t *app_list_create()
{
  list_ctx_t *lctx;
  
  lctx = malloc(sizeof list_ctx_t);
  if (!lctx)
  {
     perror("malloc");
     return NULL;
  }
  
  lctx->count_lctx = 0;
  LIST_INIT(lctx->link_lctx);
  
  return lctx;
}

void
app_list_print(list_ctx_t *lctx)
{
   
}

app_list_destroy(list_ctx_t *lctx)
{
  assert(lctx->count_lctx == 0);
  
  free(lctx);
}

#define      app_list_entries_incr(lctx) lctx->count_lctx++


list_entry_t * app_entry_alloc()
{
  list_entry_t *lentry;
  
  lentry = malloc(sizeof lentry)
  if (!entry)
  {
     perror("malloc");
     return NULL;
  }
  
  return lentry;
}

int
app_entry_free()
{
   LIST_INIT(lentry->link_lentry);
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
      LIST_INIT(lentry->link_lentry);
      list_insert(&lctx->link_lctx, &lentry->link_lentry);
      app_list_entries_incr(lctx);
   }
   
   app_list_print(lctx);
   
   app_list_destroy(lctx);

  
  exit(0);
}
