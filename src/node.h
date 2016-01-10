#include<stdbool.h>
#include <netinet/in.h>

#ifndef NODE_H
#define NODE_H

// Types
typedef struct connexion_t{
    struct sockaddr_in infos;
    int fd;
}connexion_t;


typedef struct node_t{
    connexion_t* connexion; // Connexion for accepting connexions
    int id;
    bool active;
}node_t;

// Globals
int my_id;

// Functions
int get_node_port(node_t* node);
int get_node_addr(node_t* node);
int get_node_fd(node_t* node);

bool is_the_same_node(const node_t node1, const node_t node2);
#endif
