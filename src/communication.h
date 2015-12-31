#include<stdio.h>
#include<stdbool.h>

#include"common.h"

#ifndef COMMUNICATION
#define COMMUNICATION

typedef struct message_t{
    char type;
    int id;
    void* message;
}message_t;

typedef struct message_list_t{
    message_t msg;
    node_t sender;
    struct message_list_t* prev;
    struct message_list_t* next;
}message_list_t;

message_list_t *already_received;

bool is_already_in(message_t msg, node_t sender);
void acknowledge(message_t msg);
void deliver(const message_t message);
int beb(const message_t msg);
int urb(const message_t message);
#endif
