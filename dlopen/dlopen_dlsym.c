#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
    void *handle;
    char *lib_name;
    char *lib_sym;
   
    if (argc < 3)
    {
      printf("usage: %s <lib> <symbol | none>\n", argv[0]);
      exit(1);
    }

    lib_name = argv[1];
    lib_sym = argv[2];

    handle = dlopen (lib_name, RTLD_NOW | RTLD_GLOBAL);
    if (!handle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }


    if (strcmp(lib_sym, "none") != 0)
    {
      if ( dlsym(handle, lib_sym) )
      {
        fprintf (stdout, "symbol %s found\n", lib_sym);
      }
      else
      {
        fprintf (stderr, "symbol %s NOT found errod %s\n", lib_sym, dlerror());
        exit(1);
      }
   }

    dlclose(handle);
    return 0;
}

// gcc -o dlopen_dlsym -m64 dlopen_dlsym.c -ldl
