#include <sys/types.h>                                                   
#include <sys/socket.h>                                                  
#include <netinet/in.h>                                                  
#include <arpa/inet.h>                                                   

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

int 
bind_wrap_v6(int reuse_addr, int reuse_port, char *addr, short port)
{                                                                   
        int sock_fd;                                                
        socklen_t namelen;                                          
        char host_addr[1024];                                       

        struct sockaddr_in6 sin;

        sock_fd = socket(AF_INET6, SOCK_STREAM, 0);
        if (sock_fd == -1) {                       
                perror("socket()");                
                exit(1);                           
        }                                          

        if (reuse_addr)
        {              
#if defined(SO_REUSEADDR)
                if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,
                               &reuse_addr, sizeof(reuse_addr)) == -1) {
                        perror("setsockopt()");
                        exit(1);                                                                           
                }
                printf("SO_REUSEADDR\n");                                                                  
#endif /* SO_REUSEADDR */                                                                                  
        } /* reuse */
        
        if (reuse_port)
        {              
#if defined(SO_REUSEPORT)
                if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT,
                               &reuse_port, sizeof(reuse_port)) == -1) {
                        perror("setsockopt()");
                        exit(1);
                }
                printf("SO_REUSEPORT\n");
#endif /* SO_REUSEPORT */
        }

        memset(&sin, 0, sizeof(sin));
        sin.sin6_family = AF_INET6; 
        sin.sin6_port = htons(port); 

        inet_pton(AF_INET6, addr, &sin.sin6_addr);

        if (bind(sock_fd, (struct sockaddr*)&sin, sizeof(sin)) == -1)
        {                                                            
                perror("bind()");                                    
                exit(1);                                             
        }                                                            

        namelen = sizeof(sin);
        if (getsockname(sock_fd, (struct sockaddr*)&sin, &namelen) == -1)
        {                                                                
                perror("getsockname()");                                 
                exit(1);                                                 
        }                                                                

        inet_ntop(AF_INET6, &sin.sin6_addr, host_addr, sizeof(host_addr));

        printf("AF = %d, addr = %s, port = %d\n",
                sin.sin6_family, host_addr, ntohs(sin.sin6_port));

        if (listen(sock_fd, 128) == -1)
        {                              
                perror("listen()");    
                exit(1);               
        }

        return sock_fd;
}


int 
bind_wrap_v4(int reuse_addr, int reuse_port, int addr, short port)
{                                                                 
        int sock_fd;                                              
        socklen_t namelen;                                        

        struct sockaddr_in sin;

        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd == -1) {                      
                perror("socket()");               
                exit(1);                          
        }                                         

        if (reuse_addr)
        {              
#if defined(SO_REUSEADDR)
                if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,
                               &reuse_addr, sizeof(reuse_addr)) == -1) {
                        perror("setsockopt()");
                        exit(1);
                }
                printf("SO_REUSEADDR\n");
#endif /* SO_REUSEADDR */
        } /* reuse */


        if (reuse_port)
        {              
#if defined(SO_REUSEPORT)
                if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT,
                               &reuse_port, sizeof(reuse_port)) == -1) {
                        perror("setsockopt()");
                        exit(1);
                }
                printf("SO_REUSEPORT\n");
#endif /* SO_REUSEPORT */
        }

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;    
        sin.sin_addr.s_addr = addr; 
        sin.sin_port = htons(port);  
        if (bind(sock_fd, (struct sockaddr*)&sin, sizeof(sin)) == -1)
        {                                                            
                perror("bind()");                                    
                exit(1);                                             
        }                                      
        
        namelen = sizeof(sin);
        if (getsockname(sock_fd, (struct sockaddr*)&sin, &namelen) == -1)
        {                                                                
                perror("getsockname()");                                 
                exit(1);                                                 
        }                                                                
        printf("AF = %d, addr = %s, port = %d\n",
                sin.sin_family, inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));


        if (listen(sock_fd, 128) == -1)
        {                              
                perror("listen()");    
                exit(1);               
        }                              
        return sock_fd;
}                                      



int
main(int argc, char *argv[])
{
        int sock_fd1;
        int sock_fd2;
        short port;
        char  *addr;
        int   ip_addr;
        int sleep_secs;
        int reuse_port;
        int reuse_addr;
        char *ip_type;

        if (argc < 7) {
          printf("usage: %s <i4|i6> <addr> <port> <reuse_addr> <reuse_port>"
                 " <sleep_secs>\n", argv[0]);
          exit(1);
        }

        ip_type = argv[1];
        addr = argv[2];
        port       = atoi(argv[3]);
        reuse_addr = atoi(argv[4]);
        reuse_port = atoi(argv[5]);
        sleep_secs = atoi(argv[6]);

        if (strcmp(addr, "lo") == 0)
        {
          if (strcmp(ip_type, "i6") == 0)
          {
            addr = "::1";
          }
          else
          {
            ip_addr = htonl(INADDR_LOOPBACK); 
          }
        }
        else if (strcmp(addr, "any") == 0)
        {
          if (strcmp(ip_type, "i6") == 0)
            addr = "::";
          else
            ip_addr = htonl(INADDR_ANY);
        }
        else
        {
            ip_addr = inet_addr(argv[2]);
        }
        if (strcmp(ip_type, "i6") == 0)
        {
          sock_fd1 = bind_wrap_v6(reuse_addr, reuse_port, addr, port);
        }
        else
        {
          sock_fd1 = bind_wrap_v4(reuse_addr, reuse_port, ip_addr, port);
        }

        sleep(sleep_secs);

        close(sock_fd1);

        exit(0);
}

/* COMPILE : gcc -o x sock_reuse_addr_port.c -lsocket -lnsl */
