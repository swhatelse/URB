#include<stdio.h>
#include<stdbool.h>

#include"common.h"

#ifndef COMMUNICATION
#define COMMUNICATION

// Type definition
typedef struct message_t{
    char type;
    int id; // Id of the message
    /* node_t sender; */
    int node_id; // Id of origin node
    void* content;
}message_t;

// Used to authenticate a node
typedef struct message_id_t{
    char type;
    int node_id;
}message_id_t;

typedef struct message_ack_t{
    char type;
    int message_id;
    int node_id;
}message_ack_t;

typedef struct message_list_t{
    message_t* msg;
    /* int id; */
    bool* acks;
    /* int count_ack; */
    struct message_list_t* prev;
    struct message_list_t* next;
}message_list_t;

// Global vars
message_list_t *already_received;

// Functions
void insert_message(message_t* msg, message_list_t** list);
bool remove_message(const int id, const int node_id);
message_list_t* get_msg(message_list_t** list, const int node_id, const int msg_id);
bool is_already_in(const int msg_id, const int node_id, message_list_t* list);

void acknowledge(message_t msg);
void add_ack(message_list_t** msg, int node_id);
void deliver(const message_t message);
int beb(const void* content, size_t size);
int urb(const message_t message);
#endif
