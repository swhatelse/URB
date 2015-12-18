#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<pthread.h>
#include<errno.h>
#include<string.h>
#include<strings.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>

#define MY_SOCK_PATH "/somepath"
#define LISTEN_BACKLOG 50

#define handle_error(msg)                               \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct Group{
  struct sock_addr_in* connected_fella;
};

void* server(){
  int sfd, cfd;
  struct sockaddr_un my_addr, peer_addr;
  socklen_t peer_addr_size;

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1)
    handle_error("socket");

  memset(&my_addr, 0, sizeof(struct sockaddr_un));
  /* Clear structure */

  my_addr.sun_family = AF_UNIX;
  strncpy(my_addr.sun_path, MY_SOCK_PATH,
          sizeof(my_addr.sun_path) - 1);

  if (bind(sfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_un)) == -1)
    handle_error("bind");

  if (listen(sfd, LISTEN_BACKLOG) == -1)
    handle_error("listen");

  /* Now we can accept incoming connections one
     at a time using accept(2) */

  peer_addr_size = sizeof(struct sockaddr_un);
  cfd = accept(sfd, (struct sockaddr *) &peer_addr,
               &peer_addr_size);
  if (cfd == -1)
    handle_error("accept");

  /* Code to deal with incoming connection(s)... */
           
  /* When no longer required, the socket pathname, MY_SOCK_PATH
     should be deleted using unlink(2) or remove(3) */

  return NULL;
}

#define PORT_TIME       13              /* "time" (not available on RedHat) */
#define PORT_FTP        21              /* FTP connection port */
#define SERVER_ADDR     "127.0.0.1"     /* localhost */
#define MAXBUF          1024

void* client(){
  struct sockaddr_in dest;
  char buffer[MAXBUF];
  int socketfd;

  if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
  {
    perror("Socket");
    exit(errno);
  }
  
  /*---Initialize server address/port struct---*/
  bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORT_FTP);
  /* if ( inet_aton(SERVER_ADDR, &dest.sin_addr.s_addr) == 0 ) */
  /* { */
  /*   perror(SERVER_ADDR); */
  /*   exit(errno); */
  /* } */

  /*---Connect to server---*/
  if ( connect(socketfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
  {
    perror("Connect ");
    exit(errno);
  }

  /*---Get "Hello?"---*/
  bzero(buffer, MAXBUF);
  recv(socketfd, buffer, sizeof(buffer), 0);
  printf("%s", buffer);

  /*---Clean up---*/
  close(socketfd);

  return NULL;
}

int main(int argc, char *argv[]){
  return EXIT_SUCCESS;
}
