/********************************************************************************
 ** FILENAME          : server.c
 ** DESCRIPTION       : server program is created to connect multiple clients 
 ** revision history:
 ** DATE                                NAME                         REASON
 ** _____________________________________________________________________________
 ** 17,oct                           group2                          creation of file , writing functional definitions
 **/



#include<header.h>
#include"common.c"
/*************************************************************************
 ** FUNCTION NAME                  : Printlist
 ** DESCRIPTION                    : To send all active clients in group to newly logged in clients
 ** RETURN                         : void
 **/

void printlist(int sock , int a){
	int i;
	char end[]="end";
	for(i=0;i<MAX_CLIENTS;i++)
	{
		if(clients[i]){
			send(sock , clients[i]->name ,strlen(clients[i]->name), 0);
			sleep(1);
		}
	}
	if(i == MAX_CLIENTS)
	{
		send(sock , end ,strlen(end), 0);
	}
	
}
/*************************************************************************
 ** FUNCTION NAME                  : login 
 ** DESCRIPTION                    : checking credentials of clients to login
 ** RETURN                         : void
 **/
void login(int sock, char *a){
	char y[4];
	strcpy(y, "yes");
	char n[3];
	strcpy(n ,"no");
	cstruct *q;
	char rec[4];
	q=head;
	while(q!=NULL){
		if(strcmp(q->name , a)==0){
			while(1){
				recv(sock , rec , 4, 0);
				if(strcmp(rec , q->pass)==0){
					send(sock , y , strlen(y), 0);
					break;
				}
				else{
					send(sock , n , strlen(n), 0);
				}
			}
			break;
		}
		else
			q=q->next;
	}
	
}
			

/*************************************************************************
 ** FUNCTION NAME                  : Reg
 ** DESCRIPTION                    : Getting credentials of new clients
 ** RETURN                         : void
 **/
void reg(int n,char *a){
	cstruct *temp;
	char rec[5];
	recv(n, rec , 4 , 0);
	temp =( cstruct *)malloc(1*sizeof(cstruct ));
	strcpy(temp->name , a);
	
	strcpy(temp->pass ,rec);
	if (head == NULL)
	{
		head = temp;
		temp->next =NULL;
	}
	else
	{
		temp->next =head;
		head = temp;
	}
}

 
/*************************************************************************
 ** FUNCTION NAME                  : check 
 ** DESCRIPTION                    : checking client entry in list 
 ** RETURN                         : 1 on success
 **/
int check(char *a){
	cstruct *new ;
	new =head;
	while(new !=NULL)
	{
		if(strcmp(new->name ,a )==0)
		{
			return 1;
		}
		else 
			new = new->next;
	}
	return 0;
}


/* Add clients to queue */
/*************************************************************************
 ** FUNCTION NAME                  : queue_add
 ** DESCRIPTION                    : adding new clients 
 ** RETURN                         : void
 **/
void queue_add(client_t *cl){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i){
		if(!clients[i]){
			clients[i] = cl;
			break;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

/* Remove clients to queue */
/*************************************************************************
 ** FUNCTION NAME                  : queue_remove 
 ** DESCRIPTION                    : removing client from list while log out
 ** RETURN                         : void
 **/
void queue_remove(int uid){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == uid){
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Handle all communication with the client */
/*************************************************************************
 ** FUNCTION NAME                  : handle_client 
 ** DESCRIPTION                    : login; register; send message; recieve message
 ** RETURN                         : void
 **/
void *handle_client(void *arg){
	char buff_out[BUFFER_SZ];
	char name[32];
	int leave_flag = 0;
	cli_count++;
	client_t *cli = (client_t *)arg;
	char no[]="no";
	char yes[]="yes";
	if(recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) <  2 || strlen(name) >= 32-1){
		printf("not valid name\n");
		leave_flag = 1;
	} else{
		strcpy(cli->name, name);
		int rc =check(name);  //checking client in registered list
		if (rc == 0){
			if( send(cli->sockfd , no , 2, 0)>0)
			{
				reg (cli->sockfd, name); //if not registering with password
			}
		}
		else
			send(cli->sockfd , yes , 3, 0);

		login(cli->sockfd , name);           //login
		printlist(cli->sockfd ,cli->uid);    //sending all the active clients for newly joined client
		sprintf(buff_out, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message(buff_out, cli->uid);     //sending message to all clients   
			
		bzero(buff_out, BUFFER_SZ);
                //recieving and sending message 
		while(1){
			if (leave_flag) {
				break;
			}

			int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
			if (receive > 0){
				if(strlen(buff_out) > 0){
					send_message(buff_out, cli->uid);
					str_trim_lf(buff_out, strlen(buff_out));
					printf("%s -> %s\n", buff_out, cli->name);
				}
			}
			else if (receive == 0 || strcmp(buff_out, "exit") == 0){
				sprintf(buff_out, "%s has left\n", cli->name);
				printf("%s", buff_out);
				send_message(buff_out, cli->uid);
				leave_flag = 1;
			} else {
				printf("ERROR: -1\n");
				leave_flag = 1;
			}
		}
		bzero(buff_out, BUFFER_SZ);
		/* Delete client from queue and yield thread */
		close(cli->sockfd);
		queue_remove(cli->uid);           //removing client from queue while logging out
		free(cli);
		cli_count--;
		pthread_detach(pthread_self());
	}
		return NULL;
}

/*************************************************************************
 ** FUNCTION NAME                  : main
 ** DESCRIPTION                    : create socket; bind ;listen to clients and do functional requirements 
 ** RETURN                         : EXIT_SUCCESS/EXIT_FAILURE
 **/

int main(){
	char *ip = "127.0.0.1";
	int port = 8880;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t tid;

	  /* Socket settings */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
//	serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);
		/* Bind */
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR: Socket binding failed");
		return EXIT_FAILURE;
	}

	  /* Listen */
	if (listen(listenfd, 1) < 0) {
		perror("ERROR: Socket listening failed");
		return EXIT_FAILURE;
	}
	printf("=== WELCOME TO THE CHATROOM ===\n");
	while(1){
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);
		/* Check if max clients is reached */
		if((cli_count ) == MAX_CLIENTS){
			printf("Max clients reached. Rejected: ");
			printf(":%d\n", cli_addr.sin_port);
			close(connfd);
			continue;
		}
		/* Client settings */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->sockfd = connfd;
		cli->uid = ++uid;
		/* Add client to the queue and fork thread */
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);
		/* Reduce CPU usage */
		sleep(1);
	}
	return EXIT_SUCCESS;
}

