
#include <stdio.h>

int
main()
{
    int x = 1;

    (*(char*)&x) ? printf("Little-Endian\n") : printf("Big-Endian\n") ;
}

// gcc -o endian endian.c
