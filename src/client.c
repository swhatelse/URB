#include<string.h>
#include"client.h"

/********************************************
 *
 *             Functions
 *
 *******************************************/

int connection(const char* addr, const int port){
    int sfd;
    struct sockaddr_in srv_addr;

    sfd = socket(AF_INET, SOCK_STREAM,0);
    if(sfd < 0){
        perror("Failed to attribute the socket");
        return EXIT_FAILURE;
    }
  
    // Retrieve server info
    /* struct hostent *server = malloc(sizeof(struct hostent)); */
    /* server = gethostbyname("127.0.0.1"); */
    /* printf("%lx\n", (long unsigned int) server); */

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = inet_addr(addr);

    if(connect(sfd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0){
        perror("Failed to name the socket");
        return -1;    
    }
  
    return sfd;
}

void join(){
    int *fds = NULL;
    char buf[64];
    fds = calloc(send_sockets.count, sizeof(int));

    for(int i = 0; i < send_sockets.count; i++){
        fds[i] = socket(AF_INET, SOCK_STREAM,0);
        if(fds[i] != -1){
            if(connect(fds[i], (struct sockaddr*) &(send_sockets.nodes[i].infos), sizeof(struct sockaddr_in)) != -1){
                send_sockets.nodes[i].alive = true;
                send_sockets.nodes[i].fd = fds[i];
                sprintf(buf, "Connected to server %d %d", send_sockets.nodes[i].infos.sin_addr.s_addr, send_sockets.nodes[i].infos.sin_port);
                PRINT(buf);
            }
            else{
                sprintf(buf, "Failed connect to server %d %d", send_sockets.nodes[i].infos.sin_addr.s_addr, send_sockets.nodes[i].infos.sin_port);
                PRINT(buf);
                send_sockets.nodes[i].alive = false;
            }
        }
        else{
            PRINT("Socket failed");
        }
    }
}

void* message_handler(){
    join();
    return NULL;
}
