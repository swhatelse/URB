#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<stdbool.h>
#include<pthread.h>
#include<assert.h>

#include"node.h"

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
#endif
