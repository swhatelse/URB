#include<stdio.h>

#include"common.h"

#ifndef COMMUNICATION
#define COMMUNICATION

#define IS_MSG(x)                               \
    x.type == 'M'

#define IS_ACK(x)                               \
    x.type == 'A'

typedef struct message_t{
    char type;
    void* message;
}message_t;

void receive_msg();
void deliver(const message_t message);
int beb(const message_t msg);
int urb(const void* message);
#endif
