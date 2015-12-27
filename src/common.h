#ifndef COMMON_H
#define COMMON_H
#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<stdbool.h>

#include"client.h"


typedef struct node_t{
    struct sockaddr_in infos;
    int fd;
    bool alive;
    struct node_t* next;
    struct node_t* prev;
}node_t;

typedef struct group_t{
    node_t *nodes;
    int count;
}group_t;

group_t group;
group_t send_sockets;
group_t receive_sockets;
int running;
#endif
