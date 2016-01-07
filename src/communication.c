#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>

#include"communication.h"
#include"common.h"
#include"group.h"

/********************************************
 *
 *             Global vars
 *
 *******************************************/

int current_msg_id;

/********************************************
 *
 *             Functions
 *
 *******************************************/

/** Generate unique id to attach to the message
 *
 */
int generate_msg_id(){
     return current_msg_id++;
}

int multicast(const message_t* msg, size_t size){
    int retval;
    for(int i = 0; i < send_sockets.count; i++){
        if(send_sockets.nodes[i]->connexion->fd != -1 && send_sockets.nodes[i]->active){
            DEBUG("Sending [%d][%d] to [%s:%d][%d]\n", msg->node_id, msg->id, inet_ntoa(send_sockets.nodes[i]->connexion->infos.sin_addr), ntohs(send_sockets.nodes[i]->connexion->infos.sin_port), send_sockets.nodes[i]->connexion->fd);
            //TODO ensure that all have been sent
            retval = send(send_sockets.nodes[i]->connexion->fd, (void*) msg, size,0);
        }
    }
}

/** Implementation of best effort broadcast.
 *  @return Number of node to which the message
 *          has been sent.
 */
int beb(const void* content, size_t size){
    int retval;

    message_t* msg = malloc(sizeof(message_t));
    msg->type = 'M';
    msg->content = NULL;
    msg->node_id = my_id;
    /* msg.id = generate_msg_id(); */
    msg->id = 1;

    // Send to my self
    insert_message(msg, &already_received);
    
    /* for(int i = 0; i < send_sockets.count; i++){ */
    /*     if(send_sockets.nodes[i]->connexion->fd != -1 && send_sockets.nodes[i]->active){ */
    /*         DEBUG("Sending to [%s:%d][%d]\n", inet_ntoa(send_sockets.nodes[i]->connexion->infos.sin_addr), ntohs(send_sockets.nodes[i]->connexion->infos.sin_port), send_sockets.nodes[i]->connexion->fd); */
    /*         //TODO ensure that all have been sent */
    /*         retval = send(send_sockets.nodes[i]->connexion->fd, (void*) msg, sizeof(message_t),0); */
    /*         sleep(5); */
    /*     } */
    /* } */
    multicast(msg, sizeof(message_t));

    return 0;
}

/** Check if the message is already in the list
 * @param msg The message to check
 * @param sender The sender of the message
 * @param the list from which to search
 * @return true or false
 */
// TODO modify it to return the msg or null
bool is_already_in(const int msg_id, const int node_id, message_list_t* list){
    message_list_t* current = list;

    if(!list){
        return false;
    }
    
    while(current != NULL){
        /* if (is_the_same_node(msg.sender, current->msg->sender)){ */
        /*     if(current->msg->sequence_nb == msg.sequence_nb){ */
        /*         return true; */
        /*     } */
        /* } */

         if(node_id == current->msg->node_id && msg_id == current->msg->id){
              return true;
         }
        current = current->next;
    }
    
    return false;
}

void acknowledge(message_t msg){
    message_t* ack = malloc(sizeof(message_t));
    ack->type = 'A';
    ack->id = msg.id ;
    ack->node_id = msg.node_id;
    ack->content = NULL;
    
    multicast(ack, sizeof(message_t));
}

/** Tags the nodes from which we have received an ack
 * 
 */
void add_ack(message_list_t** msg, int node_id){
    assert((*msg)->acks[node_id - 1] == false);
    (*msg)->acks[node_id - 1] = true;
    DEBUG_VALID("[%d] Ack [%d][%d]\n", (*msg)->msg->id, (*msg)->msg->node_id, (*msg)->msg->id);
}

/** Add the message msg of the sender sender in the already_received list
 *
 */
void insert_message(message_t* msg, message_list_t** list){
    message_list_t* current = *list;

    message_list_t* new_msg = malloc(sizeof(message_list_t));
    new_msg->msg = msg;

    // Initialize acks
    new_msg->acks = malloc(sizeof(int) * receive_sockets.count + 1);
    for(int i = 0; i  < receive_sockets.count + 1; i++){
        new_msg->acks[i] = false;
    }
    
    // The message is taken it count as in ack of itself.
    // This way the sender doesn't need to ack its message.
    add_ack(&(new_msg), msg->node_id);
    
    new_msg->next = NULL;
    
    if(current == NULL){
        new_msg->prev = NULL;
        /* new_msg->id = msg->id; */
        *list = new_msg;
    }
    else{
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_msg;
        new_msg->prev = current;
        /* new_msg->id = current->id + 1; */
    }
}

/** Remove the message from the already_received list and free the memory
 * @param id Message identifier
 * @return true if the message has been removed otherwise false
 */
bool remove_message(const int id, const int node_id){
    message_list_t* current = already_received;
    bool found = false;

    while(current != NULL || !found){
        if(current->msg->id == id && current->msg->node_id == node_id){
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            found = true;
        }
        else{
            current = current->next;
        }
    }
    
    return found;
}

/** Get the message by using its node_id and msg_id.
 * @return The message or NULL if not in the list.
 */
message_list_t* get_msg(message_list_t** list, const int node_id, const int msg_id){
    message_list_t* current = *list;
    while(current != NULL || (current->msg->id != msg_id && current->msg->node_id != node_id)){
        /* if(current->msg->id == msg_id && current->msg->node_id == node_id){ */
        /*     return current; */
        /* } */
        current = current->next;
    }
    return current;
}
