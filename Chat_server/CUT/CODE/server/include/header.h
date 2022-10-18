#ifndef HEADER_H
#define HEADER_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
static int cli_count = 0;
static int uid = 0;
typedef struct client{
	char name[32];
	char pass[5];
	int status ;
	struct client *next;
}cstruct;
cstruct *head =NULL;
typedef struct{
	int sockfd;
	int uid;
	char name[32];
	int status;
} client_t;
client_t *clients[MAX_CLIENTS];

#endif
