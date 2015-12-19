#ifndef CLIENT_H
#define CLIENT_H
#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

int connection(const char* addr);
#endif
