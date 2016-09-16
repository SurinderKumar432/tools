#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<errno.h>

int main(int argc, char *argv[])
{
    /* MAXHOSTNAMELEN : 64 */
    /* HOST_NAME_MAX : 64 */

    char host[MAXHOSTNAMELEN+1];
    
    // incorrect
    // char host[MAXHOSTNAMELEN];

    int hostlen;

     if (argc > 1)
       hostlen = atoi(argv[1]);
     else
       hostlen = sizeof(host);

    printf("using hostlen = %d\n", hostlen);

    if (gethostname((char *)host, hostlen) == -1)
    {
          perror("gethostname");

          if (errno == ENAMETOOLONG)
          {
            printf("host = %s\n", host);
            printf("hostlen = %d\n", strlen((const char *)host));
          }

          exit(1);
    }

    printf("host = %s\n", host);
    printf("hostlen = %d\n", strlen((const char *)host));

    exit(0);
}
