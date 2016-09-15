/*
 * FILE
 *      vm_layout.c
 *
 * Description 
 *      virtual memory layout of the process on Linux Systems.
 *
 * External Variables
 *      _start
 *      __data_start
 *      __bss_start
 *      etext
 *      edata
 *      end
 * 
 * External Functions
 *
 * Global Variables
 *      heap_growth_dir
 *      stack_growth_dir
 *      mmap_growth_dir
 *
 * Global Functions
 *
 *
 * Internal Functions
 *
 *
 * Decisions
 *
 *      
 * AUHTOR
 *      - Surinder Kumar
 *
 */

#include <sys/types.h>          /* data types */
#include <stdlib.h>             /* malloc() */
#include <stdio.h>              /* printf() */
#include <string.h>             /* strcpy and bzero */
#include <unistd.h>             /* sysconf */
#include <sys/mman.h>           /* sysconf */
#include <vmlayout_global.h>

/*
objdump -x hello

BSS = __bss_start - end
Data = __data_start - edata
RO Data...
Text = _start - etext
*/

#define UP      1
#define DOWN    0

int heap_growth_dir;
int stack_growth_dir;
int mmap_growth_dir;

extern ulong etext, edata, end;
extern ulong _start, __data_start, __bss_start;

const int const_v = 1;
int glob_v = 2;

int bss_v;

#define STACK_VARIABLE  &stack_v

#define DATA_BSS_VARIABLE       &bss_v

#define DATA_GLOBAL     &glob_v
#define DATA_CONST      &const_v

#define TEXT_MAIN       &main

#define PTR_SIZE_WIDTH  2*sizeof(void *)

LIST_HEAD(listhead, entry) vmap_list;

struct entry {
        char *name;
        void *addr;
        LIST_ENTRY(entry) entries;
};

struct entry *
alloc_node()
{
        struct entry *node_p;

        node_p = (struct entry *)malloc(sizeof(struct entry));
        if (!node_p) {
                perror("malloc()");
                exit(1);
        }

        bzero((char *)node_p, sizeof(struct entry));

        return node_p;
}

struct entry *
setup_node(struct entry *node_p, char *name, void *addr)
{

#if defined(DEBUG)
        printf("node = %p\n", node_p);
        printf("name = %s, addr = %p\n", name, addr);
#endif

        node_p->name = (char *)malloc(strlen(name) + 1);
        if (!node_p->name) {
                perror("malloc()");
                exit(1);
        }

        strcpy(&node_p->name[0], name);
        node_p->addr = addr;

        return node_p;
}
void
insert_node(struct entry *node_p)
{
        struct entry *np, *prev_p;

        if (LIST_EMPTY(&vmap_list)) {

#if defined(DEBUG)
                printf("empty list \n");
                printf("inserted head = %p \n", node_p);
#endif

                LIST_INSERT_HEAD(&vmap_list, node_p, entries);
                return;
        }

        LIST_FOREACH(np, &vmap_list, entries) {
                if ((unsigned long)node_p->addr > (unsigned long)np->addr) {
                        prev_p = np;
                        continue;
                }

                LIST_INSERT_BEFORE(np, node_p, entries);
                return;
        }

        if (np == NULL) {
                LIST_INSERT_AFTER(prev_p, node_p, entries);
                return;
        }
}

void *
current_break_value()
{
        return sbrk(0);
}
void *
get_frame_address()
{
        void *fpp = 0;

#if defined(__i386)
        asm volatile ("mov %%ebp, %0":"=r" (fpp));
#elif defined(__x86_64__)
        asm volatile ("mov %%rbp, %0":"=r" (fpp));
#else
        fpp = 0xdeadbeef;
#endif

#if defined(DEBUG)
        printf("EBP = %p\n", fpp);
#endif

        return fpp;
}

void
print_list()
{
        struct entry *np;

        LIST_FOREACH(np, &vmap_list, entries) {

                if (strstr(np->name, "START") != 0) {
                        if (strstr(np->name, "HEAP") != 0) {
                                if (!heap_growth_dir) {
                                        printf("|        .      |\n");
                                        printf("|       /|\\     |\n");
                                        printf("|        |      | %s\n",
                                               "HEAP");
                                        printf("|        |      |\n");
                                }
                        }
                        if (strstr(np->name, "MMAP") != 0) {
                                if (!mmap_growth_dir) {
                                        printf("|        .      |\n");
                                        printf("|       /|\\     |\n");
                                        printf("|        |      | %s\n",
                                               "MMAP");
                                        printf("|        |      |\n");
                                }
                        }
                        if (strstr(np->name, "STACK") != 0) {
                                if (!stack_growth_dir) {
                                        printf("|        .      |\n");
                                        printf("|       /|\\     |\n");
                                        printf("|        |      | %s\n",
                                               "STACK");
                                        printf("|        |      |\n");
                                }
                        }
                }

                if (strstr(np->name, "START") != 0) {
                        printf("|---------------| %.*p  [%s]\n", PTR_SIZE_WIDTH,
                               np->addr, &np->name[0]);
                } else if (strstr(np->name, "END") == 0) {
                        printf("|               | %.*p  [%s]\n", PTR_SIZE_WIDTH,
                               np->addr, &np->name[0]);
                }
                if (strstr(np->name, "START") != 0) {
                        if (strstr(np->name, "HEAP") != 0) {
                                if (heap_growth_dir) {
                                        printf("|        |      |\n");
                                        printf("|        |      | %s\n",
                                               "HEAP");
                                        printf("|       \\|/     |\n");
                                        printf("|        v      |\n");
                                }
                        }
                        if (strstr(np->name, "MMAP") != 0) {
                                if (mmap_growth_dir) {
                                        printf("|        |      |\n");
                                        printf("|        |      | %s\n",
                                               "MMAP");
                                        printf("|       \\|/     |\n");
                                        printf("|        v      |\n");
                                }
                        }
                        if (strstr(np->name, "STACK") != 0) {
                                if (stack_growth_dir) {
                                        printf("|        |      |\n");
                                        printf("|        |      | %s\n",
                                               "STACK");
                                        printf("|       \\|/     |\n");
                                        printf("|        v      |\n");
                                }
                        }
                        if (strstr(np->name, "HEAP") == 0
                            && strstr(np->name, "MMAP") == 0
                            && strstr(np->name, "STACK") == 0) {
                                printf("|               |\n");
                        }
                }

                if (strstr(np->name, "END") != 0) {
                        printf("|               |\n");
                        printf("|---------------| %.*p  [%s]\n", PTR_SIZE_WIDTH,
                               np->addr, &np->name[0]);
                }
        }

#if defined(EXTRA)
        printf("-------------\n");
        printf("Stack grows : %s\n",
               stack_growth_dir == UP ? "UPWARDS" : "downwards");
        printf("Heap  grows : %s\n",
               heap_growth_dir == UP ? "upwards" : "DOWNWARDS");
        printf("Mmap  grows : %s\n",
               mmap_growth_dir == UP ? "upwards" : "downwards");
        printf("Current break value: %p\n", current_break_value());
        printf("-------------\n");
#endif
}

#if defined(DEBUG)
#define d_print_list() print_list()
#else
#define d_print_list()
#endif

void *
heap_base()
{
        void *segment = "heap";

        void *ptr_base, *ptr_next;
        int pagesize;

        pagesize = sysconf(_SC_PAGESIZE);

        ptr_base = malloc(pagesize);
        if (!ptr_base) {
                perror("malloc()");
                exit(1);
        }

        ptr_next = malloc(pagesize);
        if (!ptr_next) {
                perror("malloc()");
                exit(1);
        }
#if defined(EXTRA)
        printf("%s: ptr_base = %p, ptr_next = %p\n", segment, ptr_base,
               ptr_next);
#endif
        free(ptr_base);
        free(ptr_next);

        if (ptr_base > ptr_next) {
#if defined(DEBUG)
                printf("%s ... grows down (High to Low)\n", segment);
#endif
                heap_growth_dir = DOWN;
        } else {
#if defined(DEBUG)
                printf("%s ... grows up (Low to High)\n", segment);
#endif
                heap_growth_dir = UP;
        }
        return (ptr_base);
}

void *
mmap_base()
{
        void *segment = "mmap";
        void *ptr_base, *ptr_next;
        int pagesize;
        int rvalue;

        pagesize = sysconf(_SC_PAGESIZE);

        ptr_base =
            mmap(0, pagesize, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        if (!ptr_base) {
                perror("malloc()");
                exit(1);
        }

        ptr_next =
            mmap(0, pagesize, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        if (!ptr_next) {
                perror("malloc()");
                exit(1);
        }

        rvalue = munmap(ptr_base, pagesize);
        if (rvalue == -1) {
                perror("munmap()");
                exit(1);
        }

        rvalue = munmap(ptr_next, pagesize);
        if (rvalue == -1) {
                perror("munmap()");
                exit(1);
        }
#if defined(EXTRA)
        printf("%s: ptr_base = %p, ptr_next = %p\n", segment, ptr_base,
               ptr_next);
#endif

        if (ptr_base > ptr_next) {
#if defined(DEBUG)
                printf("%s ... grows down (High to Low)\n", segment);
#endif
                mmap_growth_dir = DOWN;
        } else {
#if defined(DEBUG)
                printf("%s ... grows up (Low to High)\n", segment);
#endif
                mmap_growth_dir = UP;
        }
        return (ptr_base);
}

void
stack_inner(void *outer_p)
{
        int inner;
        void *inner_p = &inner;

        if (outer_p > inner_p) {        // down
                stack_growth_dir = DOWN;
        } else {                // up
                stack_growth_dir = UP;
        }
}

void
stack_growth_direction_calc()
{
        int outer;
        void *outer_p = &outer;

        stack_inner(outer_p);
}

int
main(int argc, char *argv[], char *const environ[])
{
        int stack_v = 4;
        struct entry *node_p;
        void *heap_base_p;
        void *frame_bp;

#if defined(DEBUG)
        printf("addr of vmap_list = %p\n", &vmap_list);
#endif

        stack_growth_direction_calc();

        LIST_INIT(&vmap_list);

        heap_base_p = heap_base();
        frame_bp = get_frame_address();

        node_p = alloc_node();
        setup_node(node_p, "HEAP-START", heap_base_p);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "STACK-START", frame_bp);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "MMAP-START", mmap_base());
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "BSS-END", DATA_BSS_END);
        insert_node(node_p);
        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "BSS-VAR", DATA_BSS_VARIABLE);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "BSS-START", DATA_BSS_START);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "DATA-END", DATA_END);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "DATA-GLOBAL-VAR", DATA_GLOBAL);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "DATA-START", DATA_START);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "RO-DATA-CONST", (void *)DATA_CONST);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "TEXT-END", TEXT_END);
        insert_node(node_p);
        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "TEXT-MAIN-ADDR", TEXT_MAIN);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "TEXT-START", TEXT_START);
        insert_node(node_p);

        d_print_list();

        node_p = alloc_node();
        setup_node(node_p, "BREAK-VALUE", current_break_value());
        insert_node(node_p);

        print_list();

        return 0;
}
