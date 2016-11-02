#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

#include "server.h"

void handler(char* request, char* cliip, int sockfd)
{
	char* reply = "HTTP/1.1 200 OK\r\n\r\nHello World !";
	printf("New connexion from %s !\n%s\n", cliip, request);
	send(sockfd, reply, strlen(reply), 0);
}

int main()
{
	struct server srv;

	srv.port = 8080;
	srv.handler = &handler;

	serverListen(&srv);

	return 0;
}
