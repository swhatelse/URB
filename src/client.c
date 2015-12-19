#include"client.h"

#define PRINT(x)                                \
  printf("Client : %s\n", x)

int connection(const char* addr){
  int sfd;
  struct sockaddr_in srv_addr;
  char buf[8];

  sfd = socket(AF_INET, SOCK_STREAM,0);
  if(sfd < 0){
    perror("Failed to attribute the socket");
    return EXIT_FAILURE;
  }

  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(9000);
  srv_addr.sin_addr.s_addr = inet_addr(addr);

  if(connect(sfd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0){
    perror("Failed to name the socket");
    return EXIT_FAILURE;    
  }

  recv(sfd, buf, sizeof(buf), 0);

  PRINT(buf);
  
  close(sfd);
  return EXIT_SUCCESS;
}
