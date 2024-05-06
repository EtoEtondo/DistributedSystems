#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#define port 4070

int main(int argc, char *argv[]){
	struct sockaddr_in6 server, client;
	char message[1024];
	int sfd = 0;
	int  i, j, addrlen, fdmax = 0;
	int client_socket;
	int lesen;
	//int yes = 1;
	fd_set readfds;
	fd_set serverfd;
	
	FD_ZERO(&readfds);
	FD_ZERO(&serverfd);

	if((sfd = socket(AF_INET6, SOCK_STREAM, 0)) == -1){
		perror("Server socket error");
		exit(1);
	}
	printf("server socket ist ok\n");
	/*if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror("setsockopt error");
		exit(1);
	}*/
	//printf("setsockopt ist ok");
	server.sin6_flowinfo = 0;
	server.sin6_family = AF_INET6;
	server.sin6_addr = in6addr_any;//inet_addr("127.0.0.1");
	server.sin6_port = htons(port);
	//memset(&(server.sin6_zero), '\0', 8);
	//memset(&server,0,sizeof(server));
	//bzero((char *) &server, sizeof(server));
	if(bind(sfd,(struct sockaddr *)&server, sizeof(server)) < 0){
		perror("Bind failed\n");
		exit(1);
	}
	printf("bind ok\n");
	if(listen(sfd, 10) < 0){
		perror("listen fail\n");
		exit(1);
	}
	printf("listen ok\n");
	FD_SET(sfd, &serverfd);
	fdmax = sfd;

	//FD_SET(sfd, &readfds);
	//FD_SET(0, &readfds);
	for(;;){
	
	readfds = serverfd;	

	if(select(fdmax+1, &readfds, NULL, NULL, NULL) < 0){
		perror("Error in select");
		exit(1);
	}
	printf("select ok\n");
	for(j = 0; j <=fdmax; j++){
		if(FD_ISSET(j, &readfds)){
			if(j == sfd){
				addrlen = sizeof(client);
				client_socket = accept(sfd, (struct sockaddr *)&client, &addrlen);
				if(client_socket < 0){
					perror("accept failed");
				}
				else{
					printf("accept ok\n");
			
					FD_SET(client_socket, &serverfd);
					if(client_socket > fdmax){
						fdmax = client_socket;
					}//printf("%s: New connection from %s on socket %d\n", argv[0], inet_ntoa(client.sin6_addr), client_socket);
				}
			}
			else{
				if((lesen = recv(j, message, sizeof(message), 0)) <= 0){
					if(lesen == 0){
						printf("%s: socket %d hung up\n",argv[0], j);
					}
					else
						perror("recv() error");
					close(j);
					FD_CLR(j, &serverfd);
				}	
				else{
					for(i = 0; i <=fdmax; i++){
						if(FD_ISSET(i, &serverfd)){
							if(i != sfd && i != j)
								if(send(i, message, lesen, 0) == -1){
									perror("send error");
								}
						}
					}
				}
			}
		}
	}}
	return 0;
}
