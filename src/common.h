#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<stdbool.h>
#include<pthread.h>
#include<assert.h>

#ifndef COMMON_H
#define COMMON_H

/******************************************
 *
 *               Macros
 *
 *****************************************/
#define PRINT(string)                                 \
    printf("%d: %s\n" , my_port, string)

/******************************************
 *
 *               Types
 *
 *****************************************/

typedef struct node_t{
    struct sockaddr_in infos;
    int fd;
    bool alive;
}node_t;

/******************************************
 *
 *               Global vars
 *
 *****************************************/

int running;
int my_port;

/******************************************
 *
 *               Functions
 *
 *****************************************/

int init(char *file, char *my_addr, int port);
bool is_the_same_node(node_t node1, node_t node2);
#endif
