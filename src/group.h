#ifndef GROUP_H
#define GROUP_H
#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netdb.h>

#include"common.h"
#include"node.h"

// Types
typedef struct group_t{
    node_t **nodes;
    int count;
}group_t;

// Globals
group_t send_sockets;
pthread_mutex_t send_sockets_mtx;
group_t receive_sockets;
pthread_mutex_t receive_sockets_mtx;

// Functions
int connection(const char* addr, const int port);
void join();
int add_node(const int fd, const struct sockaddr_in addr);
void remove_node(node_t* node);
void* message_handler();
#endif
