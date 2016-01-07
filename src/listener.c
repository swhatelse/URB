#include<assert.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/time.h>

#include"listener.h"
#include"communication.h"

/********************************************
 *
 *             Macros
 *
 *******************************************/

#define BACKLOG 50

/********************************************
 *
 *             Global vars
 *
 *******************************************/
dlk_list_t connexions_pending;
/********************************************
 *
 *             Functions
 *
 *******************************************/

/** Put the connexion in the waiting queue
 * Connexions are put in standby in order to wait for the remote to
 * Identify it self by sending its id.
 * @param fd File descriptor of the incomming connexion.
 * @param addr Address informations of the incomming connexion.
 */
void connexion_pending_add(int fd, struct sockaddr_in addr){
    dlk_element_t* pending = malloc(sizeof(dlk_element_t));
    connexion_t* cnx = malloc(sizeof(connexion_t));
 
    cnx->fd = fd;
    cnx->infos = addr;

    pending->data = (void*)cnx;

    dlk_list_append(&connexions_pending, pending);
}

/** Extract a connexion from the pending list.
 *
 */
connexion_t* connexion_pending_pop(dlk_element_t* cnx_pnd){
    connexion_t* cnx;
    dlk_list_remove(&connexions_pending, cnx_pnd);

    cnx = (connexion_t*)cnx_pnd->data;
    return cnx;
}

/** Pop and destroy the connexion
 *
 */
void connexion_pending_remove(dlk_element_t* cnx_pnd){
    connexion_t* cnx =  connexion_pending_pop(cnx_pnd);
    free(cnx);
    cnx = NULL;
}

dlk_element_t* connexions_pending_get(int fd){
    dlk_element_t* current = connexions_pending.tail;
    while(current != NULL){
        if(((connexion_t*)(current->data))->fd == fd){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void handle_id(message_id_t* msg){
    PRINT("Node identified");
    printf("%d\n", msg->node_id);    
}

/** Manage the received acks
 * @param ack The received acknowledgment.
 * @param sender The node which sent ack.
 */
void handle_ack(message_t* ack, node_t* sender){
    // First check if we already received the corresponding message.
    // If not create an entry in the already_received list and add the ack.
    // The message will be filled when received.
    /* message_list_t* msg = get_msg(&already_received, ack->node_id, ack->id); */
    /* if(!msg){ */
    /*     msg = malloc(sizeof(message_list_t)); */
    /* } */
    DEBUG_VALID("[%d] Ack [%d][%d]\n", sender->id, ack->node_id, ack->id);
}

void handle_normal(message_t* msg, node_t* sender){
     DEBUG("[%d] Message received from [%s:%d][%d]\n", msg->node_id, inet_ntoa(sender->connexion->infos.sin_addr), ntohs(sender->connexion->infos.sin_port), sender->connexion->fd);
     // Message have not received yet
     if(!is_already_in(msg->id, msg->node_id, &already_received)){
         insert_message(msg, &already_received);
         // Send ack on new message
         acknowledge(*msg);
     }
     // Message already received
     else{
          // Message already received, we can drop it
         free(msg);
         msg = NULL;
     }
}

void handle_message(message_t* msg, node_t* sender){
    switch(msg->type){
    case 'M':
         handle_normal(msg, sender);
        break;
    case 'A':
        handle_ack((message_t*)msg, sender);
        break;
    default:
        /* handle_ack((message_ack_t*)msg, sender); */
        PRINT("Unknown type");
        break;
    }
}

int connexion_accept(){
    int cfd;
    struct sockaddr_in peer_addr;
    /* struct sockaddr_in* peer_addr = malloc(sizeof(peer_addr)); */
    socklen_t peer_addr_size = sizeof(struct sockaddr_in);
  
    cfd = accept(listening_fd, (struct sockaddr*) &peer_addr, &peer_addr_size);
    /* cfd = accept(listening_fd, (struct sockaddr*) peer_addr, sizeof(struct sockaddr_in)); */
    if(cfd < 0){
        perror("Failed to accept\n");
    }

    FD_SET(cfd, &reception_fd_set);
    /* fcntl(cfd, F_SETFL, O_NONBLOCK); */
    /* connexion_pending_add(cfd, &peer_addr); */
    connexion_pending_add(cfd, peer_addr);    
    
    DEBUG("[?] Client request [%s:%d][%d]\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port), cfd);
    
    return cfd;
}


void handle_connexion_requests(fd_set active_set){
     int size;
     message_id_t msg;
    
     // First connexion step
     if(FD_ISSET(listening_fd, &active_set)){
          connexion_accept();
     }

     dlk_element_t* current = connexions_pending.tail;
    
     // Accept pending connexion
     while(current != NULL){
         if(((connexion_t*)(current->data)) && FD_ISSET(((connexion_t*)(current->data))->fd, &active_set)){
               size = recv(((connexion_t*)(current->data))->fd, (void*)&msg, sizeof(message_id_t), 0);
               if(size > 0){
                    DEBUG("[%d] Client validation validated [%s:%d][%d]\n", msg.node_id, inet_ntoa(((connexion_t*)(current->data))->infos.sin_addr), ntohs(((connexion_t*)(current->data))->infos.sin_port), ((connexion_t*)(current->data))->fd);
                    /* add_node(((connexion_t*)(current->data)), msg.node_id); */
                    add_node(connexion_pending_pop(current), msg.node_id);
                    // If the sending connexion is not establish, establishes it
                    if(!is_node_active(&send_sockets, msg.node_id)){
                        DEBUG("================ Rejoin =====================\n");
                        node_t* node = get_node_by_id(&send_sockets, msg.node_id); 
                        connexion(node->connexion);
                        node->active = true;
                    }
               }
               else{
                    // Disconnection
                   if(((connexion_t*)(current->data))){
                       DEBUG("[?] Client validation aborted [%s:%d][%d]\n", inet_ntoa(((connexion_t*)(current->data))->infos.sin_addr), ntohs(((connexion_t*)(current->data))->infos.sin_port), ((connexion_t*)(current->data))->fd);
                       FD_CLR(((connexion_t*)(current->data))->fd, &reception_fd_set);
                       close(((connexion_t*)(current->data))->fd);
                       connexion_pending_remove(current);
                   }
               }
          }
          current = current->next;
     }
}

void handle_disconnexion(int index){
    if(receive_sockets.nodes[index]->connexion){
        DEBUG("[%d] Deconnexion\n", receive_sockets.nodes[index]->id);
        FD_CLR(receive_sockets.nodes[index]->connexion->fd, &reception_fd_set);
        close(receive_sockets.nodes[index]->connexion->fd);
        free(receive_sockets.nodes[index]->connexion);
        receive_sockets.nodes[index]->connexion = NULL;
        // Not sure it is need
        /* remove_node(receive_sockets.nodes[index]); */
    }
}

void handle_event(fd_set active_set){
    handle_connexion_requests(active_set);

    for(int i = 0; i < receive_sockets.count; i++){
        if(receive_sockets.nodes[i] != NULL && receive_sockets.nodes[i]->connexion != NULL){
            if(FD_ISSET(receive_sockets.nodes[i]->connexion->fd, &active_set)){
                message_t *msg = malloc(sizeof(message_t));
                int size = 0;
                // The size of message_t is used here because it is the longuest we can receive.
                size = recv(receive_sockets.nodes[i]->connexion->fd, (void*)msg, sizeof(message_t), 0);
                if(size > 0){
                    handle_message(msg, receive_sockets.nodes[i]);
                }
                else{
                    free(msg);
                    handle_disconnexion(i);
                }
            }
        }
    }
}

/** Prepare the socket for waiting connexion
 *
 */
void listener_init(){
    // TODO here just for the moment
    dlk_list_init(&already_received);
    dlk_list_init(&connexions_pending);
        
    PRINT("Initialization of the listener");
    
    listening_fd = socket(AF_INET, SOCK_STREAM,0);
    if(listening_fd < 0){
        perror("Failed to attribute the socket\n");
        exit(EXIT_FAILURE);
    }
  
    my_addr.sin_family = AF_INET;
    /* my_addr.sin_port = htons(my_port); */
    /* set_my_port(my_port);     */
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listening_fd, (struct sockaddr*) &my_addr, sizeof(my_addr)) < 0 ){
        perror("Failed to name the socket\n");
        exit(EXIT_FAILURE);    
    }

    if(listen(listening_fd, BACKLOG) < 0){
        perror("Failed to listen\n");
        exit(EXIT_FAILURE);    
    }
}

void* listener_run(){
    struct timeval timeout;
    int event = 0;
    
    FD_ZERO(&reception_fd_set);
    FD_SET(listening_fd, &reception_fd_set);

    DEBUG("Start to listen\n");
    DEBUG("=============================================\n");
    while(1){
        // Timeout needs to be reset each time
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        fd_set active_set;
        FD_ZERO(&active_set);
        FD_SET(listening_fd, &active_set);
        
        active_set = reception_fd_set;

        event = select(FD_SETSIZE, &active_set, NULL, NULL, &timeout);
        if(event == -1){
            perror("Select failed");
        }
        else if(event){
            handle_event(active_set);
        }
        else{

        }
    }
    return NULL;
}
