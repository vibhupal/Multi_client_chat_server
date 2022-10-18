/********************************************************************************
 * FILENAME : login.c
 * DESCRIPTION:login to server by giving username and password
 * revision history:
 * DATE                                NAME                         REASON
 * _____________________________________________________________________________
 * 17,oct                           group2                          creation of file , writing functional definitions
*/


#include<header.h>
#include"common_client.c"
//registerting for first time by setting password

/*************************************************************************
 * FUNCTION NAME                  : set
 * DESCRIPTION                    : To set password in the list
 * RETURN                         : 0(Success), 1(Failure)
 */
int set(int a){
	printf("not registered \n");
	while(1){
		char pass1[5],pass2[5];
		printf("enter password to register\n");
		fgets(pass1 , 5, stdin);                        //getting pasword to register
	        str_trim_lf(pass1, strlen(pass1));
		if(strlen(pass1) < 5&&strlen(pass1) >2){
			while(1){
				printf("reenter the password\n");
				fgets(pass2 , 5, stdin);
				str_trim_lf(pass2,strlen(pass2));
				if(strcmp(pass1, pass2)==0)      
				{
					send(a , pass1 , strlen(pass1), 0); //sending password to server to register for first time
					return 0;
				}
				else
					printf("enter valid password\n");
			}
		}	
		else
			printf("password should be 2 to 4 digits \n");
	}
	return 1;
}

/****** for already registered checking password**************/

/*************************************************************************
 * FUNCTION NAME                  : Valid
 * DESCRIPTION                    : To check the entered password ,matches with registered password
 * RETURN                         : 0(Success),-1(Failure)
 */

int valid (int a)
{
	char ch[3];
	recv(a , ch ,3,0);
	if (strcmp(ch ,"yes")==0)
	{
		return 0;
	}
	else 
		return -1;
}

/************printing active list of clients in the group***********/
/*************************************************************************
 * FUNCTION NAME                  : Activelist
 * DESCRIPTION                    : To get all the active client in the group
 * RETURN                         : void
 */
void activelist(int a){
	char p[32];
	while(1){
		memset(p, 0, sizeof(p));
		recv(a ,p,32,0);
		if(strcmp(p ,"end")==0){
			break;
		}
		else
			printf("%s\n",p);
	}
}
