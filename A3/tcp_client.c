#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

int main(){
	int sfd, id;
	struct sockaddr_in6 server;
	//fprintf(stdout,"nach sockaddr struct\n");
	char client_message[1000], server_message[2000], name[100], buffer[1024];
	//struct addrinfo req, *ans;
	//fprintf(stdout,"nach char initialisierung\n");
	//struct hostent, *servera;
	//printf("test");
	//req.ai_flags = 0;
	sfd = socket(AF_INET6, SOCK_STREAM, 0);
	printf("socket erstellt");
	memset(&server,0 ,sizeof(server));	
	
	//servera = gethostbyname2("127.0.0.1", AF_INET6);

	server.sin6_flowinfo = 0;
	//memmove((char *) &server.sin6_addr, (char *) servera->h_addr, servera->h_length);
	server.sin6_addr = in6addr_loopback;
	server.sin6_family = AF_INET6;
	server.sin6_port = htons(4070);
	printf("before connecting to the Server please enter your Username: ");
	scanf("%s",name);
	memset(&buffer,0,sizeof(buffer));
	if(connect(sfd, (struct sockaddr *)&server, sizeof(server))<0){
		perror("Connect failed\n");
		return 1;
	}
	id = fork();
	while(1){
		if(id == -1){
			exit(EXIT_FAILURE);
		}
		else if(id == 0){
			//printf("Enter message: ")
			fgets(buffer,999,stdin);
			//scanf("%s", client_message);
			//printf("Test: %s\n", client_message);
			sprintf(client_message,"%s: %s", name, buffer);
			if(strlen(buffer) > 2)
				send(sfd, client_message, strlen(client_message), 0);
			//write(sfd, client_message, sizeof(client_message));
		}
		else{
			//recv(sfd, server_message, 2000, 0);
			memset(&server_message,0,sizeof(server_message));
			read(sfd, server_message, sizeof(server_message));
			fputs(server_message, stdout);
			//printf("Server: %s\n", server_message);
		}
	}

	//close(sfd);
	return 0;
}

