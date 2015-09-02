
#include <stdio.h>

void child_func(int *parent_var_p)
{
        int child_var;

        if (&child_var > parent_var_p) {
                printf("Low-2-High\n");
        } else {
                printf("High-2-Low\n");
        }
}

void parent_func()
{
        int parent_var;

        child_func(&parent_var);
}

int
main(int argc, char **argv)
{
        parent_func();
}

// gcc -o stack-growth-direction stack-growth-direction.c
