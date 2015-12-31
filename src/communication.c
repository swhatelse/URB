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


/********************************************
 *
 *             Functions
 *
 *******************************************/

/** Implementation of best effort broadcast.
 *  @return Number of node to which the message
 *          has been sent.
 */
int beb(const message_t msg){
    char buf[64];
    for(int i = 0; i < send_sockets.count; i++){
        if(send_sockets.nodes[i]->fd != -1 ){
            sprintf(buf, "Sending a message to %d %d", send_sockets.nodes[i]->infos.sin_addr.s_addr, send_sockets.nodes[i]->infos.sin_port); 
            PRINT(buf);
            //TODO ensure that all have been sent
            send(send_sockets.nodes[i]->fd, (void*) &msg, sizeof(message_t),0);
            sleep(3);
        }
    }
    
    return 0;
}

bool is_already_in(message_t msg, node_t sender){
    bool found = false;
    message_list_t* current = already_received;
    while(!found || current != NULL){
        if (is_the_same_node(current->sender, sender)){
            if(current->msg.id == msg.id){
                found = true;
            }
        }
        current = current->next;
    }
    
    return true;
}
