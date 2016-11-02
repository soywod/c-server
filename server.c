#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

#include "server.h"

#define BUFFER 512

typedef struct server S;

void serverListen(S* srv)
{
	int sockfd, newsockfd, sockopt, clilen, pid, chunk, i;
	struct sockaddr_in srvaddr, cliaddr;
	char *buffer, *request;
	char cliip[INET_ADDRSTRLEN];

	clilen = sizeof(cliaddr);

	// Open inet stream socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) return perror("socket()");

	// Set socket options
	sockopt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt);

	// Init server address
	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = INADDR_ANY;
	srvaddr.sin_port = htons(srv->port);

	// Bind & listen socket
	if (bind(sockfd, (struct sockaddr*) &srvaddr, sizeof(srvaddr)) < 0) return perror("bind()");
	if (listen(sockfd, 5) < 0) return perror("listen()");

	// Listen to connexions
	while(1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, (socklen_t *) &clilen);
		if (newsockfd < 0) return perror("accept()");

		// Child process
		pid = fork();

		if (pid < 0) return perror("fork()");
		if (pid != 0) close(newsockfd);
		else
		{
			i = 0;
			close(sockfd);

			// Init buffer
			buffer = malloc(BUFFER + 1);
			if (buffer == NULL) return perror("buffer malloc()");
			memset(buffer, 0, BUFFER + 1);

			// Get the complete http request
			while((chunk = recv(newsockfd, buffer, BUFFER - 1, MSG_DONTWAIT)) > 0)
			{
				if (i++ == 0)
				{
					request = malloc(BUFFER * i + 1);
					if (request == NULL) return perror("request malloc()");
					strcpy(request, buffer);
				}
				else
				{
					request = realloc(request, (BUFFER * i + 1));
					if (request == NULL) return perror("request realloc()");
					strcat(request, buffer);
				}

				// Reset buffer
				memset(buffer, 0, BUFFER + 1);
			}

			// Get client IP and convert
			inet_ntop(AF_INET, &(cliaddr.sin_addr), cliip, INET_ADDRSTRLEN);

			// Call handler
			srv->handler(request, cliip, newsockfd);

			free(buffer);
			free(request);
			close(newsockfd);
			return;
		}
	}

	return;
}
