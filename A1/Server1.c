#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 4030

int main(int argc, char ** argv){
    /* This is the server */

    /* Get a socket */
	int serverFD = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(serverFD == -1){
	    /* Exit if we got an invalid file descriptor */
		perror("Failed to create a socket!");
		exit(1);
	}
	
    /* Create and clear our socket-address */
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(struct sockaddr_in));

    /* Fill it with values */
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
    /* sck.sin_addr is set to 0.0.0.0 since we implicitly set it by clearing the memory */

    /* Now bind this socket + address to listen on the given port */
	if(bind(serverFD, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in))){
	    /* Exit if bind fails */
		perror("Failed to bind socket!");
		exit(1);
	}

    /* Infinite loop to listen */
	while(1){
	    /* Creating an empty sockaddr struct */
	    /* This will store the source address in recvfrom() */
	    // using pointers and calloc here (same result as with sck above)
	    struct sockaddr_in * client;
		client = calloc(1, sizeof(struct sockaddr_in));
	    
	    /* addrLen - size of the sockaddr struct */ 
        socklen_t addrLen = sizeof(struct sockaddr_in);
        
        /* setting buffer + bufferSize for recvfrom() */
        size_t maxDataSize = 512;
        void * data = malloc(maxDataSize);
    
        /* recvfrom() - socket, allocated buffer, buffer size, flags, client-addr-struct, size of struct */
        size_t recSize = recvfrom(serverFD, data, maxDataSize, MSG_WAITALL, (struct sockaddr *) client, &addrLen);
        /* if recvfrom() fails, it will return -1 */
		if(recSize >= 0){
		    
		    /* print source addr IP and port */
            printf("(%s , %d) said: ",inet_ntoa(client->sin_addr), ntohs(client->sin_port));
            
            /* print data */
		 	printf("%s\n", (char *) data);
			
			/* send the same data back */
			/* sendto() - socket, data to send, size of data, flags, destination addr struct, size of this struct */
			size_t sendSize = sendto(serverFD, data, recSize, MSG_CONFIRM, (struct sockaddr *) client, sizeof(struct sockaddr_in));
			/* if sendto fails, it will return -1 */
			if(sendSize >= 0){
				puts("Send OK!");
			} else {
				puts("Send failed!");
			}
		} else {
			puts("Recieve failed!");
		} /* if */
	} /* while */
	return 0;
}
