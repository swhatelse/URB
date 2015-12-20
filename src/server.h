#ifndef SERVER_H
#define SERVER_H
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

#include"common.h"
void connexion_init();
int connexion_accept();
void connexion_handler();
#endif
