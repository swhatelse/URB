#include<stdio.h>
#include<stdbool.h>

#include"common.h"
#include"list.h"

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

typedef struct message_element_t{
    message_t* msg;
    bool* acks;
}message_element_t;

// Global vars
dlk_list_t already_received;
dlk_list_t not_received_yet;

// Functions
void insert_message(message_t* msg, dlk_list_t* list);
bool remove_message(dlk_list_t* list, const int id, const int node_id);
dlk_element_t* get_msg_from_list(dlk_list_t* list, const int node_id, const int msg_id);
bool is_already_in(const int msg_id, const int node_id, dlk_list_t* list);

void initialize_acks(bool** acks);
void acknowledge(message_t msg);
void add_ack(message_element_t** msg, int node_id);
void deliver(const message_t message);
void multicast(const message_t* msg, size_t size);
int beb(const void* content, size_t size);
int urb(const message_t message);
bool recv_all(int socket, void* buf, size_t length);

bool is_replicated(message_element_t* msg);
#endif
