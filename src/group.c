#include<string.h>

#include"group.h"
#include"listener.h"
#include"node.h"
#include"communication.h"

/********************************************
 *
 *             Functions
 *
 *******************************************/

int connexion(connexion_t* cnx){

    if(connect(cnx->fd, (struct sockaddr*) &(cnx->infos), sizeof(cnx->infos)) < 0){
        perror("Failed to name the socket");
        return EXIT_FAILURE;    
    }

    // TODO send node id
    printf("Sending id %d\n", my_id);
    message_id_t msg;
    msg.type = 'I';
    msg.node_id = my_id;
    send(cnx->fd, &msg, sizeof(msg), 0);
    
    return EXIT_SUCCESS;
}

/** Join the group
 *
 */
void join(){
    int *fds = NULL;
    char buf[64];
    fds = calloc(send_sockets.count, sizeof(int));

    for(int i = 0; i < send_sockets.count; i++){
        send_sockets.nodes[i]->connexion.fd = socket(AF_INET, SOCK_STREAM,0);
        if(send_sockets.nodes[i]->connexion.fd != -1){
            if(connexion(&(send_sockets.nodes[i]->connexion)) != EXIT_FAILURE ){
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
