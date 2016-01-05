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
    DEBUG("Send id %d to [%s:%d][%d]\n", my_id, inet_ntoa(cnx->infos.sin_addr), ntohs(cnx->infos.sin_port), cnx->fd);
    message_id_t msg;
    msg.type = 'I';
    msg.node_id = my_id;
    send(cnx->fd, &msg, sizeof(msg), 0);
    
    return EXIT_SUCCESS;
}

bool is_node_active(group_t* group, int node_id){
    /* return  group->nodes[node_id - 1]->active; */
    return get_node_by_id(group, node_id)->active;
}

node_t* get_node_by_id(group_t* group, int node_id){
    for(int i = 0; i < group->count; i++){
        if(group->nodes[i]->id == node_id){
            return group->nodes[i];
        }
    }
    return NULL;
}

/** Join the group
 *
 */
void join(){
    int *fds = NULL;
    fds = calloc(send_sockets.count, sizeof(int));

    for(int i = 0; i < send_sockets.count; i++){
        send_sockets.nodes[i]->connexion->fd = socket(AF_INET, SOCK_STREAM,0);
        if(send_sockets.nodes[i]->connexion->fd != -1){
            if(connexion(send_sockets.nodes[i]->connexion) != EXIT_FAILURE ){
                send_sockets.nodes[i]->active = true;
                DEBUG("Connected to [%s:%d][%d]\n", inet_ntoa(send_sockets.nodes[i]->connexion->infos.sin_addr), ntohs(send_sockets.nodes[i]->connexion->infos.sin_port), send_sockets.nodes[i]->connexion->fd);
            }
            else{
                send_sockets.nodes[i]->active = false;
                DEBUG("Failed to connect to [%s:%d][%d]\n", inet_ntoa(send_sockets.nodes[i]->connexion->infos.sin_addr), ntohs(send_sockets.nodes[i]->connexion->infos.sin_port), send_sockets.nodes[i]->connexion->fd);
            }
        }
        else{
            PRINT("Socket failed");
        }
    }

    // Debug
    dump_group_fd(send_sockets);
}

/** Add a node to the receiving list
 * @param fd File descriptor associated to the addr
 * @param addr Address information of the incomming connexion
 * @return 0 if ok 1 if fails
 */
/* int add_node(const int fd, const struct sockaddr_in addr){ */
int add_node(connexion_t* cnx, const int node_id){
    assert(cnx->fd > 2); // To be sure the fd is valid
    
    for(int i = 0; i < receive_sockets.count; i++){
        if(receive_sockets.nodes[i]->id == node_id){
            receive_sockets.nodes[i]->connexion = cnx;
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

void dump_group_fd(group_t group){
    for(int i = 0; i < group.count; i++){
        DEBUG("Send sockets [%s:%d][%d]\n", inet_ntoa(group.nodes[i]->connexion->infos.sin_addr), ntohs(get_node_port(group.nodes[i])), get_node_fd(group.nodes[i]));
    }
}
