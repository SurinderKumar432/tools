#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_dl.h>

#include <netinet/if_ether.h>

#include <stdio.h>
#include <stdlib.h>

#include <macros.h>

#define SA_LEN(_sa_p) ((_sa_p)->sa_len) 

int
main()
{
  int           sock_fd;
  int           num_if;
  int           ifconf_size;
  struct ifconf ifc;
  struct ifreq  *ifr;
  struct ifreq  *next_ifr;
  int		ifspace;
  int		i;
  int		num_if_2;
  int		socket_length;
  int		ifname_length;
  int		ifr_offset;
  int		ifreq_size;
  struct sockaddr *sa_p;
  char ip_buf[46];



  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd == -1) {
    perror("socket()");
    exit(1);
  }

  if (ioctl(sock_fd, SIOCGSIZIFCONF, &ifconf_size) == -1) {
    perror("ioctl()");
    close(sock_fd);
    exit(1);
  }

  printf("SIOCGSIZIFCONF, ifconf_size = %d\n", ifconf_size);

  /* interface count */
  ifc.ifc_len = ifconf_size;
  ifc.ifc_buf = (char *)malloc(ifc.ifc_len); 
  if (!ifc.ifc_buf) {
   perror("malloc()"); 
   close(sock_fd);
   exit(1);
  }

  if (ioctl(sock_fd, OSIOCGIFCONF, &ifc) == -1) {
    free(ifc.ifc_buf);
    perror("ioctl()");
    close(sock_fd);
    exit(1);
  }

  /* Initialize numifs */
  num_if = (((size_t)ifc.ifc_len) / sizeof(struct ifreq));
  printf("OSIOCGIFCONF, num_if = %d\n", num_if);
  ifspace = num_if;
  ifreq_size = sizeof(struct ifreq);

  printf("Data using OSIOCGIFCONF (ipv4-correct, ipv6-incorrect, link-incorrect) - FIXED-SIZE IFREQ\n");
  
  for (ifr = (struct ifreq *)ifc.ifc_buf;
      (char *)ifr < ( (char *)ifc.ifc_buf + ifc.ifc_len);
      i++, ifr = (struct ifreq *) (((unsigned long)ifr)+ifr_offset)) {

    sa_p = &ifr->ifr_addr ;
    ifname_length = sizeof(ifr->ifr_name) ;
    if (SA_LEN(sa_p) > sizeof(*sa_p)) {
      socket_length = SA_LEN(sa_p); 
    } else {
      socket_length = sizeof(*sa_p); 
    }
    ifr_offset = ifname_length + socket_length; 

    printf("i=%d, name=%s, ifname_length=%d, socket_length=%d, ifreq_size=%d, ifr_offset=%d\n",
            i,ifr->ifr_name, ifname_length, socket_length, ifreq_size, ifr_offset);

    printf("\tfamily = %d", sa_p->sa_family);
    if (sa_p->sa_family == AF_INET) {
    	printf(", addr = %s", inet_ntoa(((struct sockaddr_in *)sa_p)->sin_addr));
    } else if (sa_p->sa_family == AF_INET6) {
    	inet_ntop(sa_p->sa_family, &((struct sockaddr_in6 *)sa_p)->sin6_addr, ip_buf, 46);
        printf(", addr = %s", ip_buf);
    } else if (sa_p->sa_family == AF_LINK) {
        printf(", addr length = %d", ((struct sockaddr_dl *)sa_p)->sdl_alen);
        printf(", addr type = %d", ((struct sockaddr_dl *)sa_p)->sdl_type);
        if (((struct sockaddr_dl *)sa_p)->sdl_type == IFT_ETHER) {
          struct ether_addr *e = (struct ether_addr *) LLADDR((struct sockaddr_dl *)sa_p) ;

	  //printf("addr = %.16s\n",ether_ntoa((struct ehter *)LLADDR((struct sockaddr_dl *)sa_p)));

          memset(ip_buf, sizeof(ip_buf), 0);
 
  	  sprintf(ip_buf, "%02x:%02x:%02x:%02x:%02x:%02x",
  	    e->ether_addr_octet[0], e->ether_addr_octet[1],
  	    e->ether_addr_octet[2], e->ether_addr_octet[3],
  	    e->ether_addr_octet[4], e->ether_addr_octet[5]);

          printf(", addr = %s", ip_buf);
        }
    }


    /* ip address 
     */
    if ((ioctl(sock_fd, SIOCGIFADDR, (char *) ifr) == -1))
    {                                                                    /*NF*/
      free(ifc.ifc_buf);
      perror("ioctl(SIOCGIFADDR)");
      close(sock_fd);
      exit(1);
    }
    /* net mask
     */
    if ((ioctl(sock_fd, SIOCGIFNETMASK, (char *) ifr) == -1))
    {                                                                    /*NF*/
      free(ifc.ifc_buf);
      perror("ioctl(SIOCGIFNETMASK)");
      close(sock_fd);
      exit(1);
    }

    sa_p = &ifr->ifr_addr ;
    printf(", mask= %s", inet_ntoa(((struct sockaddr_in *)sa_p)->sin_addr));

    /* current interface state flags
     */
    if ((ioctl(sock_fd, SIOCGIFFLAGS, (char *) ifr) == -1))
    {                                                                    /*NF*/
      free(ifc.ifc_buf);
      perror("ioctl(SIOCGIFFLAGS)");
      close(sock_fd);
      exit(1);
    }

    printf("\n");

  }


  //reset ifc_len
  ifc.ifc_len = ifconf_size;
  if (ioctl(sock_fd, SIOCGIFCONF, &ifc) == -1) {
    free(ifc.ifc_buf);
    perror("ioctl()");
    close(sock_fd);
    exit(1);
  }


  printf("\nData using SIOCGIFCONF (ipv4-correct, ipv6-correct, link-correct) - VARIABLE_SIZE IFREQ\n");
  for (i = 0, ifr = (struct ifreq *)ifc.ifc_buf;
       i < ifspace && (char *)ifr < ( (char *)ifc.ifc_buf + ifc.ifc_len);
       i++, ifr = next_ifr ) 
  {

    printf("i=%d, name=%s\n",i,ifr->ifr_name);

    next_ifr = (char *) ifr + max(sizeof(*ifr), sizeof(ifr->ifr_name)+ifr->ifr_addr.sa_len) ;

    sa_p = &ifr->ifr_addr ;
    printf("\tfamily = %d", sa_p->sa_family);
    if (sa_p->sa_family == AF_INET) {
    	printf(", addr = %s", inet_ntoa(((struct sockaddr_in *)sa_p)->sin_addr));
    } else if (sa_p->sa_family == AF_INET6) {
    	inet_ntop(sa_p->sa_family, &((struct sockaddr_in6 *)sa_p)->sin6_addr, ip_buf, 46);
        printf(", addr = %s", ip_buf);
    } else if (sa_p->sa_family == AF_LINK) {
        printf(", addr length = %d", ((struct sockaddr_dl *)sa_p)->sdl_alen);
        printf(", addr type = %d", ((struct sockaddr_dl *)sa_p)->sdl_type);
        if (((struct sockaddr_dl *)sa_p)->sdl_type == IFT_ETHER) {
          struct ether_addr *e = (struct ether_addr *) LLADDR((struct sockaddr_dl *)sa_p) ;

	  //printf("addr = %.16s\n",ether_ntoa((struct ehter *)LLADDR((struct sockaddr_dl *)sa_p)));

 
          memset(ip_buf, sizeof(ip_buf), 0);
 
  	  sprintf(ip_buf, "%02x:%02x:%02x:%02x:%02x:%02x",
  	    e->ether_addr_octet[0], e->ether_addr_octet[1],
  	    e->ether_addr_octet[2], e->ether_addr_octet[3],
  	    e->ether_addr_octet[4], e->ether_addr_octet[5]);

          printf(", addr = %s",ip_buf);
        }
    }




    /* ip address 
     */
    if ((ioctl(sock_fd, SIOCGIFADDR, (char *) ifr) == -1))
    {                                                                    /*NF*/
      free(ifc.ifc_buf);
      perror("ioctl(SIOCGIFADDR)");
      close(sock_fd);
      exit(1);
    }
    /* net mask
     */
    if ((ioctl(sock_fd, SIOCGIFNETMASK, (char *) ifr) == -1))
    {                                                                    /*NF*/
      free(ifc.ifc_buf);
      perror("ioctl(SIOCGIFNETMASK)");
      close(sock_fd);
      exit(1);
    }

    sa_p = &ifr->ifr_addr ;
    printf(", mask= %s", inet_ntoa(((struct sockaddr_in *)sa_p)->sin_addr));

    /* current interface state flags
     */
    if ((ioctl(sock_fd, SIOCGIFFLAGS, (char *) ifr) == -1))
    {                                                                    /*NF*/
      free(ifc.ifc_buf);
      perror("ioctl(SIOCGIFFLAGS)");
      close(sock_fd);
      exit(1);
    }
    printf("\n"); 

  }

  free(ifc.ifc_buf);
  close(sock_fd);
  exit(0);
}
