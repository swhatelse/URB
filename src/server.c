#include"server.h"

#define BACKLOG 50
#define PRINT(x)                                \
  printf("Server : %s\n", x)

void connexion_accept(){
  int sfd, cfd;
  struct sockaddr_in my_addr, peer_addr;
  socklen_t peer_addr_size;
  
  sfd = socket(AF_INET, SOCK_STREAM,0);
  if(sfd < 0){
    perror("Failed to attribute the socket\n");
    exit(EXIT_FAILURE);
  }

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(9000);
  my_addr.sin_addr.s_addr = INADDR_ANY;


  if(bind(sfd, (struct sockaddr*) &my_addr, sizeof(my_addr)) < 0 ){
    perror("Failed to name the socket\n");
    exit(EXIT_FAILURE);    
  }

  if(listen(sfd, BACKLOG) < 0){
    perror("Failed to listen\n");
    exit(EXIT_FAILURE);    
  }

  cfd = accept(sfd, (struct sockaddr*) &peer_addr, &peer_addr_size);
  if(cfd < 0){
    perror("Failed to accept\n");
    exit(EXIT_FAILURE);    
  }
  
  PRINT("Client connected");

    close(sfd);
  
  return EXIT_SUCCESS;
}
