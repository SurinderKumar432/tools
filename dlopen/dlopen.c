
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    void *handle;
   
    if (argc < 2)
    {
      printf("usage: %s <lib>\n", argv[0]);
      exit(1);
    }

    handle = dlopen (argv[1], RTLD_NOW | RTLD_GLOBAL);
    if (!handle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }

    dlclose(handle);
    return 0;
}
// gcc -o dlopen dlopen.c -ldl
