#include<stdio.h>

#include"common.h"

#ifndef COMMUNICATION
#define COMMUNICATION

void receive_msg();
void deliver(void* message);
void beb(void* message);
void urb(void* message);
#endif
