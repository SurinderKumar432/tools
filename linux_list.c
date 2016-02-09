
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


typedef uint slot_t;
typedef uint key_t;

struct hash_table_entry {
          int         key_hte;
          int         value_hte;
          link_node_t link_hte;
};


#define HASH_SIZE(ht) (ht->slots_htbl)
#define HASH_FUNC(ht, key)  (key % HASH_SIZE(ht))

struct htbl
{
      int         count_htbl;
      int         slots_htbl;
      link_node_t *link_htbl;
};
  
htbl *ht_create(int num_slots)
{
  htbl *ht;

  ht = malloc(sizeof hash_table)
  if (!ht)
  {
     perror("malloc");
     return NULL;
  }
  
  ht->link_ht = malloc(sizeof (link_node_t) * num_slots);
  
  if (!ht->links_ht)
  {
     perror("malloc");
     free(ht);
     return NULL;
  }
  
  ht->slots_ht = num_slots;
  
  return ht;
}

init ht_init(htbl *ht)
{
  ht->count_ht = 0;
  
  for (slot = 0; slot < HASH_SIZE(ht); slot++)
  {
      LIST_INIT(ht->link[slot]);
  }
  
}

ht_destroy(htbl *ht)
{
  free(ht->links_ht);
  free(ht);
}

ht_entry_t * ht_alloc_entry()
{

}

ht_free_entry()
{

}

int main(int argc, char *argv[])
{


  for (key = 0; key < 100; key++)
  {
      slot = HASH_FUNC(key);
      
  }
  
  
  exit(0);
}
