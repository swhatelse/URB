#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<assert.h>

#include"node.h"

int get_node_port(node_t* node){
    return node->connexion->infos.sin_port;
}

int get_node_addr(node_t* node){
   return node->connexion->infos.sin_addr.s_addr;
}

int get_node_fd(node_t* node){
   return node->connexion->fd;
}

connexion_t* connexion_create(char* addr, int port){
    assert(addr);
    
    connexion_t* cnx = (connexion_t*)malloc(sizeof(connexion_t));
    cnx->infos.sin_family = AF_INET;
    cnx->infos.sin_port = htons(port);
    cnx->infos.sin_addr.s_addr = inet_addr(addr);

    return cnx;
}

node_t* node_create(connexion_t* cnx){
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->connexion = cnx;
    node->active = false;
    node->id = -1;

    return node;
}

bool is_the_same_node(const node_t node1, const node_t node2){
    if(node1.id == node2.id){
        return true;
    }
    else{
        return false;
    }
}

