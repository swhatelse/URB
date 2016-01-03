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

int generate_msg_id(){
     return current_msg_id++;
}
/** Implementation of best effort broadcast.
 *  @return Number of node to which the message
 *          has been sent.
 */
int beb(const void* content, size_t size){
    int retval;

    message_t msg;
    msg.type = 'M';
    msg.content = NULL;
    msg.node_id = my_id;
    msg.id = generate_msg_id();
    
    for(int i = 0; i < send_sockets.count; i++){
        if(send_sockets.nodes[i]->connexion.fd != -1 && send_sockets.nodes[i]->active){
            DEBUG("Sending to [%s:%d][%d]\n", inet_ntoa(send_sockets.nodes[i]->connexion.infos.sin_addr), ntohs(send_sockets.nodes[i]->connexion.infos.sin_port), send_sockets.nodes[i]->connexion.fd);
            //TODO ensure that all have been sent
            /* retval = send(send_sockets.nodes[i]->connexion.fd, (void*) &msg, sizeof(message_t),0); */
            /* retval = send(send_sockets.nodes[i]->connexion.fd, (void*) &message, sizeof(message_t),0); */
            retval = send(send_sockets.nodes[i]->connexion.fd, (void*) &msg, sizeof(message_t),0);
            sleep(5);
        }
    }

    return 0;
}

/** Check if the message is already in the list
 * @param msg The message to check
 * @param sender The sender of the message
 * @param the list from which to search
 * @return true or false
 */
bool is_already_in(message_t msg, message_list_t* list){
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

         if(msg.node_id == current->msg->node_id && msg.id == current->msg->id){
              return true;
         }
        current = current->next;
    }
    
    return false;
}

/** Add the message msg of the sender sender in the already_received list
 *
 */
void insert_message(message_t* msg, message_list_t* list){
    message_list_t* current = list;

    message_list_t* new_msg = malloc(sizeof(message_list_t));
    new_msg->msg = msg;
    new_msg->next = NULL;
    
    if(current == NULL){
        new_msg->prev = NULL;
        new_msg->id = 0;
        list = new_msg;
    }
    else{
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_msg;
        new_msg->prev = current;
        new_msg->id = current->id + 1;
    }
}

/** Remove the message from the already_received list and free the memory
 * @param id Message identifier
 * @return true if the message has been removed otherwise false
 */
bool remove_message(const int id){
    message_list_t* current = already_received;
    bool found = false;

    while(current != NULL || !found){
        if(current->id != id){
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
