#include<header.h>

/*************************************************************************
 ** FUNCTION NAME                  : str_trim_lf
 ** DESCRIPTION                    : /n char of input or 0
 ** RETURN                         : void
 **/
void str_overwrite_stdout() {
	printf("\r%s", "> ");
	fflush(stdout);
}
void str_trim_lf (char* arr, int length) {
	int i;
	for (i = 0; i < length; i++) { 
		if (arr[i] == '\n') {
			arr[i] = '\0';
			break;
		}
	}
}

/* Send message to all clients except sender */
/*************************************************************************
 ** FUNCTION NAME                  : send_message
 ** DESCRIPTION                    : sending message to active clients
 ** RETURN                         : void
 **/
void send_message(char *s, int uid){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i<MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid){
				if(write(clients[i]->sockfd, s, strlen(s)) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}
