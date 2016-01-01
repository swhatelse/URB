#include<stdbool.h>
#include <netinet/in.h>

#ifndef NODE_H
#define NODE_H

typedef struct connexion_t{
    struct sockaddr_in infos;
    int fd;
}connexion_t;


typedef struct node_t{
    connexion_t connexion;
    int id;
}node_t;

int my_id;
bool is_the_same_node(const node_t node1, const node_t node2);
#endif
