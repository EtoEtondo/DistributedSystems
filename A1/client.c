#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFLEN 1432
#define PORT 4030

int main(){

struct sockaddr_in server ,client;

char buf[BUFLEN];
int n,m, sock;

socklen_t len;
len = sizeof(server);

sock = socket(AF_INET,SOCK_DGRAM,0);

if (sock < 0){

	perror("Fehler");	
}


memset(&server,0,sizeof(server));

server.sin_port = htons(PORT);
server.sin_family = AF_INET;
server.sin_addr.s_addr = inet_addr("127.0.0.1");

bzero(buf,BUFLEN);
do{
printf("Eingabe: ");
fgets(buf,BUFLEN,stdin);



n=sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&server , len);

if (n<0){
	perror("Fehler");
}

m=recvfrom(sock,buf,strlen(buf),0,(struct sockaddr *)&server ,&len);
printf("%s\n",buf);


}while(1);

if (n<0){
	perror("Fehler");
	printf("Echo from server: %s",buf);
}




return EXIT_SUCCESS;



}
