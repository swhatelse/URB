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
/* int add_node(const int fd, const struct sockaddr_in addr){ */
int add_node(const connexion_t cnx, const int node_id){
    assert(cnx.fd > 2); // To be sure the fd is valid
    char buf[64];
    
    for(int i = 0; i < receive_sockets.count; i++){
        if(receive_sockets.nodes[i]->id == node_id){
            receive_sockets.nodes[i]->connexion = cnx;
            sprintf(buf, "%d %d identified as %d", receive_sockets.nodes[i]->connexion.infos.sin_addr.s_addr, receive_sockets.nodes[i]->connexion.infos.sin_port, receive_sockets.nodes[i]->id);
            PRINT(buf);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

// TODO is it useful?
void remove_node(node_t* node){
    /* free(node); */
}

void* message_handler(){
    join();
    return NULL;
}
