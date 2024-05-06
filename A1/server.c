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

sock = socket(AF_INET,SOCK_DGRAM,0);

	if (sock < 0){

		perror("Fehler");	
	}


memset(&server,0,sizeof(server));

server.sin_port = htons(PORT);
server.sin_family = AF_INET;
server.sin_addr.s_addr = inet_addr("127.0.0.1");


	if(bind(sock , (struct sockaddr *) &server , sizeof(server))<0){
		perror("Fehler");

	}

while(1)
{
	printf("Warte auf Daten..\n\n");
	len = sizeof(client);
	n = recvfrom(sock , buf, BUFLEN , 0, (struct sockaddr *)&client ,&len);
	char *eingabe; 
	char trzeichen[] = " " ;
	double a,b = 0;
	int zaehler = 1;
	char *operator;
	double ergebnis = 0;
	printf("Empfangen : %s\n\n",buf);
	eingabe = strtok (buf,trzeichen);
	
	while(eingabe != NULL && zaehler == 1){
		a = atof(eingabe);
		zaehler++;
	}
	
	eingabe = strtok(NULL, trzeichen);

	while(eingabe != NULL && zaehler == 2){
		operator = eingabe;
		zaehler++;
	}

	eingabe = strtok(NULL, trzeichen);

	while(eingabe != NULL && zaehler == 3){
		b = atof(eingabe);
		zaehler++;
	}

	if(strcmp(operator , "+")==0){
	ergebnis = a+b;
	}
	if(strcmp(operator , "-")==0){
	ergebnis = a - b;
	}
	if(strcmp(operator , ":")==0 ||strcmp(operator, "/")==0){
	ergebnis = a/b;
	}
	if(strcmp(operator, "*")==0){
	ergebnis=a*b;
	}
	if(strcmp(operator,"%")==0){
	ergebnis=(int)a% (int)b ;
	}
	
	printf("Ergebnis : %.2f\n\n",ergebnis);
	bzero(buf,BUFLEN);
	sprintf(buf,"%.2f",ergebnis);

	m = sendto (sock , buf, strlen(buf) , 0 ,(struct sockaddr *) &client ,len);


	printf("Nächster Empfang ..\n\n ");
}

return EXIT_SUCCESS;



}
