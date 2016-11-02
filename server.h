#ifndef SERVER_H
#define SERVER_H

struct server
{
	int port;
	void (*handler)(char* request, char* cliip, int sockfd);
};

void serverListen(struct server* srv);

#endif
