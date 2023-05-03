#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#define PORT 9001
#define BUFF_SIZE 5000

int main(int argc, char const* argv[])
{
	int server_fd, client1, client2, valread;
	struct sockaddr_in servAddr, address1, address2;
	int opt = 1;
	int addrlen1 = sizeof(address1);
	int addrlen2 = sizeof(address2);
	char buffer[BUFF_SIZE] = { 0 };

	// CREATE SOCKET
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket ERROR: %s", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("Socket SUCCESS\n");
	}

	// SET SOCKET OPTIONS
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		printf("Socket opt. ERROR: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(PORT);

	// BIND SOCKET TO PORT 9001
	if (bind(server_fd, (struct sockaddr*)&servAddr,
			sizeof(servAddr))
		< 0) {
		printf("Bind ERROR: %s", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("Bind SUCCESS\n");
	}

	// LISTEN FOR CLIENTS
	if (listen(server_fd, 2) < 0) {
		printf("Listen ERROR: %s", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("Listening...\n");
	}

	// CONNECTING CLIENT 1
	if ((client1
		= accept(server_fd, (struct sockaddr*)&address1,
				(socklen_t*)&addrlen1))
		< 0) {
		printf("Accept client 1. ERROR: %s", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("Connected to client 1.\n");
	}

	// CONNECTING CLIENT 2
	if ((client2
		= accept(server_fd, (struct sockaddr*)&address2,
				(socklen_t*)&addrlen2))
		< 0) {
		printf("Accept client 2. ERROR: %s", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("Connected to client 2.\n");
	}

	// COMMUNICATION BETWEEN CLIENT 1 & 2
//	char *token;
//	char delim[1] = ";";
//	while (1) {
//		bzero(buffer, BUFF_SIZE);
//		valread = recv(client1, buffer, BUFF_SIZE, 0);
//		if (valread > 0) {
//			printf("Recv: %s\n", buffer);
//			token = strtok(buffer, ";");
//			while (token != NULL) {
//				send(client2, token, strlen(token), 0);
//				send(client2, delim, 1, 0);
//				token = strtok(NULL, ";");
//			}
//		}
//	}

	// COMMUNICATION BETWEEN CLIENT 1 & 2
	while (1) {
		bzero(buffer, BUFF_SIZE);
		valread = recv(client1, buffer, BUFF_SIZE, 0);
		if (valread > 0) {
			printf("Recv: %s\n", buffer);
			send(client2, buffer, strlen(buffer), 0);
		}
	}

	// CLOSE CLIENT CONNECTION
	printf("Exiting program...\n");
	close(client1);
	close(client2);
	// CLOSE SOCKET
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
