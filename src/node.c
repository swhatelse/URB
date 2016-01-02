#include"node.h"

int get_node_port(node_t* node){
    return node->connexion.infos.sin_port;
}

int get_node_addr(node_t* node){
   return node->connexion.infos.sin_addr.s_addr;
}

int get_node_fd(node_t* node){
   return node->connexion.fd;
}


bool is_the_same_node(const node_t node1, const node_t node2){
    if(node1.id == node2.id){
        return true;
    }
    else{
        return false;
    }
}

