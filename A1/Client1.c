#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#define PORT 4030
int check=2;
int main(int argc, char ** argv){
    /* This is the client */    
    
    /* Get a socket */
	int clientFD = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(clientFD == -1){
	    /* Exit if socket is invalid */
		perror("Failed to create a socket!");
		exit(1);
	}

	/* Create a sockaddr for the server address */
	struct sockaddr_in serverAddr;
	/* clear it */
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	/* fill it with values */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(4030);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* Create a sockaddr for the local address */
	/* If we don't bind a socket in the client, we won't be able to receive anything */
	struct sockaddr_in clientAddr;
	/* clear it */
	memset(&clientAddr, 0, sizeof(struct sockaddr_in));
	
	clientAddr.sin_family = AF_INET;
	/* Port = 0 - Let the kernel decide */
	clientAddr.sin_port = htons(0);
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY);

    /* Create data for the server */

	char * data_string;
	if(argc > 1){
	    /* use parameter (if there is one) */

	    data_string = strdup(argv[1]);
	} else {
	    data_string = "test";
	}
	void * data_send = (void *) data_string;
	int dataLen = strlen(data_string) + 1; // +1 for the \0 byte
	
	/* bind the clientaddr */
	if(bind(clientFD, (struct sockaddr *) &clientAddr, sizeof(struct sockaddr_in))){
		perror("Failed to bind socket!");
		exit(1);
	}
while(check)
{
    /* Send the data to the server */
    /* sendto() - socket, data pointer, data size, flags, sockaddr struct, size of this struct */
	size_t bytesSend = sendto(clientFD, data_send, dataLen, MSG_CONFIRM, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr_in));

    if(bytesSend >= 0){
        printf("Sendto() OK - %s\n", data_string);
        /* if sendto() did not fail with -1 */
        /* this is nearly the same as in the server */
	    
	    /* addrLen - size of the sockaddr struct */ 
        socklen_t addrLen = sizeof(struct sockaddr_in);
        
        /* setting buffer + bufferSize for recvfrom() */
        size_t maxDataSize = 512;
        void * data_recv = malloc(maxDataSize);
    
/* select */
fd_set rfds, cprfds;

struct timeval tv;
int retval;
//watch stdin (fd 0)to see wehn it has input
FD_ZERO(&rfds);
FD_SET(clientFD,&rfds);
cprfds=rfds;
//wait up to five sec
tv.tv_sec=5;
tv.tv_usec=0;


retval=select(4,&rfds,NULL,NULL,&tv);

if(retval==-1)
{
  perror("select()");
}
else if(retval)
	{      /* recvfrom() - socket, allocated buffer, buffer size, 			flags, client-addr-struct, size of struct */
        	size_t recSize = recvfrom(clientFD, data_recv, 			maxDataSize, MSG_WAITALL, (struct sockaddr *) 			&serverAddr, &addrLen);
		if(recSize){
			printf("Recieve OK! - %s\n", (char *) 				data_recv);
			printf("data iz da! 	\n");			
			break;		
		} 
		else {
			puts("Recieve failed!");
		}
    	} 		
	

else
	{
	  printf("keine data \n");
	  check--;
	}		
 }

}	return 0;
}
