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

gint compare_msg(gconstpointer a, gconstpointer b){
    message_t* m1 = (message_t*) a;
    message_element_t* m2 = (message_element_t*) b;
    if(m1->node_id == m2->msg->node_id && m1->id == m2->msg->id){
        return 0;
    }
    else{
        return 1;
    }
}

/** Check if the message is already in the list
 * @param msg The message to check
 * @param sender The sender of the message
 * @param the list from which to search
 * @return true or false
 */
// TODO modify it to return the msg or null
bool is_already_in(GList* list, message_t* msg){
    /* if(g_list_find_custom(list,(GCompareFunc)compare_msg, (gpointer)msg)){ */
    if(get_msg_from_list(list,msg)){
        return true;
    }
    else{
        return false;
    }
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
GHashTable* acks_create(){
    // Initialize my entry
    GHashTable* acks = NULL;
    acks = g_hash_table_new(g_int_hash, g_int_equal);
    /* acks = g_hash_table_new(g_direct_hash, g_direct_equal); */

    if(!acks){
        perror("Failed to create the ack hash table");
        exit(EXIT_FAILURE);
    }
    
    bool* my_ack = NULL;
    my_ack = (bool*) malloc(sizeof(bool));
    *my_ack = false;

    if(!g_hash_table_insert(acks, &my_id, my_ack)){
        perror("Failed to create entry in the ack hash table");
        exit(EXIT_FAILURE);
    }

    // Initialize the entries of the others
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init (&iter, group);
    while (g_hash_table_iter_next (&iter, &key, &value))
    {
        int* my_key = (int*) key;
        bool* ack_val = (bool*)malloc(sizeof(bool));
        *ack_val = false;
        if(!g_hash_table_insert(acks, my_key, ack_val)){
            fprintf(stderr,"Failed to create entry %d in the ack hash table\n", *my_key);
            perror("Failed to create other entries in the ack hash table");
            exit(EXIT_FAILURE);
        }
    }
    return acks;
}

/** Tags the nodes from which we have received an ack
 * 
 */
void add_ack(message_element_t* msg, int* node_id){
    assert(msg);
    assert(msg->acks);
    bool* ack_val = (bool*)g_hash_table_lookup(msg->acks, node_id);
    assert(*ack_val == false);
    *ack_val = true;
    DEBUG_VALID("[%d][%d] Ack from [%d]\n", msg->msg->node_id, msg->msg->id, *node_id);
}

void is_replicated_foreach(gpointer key, gpointer value, gpointer userdata){
    node_t* node = (node_t*) value;
}

bool is_replicated(message_element_t* element){
    /* g_hash_table_foreach(group, (GHFunc)is_replicated, NULL);     */
    /* return false; */
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init (&iter, element->acks);
    while (g_hash_table_iter_next (&iter, &key, &value))
    {
        if(*(int*)key != my_id){
            node_t* node = (node_t*)g_hash_table_lookup(group, key);
            bool* ack_val = (bool*) value;
            if(node->in_connected && !ack_val){
                return false;
            }
        }
    }
    return true;
}

/** Add the message msg of the sender sender in the already_received list
 *
 */
void insert_message(message_t* msg, GList** list){
    message_element_t* msg_elmnt = malloc(sizeof(message_element_t));
    msg_elmnt->msg = msg;
    msg_elmnt->acks = NULL;
    msg_elmnt->acks = acks_create();

    *list = g_list_append(*list, msg_elmnt);
    
    // The message is taken it count as in ack of itself.
    // This way the sender doesn't need to ack its message.
    add_ack(msg_elmnt, &msg->node_id);    
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
GList* get_msg_from_list(GList* list, message_t* msg){
    /* return g_list_find_custom(list,(GCompareFunc)compare_msg, (gpointer)msg); */
    
    for(GList* current = list; current != NULL; current = current->next){
        message_element_t* element = (message_element_t*)current->data;
        if(element->msg->node_id == msg->node_id && element->msg->id == msg->id){
            return current;
        }
    }
    return NULL;
}

