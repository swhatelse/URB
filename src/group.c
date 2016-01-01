#include<string.h>

#include"group.h"
#include"listener.h"
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
  
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = inet_addr(addr);

    if(connect(sfd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0){
        perror("Failed to name the socket");
        return -1;    
    }
  
    return sfd;
}

/** Join the group
 *
 */
void join(){
    int *fds = NULL;
    char buf[64];
    fds = calloc(send_sockets.count, sizeof(int));

    for(int i = 0; i < send_sockets.count; i++){
        fds[i] = socket(AF_INET, SOCK_STREAM,0);
        if(fds[i] != -1){
            if(connect(fds[i], (struct sockaddr*) &(send_sockets.nodes[i]->connexion.infos), sizeof(struct sockaddr_in)) != -1){
                send_sockets.nodes[i]->connexion.fd = fds[i];
                sprintf(buf, "Connected to server %d %d", send_sockets.nodes[i]->connexion.infos.sin_addr.s_addr, send_sockets.nodes[i]->connexion.infos.sin_port);
                PRINT(buf);
            }
            else{
                sprintf(buf, "Failed connect to server %d %d", send_sockets.nodes[i]->connexion.infos.sin_addr.s_addr, send_sockets.nodes[i]->connexion.infos.sin_port);
                PRINT(buf);
            }
        }
        else{
            PRINT("Socket failed");
        }
    }
}

/** Add a node to the receiving list
 * @param fd File descriptor associated to the addr
 * @param addr Address information of the incomming connexion
 * @return 0 if ok 1 if fails
 */
int add_node(const int fd, const struct sockaddr_in addr){
    assert(fd > 2); // To be sure the fd is valid
    
    for(int i = 0; i < receive_sockets.count; i++){
        /* if(receive_sockets.nodes[i].connexion.infos.sin_addr.s_addr == 0){ */
        if(receive_sockets.nodes[i] == NULL){
            receive_sockets.nodes[i] = malloc(sizeof(node_t));
            receive_sockets.nodes[i]->connexion.infos = addr;
            receive_sockets.nodes[i]->connexion.fd = fd;
            FD_SET(fd, &reception_fd_set);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

void remove_node(node_t* node){
    free(node);
}

void* message_handler(){
    join();
    return NULL;
}
