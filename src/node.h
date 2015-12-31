#include<stdbool.h>
#include <netinet/in.h>

#ifndef NODE_H
#define NODE_H

typedef struct node_t{
    struct sockaddr_in infos;
    int fd;
    bool alive;
}node_t;

bool is_the_same_node(const node_t node1, const node_t node2);
#endif
