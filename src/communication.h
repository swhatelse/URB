#include<stdio.h>
#include<stdbool.h>

#include"common.h"

#ifndef COMMUNICATION
#define COMMUNICATION

// Type definition
typedef struct message_t{
    char type;
    int sequence_nb;
    node_t sender;
    void* content;
}message_t;

typedef struct message_list_t{
    message_t* msg;
    int id;
    struct message_list_t* prev;
    struct message_list_t* next;
}message_list_t;

// Global vars
message_list_t *already_received;

// Functions
void insert_message(message_t* msg, message_list_t* list);
bool remove_message(const int id);
bool is_already_in(const message_t msg, message_list_t* list);
void acknowledge(message_t msg);
void deliver(const message_t message);
int beb(const message_t msg);
int urb(const message_t message);
#endif
