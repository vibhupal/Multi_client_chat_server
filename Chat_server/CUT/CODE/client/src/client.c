/*******************************************************************************
**FILENAME             :client.c
**DESCRIPTION          :The client program is created to get cinnectd with server , login with password and start chatting with
                        multiple client
**REVISION HISTORY     :
**DATE                  NAME                             REASON
**____________________________________________________________________________
**17,oct                group2                            	creating client file
**
**__________________________________________________________________________
*/

#include<header.h>
#define LENGTH 2048
#include"login.c"
//#include"common_client.c"
pthread_mutex_t lock;

int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}
	char pass[5];
	char rec[15];
	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	signal(SIGINT, catch_ctrl_c_and_exit);
	printf("Please enter your name: ");
	fgets(name, 32, stdin);                 //client enters the name
	str_trim_lf(name, strlen(name));
	if (strlen(name) > 32 || strlen(name) < 2){
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}


	struct sockaddr_in server_addr;

	/* Socket settings */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);


        /*socket connection with server*/
	int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err == -1) {
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

        
	send(sockfd, name, 32, 0);//sending client name to check for registeration
	recv(sockfd ,rec ,15 ,0);
	if(strcmp(rec , "no")==0)
	{
		int a = set(sockfd ); //if not registered setting password
		if (a ==0)
		{
			printf("registered\n");
		}
		else
		{
			printf("registeration failed\n ");
			exit(0);
		}
	} 
	//after registeration logging in with password
	while(1){
		printf("please enter the password to login\n");
		fgets(pass , 5 , stdin);
	        str_trim_lf(pass, strlen(pass));
		send(sockfd , pass , strlen(pass) ,0) ;
		int val = valid(sockfd);//checking password with registered password
		if(val ==0)
			break;
		else{
			printf("password incorrecct\n");
		}
	}
	//after login getting activelist
	printf("/************************ACTIVE CLIENTS*************************/\n");
	activelist(sockfd);
	printf("/************************ACTIVE CLIENTS*************************/\n");
	/************************ACTIVE CLIENTS*************************/

	printf("=== WELCOME TO THE CHATROOM ===\n");
	//creating thread to send and recieve message
	pthread_t send_msg_thread;
	if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}
	pthread_t recv_msg_thread;
	if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}
	while (1){
		if(flag){
			printf("\nBye\n");
			break;
		}
	}
	close(sockfd);
	return EXIT_SUCCESS;
}

