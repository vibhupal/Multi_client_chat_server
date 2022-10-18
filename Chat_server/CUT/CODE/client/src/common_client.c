/********************************************************************************
 ** FILENAME               : commonclient.c
 ** DESCRIPTION            : To send and receive messages
 ** revision history:
 ** DATE                                NAME                         REASON
 ** _____________________________________________________________________________
 ** 17,oct                           group2                          creation of file , writing functional definitions
 **/


#include<header.h>

void str_overwrite_stdout() {
	printf("%s", "> ");
	fflush(stdout);
}

/* to trim \n character from the fgets input*/
/*************************************************************************
 ** FUNCTION NAME                  : tr_trim_lf
 ** DESCRIPTION                    : makes final char \n to 0 from user input
 ** RETURN                         : void
 **/
void str_trim_lf (char* arr, int length) {
	int i;
	for (i = 0; i < length; i++) { 
		if (arr[i] == '\n') {
			arr[i] = '\0';
			break;
		}
	}
}
/********************************************/

/*************************************************************************
 ** FUNCTION NAME                  : catch_ctrl_c_and_exit 
 ** DESCRIPTION                    : set flag value to send signal to stop
 ** RETURN                         : void
 **/

void catch_ctrl_c_and_exit(int sig) {
	    flag = 1;
}
/***********************************/

/*sending message to server*/
/*************************************************************************
 ** FUNCTION NAME                  : send_msg_handler
 ** DESCRIPTION                    : To send messages to server
 ** RETURN                         : void
 **/
void send_msg_handler() {
	char message[LENGTH] = {};
	char buffer[LENGTH + 32] = {};

	while(1) {
		str_overwrite_stdout();
		fgets(message, LENGTH, stdin);
		str_trim_lf(message, LENGTH);

		if (strcmp(message, "exit") == 0) {
			break;
		} else {
			sprintf(buffer, "%s: %s\n", name, message);
			send(sockfd, buffer, strlen(buffer), 0);  // sending message to server
		}
		bzero(message, LENGTH);
		bzero(buffer, LENGTH + 32);
	}
	catch_ctrl_c_and_exit(2);  // to logout if client enters exit
}

/******************************/
//recieving input from multi clients

/*************************************************************************
 ** FUNCTION NAME                  : recv_msg_handler
 ** DESCRIPTION                    : To receive messages from server
 ** RETURN                         : void
 **/
void recv_msg_handler() {
	char message[LENGTH] = {};
	while (1) {
		int receive = recv(sockfd, message, LENGTH, 0);
		if (receive > 0) {
			printf("%s", message);
			str_overwrite_stdout();
		} else if (receive == 0) {
			break;
		} else {

		}
		memset(message, 0, sizeof(message));
	}
}
