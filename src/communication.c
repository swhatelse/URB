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

bool send_all(int socket, void* buf, size_t length){
    char* cursor = (char*) buf;
    while(length > 0){
        int sent = send(socket, cursor, length, 0);
        if(sent  < 1) return false; 
        cursor += sent;
        length -= sent;
    }
    return true;
}

bool recv_all(int socket, void* buf, size_t length){
    char* cursor = (char*) buf;
    while(length > 0){
        int received = recv(socket, cursor, length, 0);
        if(received  < 1) return false; 
        cursor += received;
        length -= received;
    }
    return true;
}

void multicast_foreach(gpointer key, gpointer value, gpointer userdata){
    int retval;
    node_t* node = (node_t*) value;
    message_t* msg = (message_t*) userdata;
    if(node->outbox->fd != -1 && node->out_connected){
        /* DEBUG("Sending '%c' [%d][%d] to [%s:%d][%d]\n", msg->type, msg->node_id, msg->id, inet_ntoa(node->connexion->infos.sin_addr), ntohs(node->connexion->infos.sin_port), node->connexion->fd); */
        retval = send_all(node->outbox->fd, (void*) msg, msg->size);
    }
}

void multicast(message_t* msg, size_t size){
    bool retval;
    msg->size = size;
    g_hash_table_foreach(group, (GHFunc)multicast_foreach, msg);    
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
    
    multicast(msg, sizeof(message_t));
    DEBUG_SEND("[%d][%d] Broadcasted\n", msg->node_id, msg->id);
    return 0;
}

/** Check if the message is already in the list
 * @param msg The message to check
 * @param sender The sender of the message
 * @param the list from which to search
 * @return true or false
 */
// TODO modify it to return the msg or null
bool is_already_in(const int msg_id, const int node_id, dlk_list_t* list){
    dlk_element_t* current = list->tail;

    // The list is empty.
    if(!current){
        return false;
    }
    
    while(current != NULL){
        if(node_id == ((message_element_t*)current->data)->msg->node_id && msg_id == ((message_element_t*)current->data)->msg->id){
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

/** Allocates and initialize acks to false
 *
 */
void initialize_acks(bool** acks){
    size_t size = g_hash_table_size(group) + 1;
    
    *acks = calloc(size, sizeof(bool));
    for(int i = 0; i < size; i++){
        (*acks)[i] = false;
    }
}

/** Tags the nodes from which we have received an ack
 * 
 */
void add_ack(message_element_t** msg, int node_id){
    assert((*msg)->acks[node_id - 1] == false);
    (*msg)->acks[node_id - 1] = true;
    DEBUG_VALID("[%d][%d] Ack from [%d]\n", (*msg)->msg->node_id, (*msg)->msg->id, node_id);
}

void is_replicated_foreach(gpointer key, gpointer value, gpointer userdata){
    node_t* node = (node_t*) value;
}

bool is_replicated(message_element_t* element){
    g_hash_table_foreach(group, (GHFunc)is_replicated, NULL);    
    return false;
}

/** Add the message msg of the sender sender in the already_received list
 *
 */
void insert_message(message_t* msg, dlk_list_t* list){
    dlk_element_t* element = malloc(sizeof(dlk_element_t));
    message_element_t* msg_elmnt = malloc(sizeof(message_element_t));
    msg_elmnt->msg = msg;
    element->data = (void*)msg_elmnt;
    
    initialize_acks(&msg_elmnt->acks);

    dlk_list_append(&already_received, element);
    
    // The message is taken it count as in ack of itself.
    // This way the sender doesn't need to ack its message.
    add_ack(&msg_elmnt, msg->node_id);    
}

/** Remove the message from the already_received list and free the memory
 * @param id Message identifier
 * @return true if the message has been removed otherwise false
 */
bool remove_message(dlk_list_t* list, const int id, const int node_id){
    dlk_element_t* current = list->tail;
    bool found = false;

    while(current != NULL || !found){
        if(((message_element_t*)current->data)->msg->id == id && ((message_element_t*)current->data)->msg->node_id == node_id){
            dlk_list_remove(list, current);
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
dlk_element_t* get_msg_from_list(dlk_list_t* list, const int node_id, const int msg_id){
    dlk_element_t* current = list->tail;
    while(current != NULL){
        if(((message_element_t*)current->data)->msg->id != msg_id && ((message_element_t*)current->data)->msg->node_id != node_id){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

