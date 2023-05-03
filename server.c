#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include "gui.h"

#define screenWidth 1600
#define screenHeight 900

#define PORT 9001
#define BUFF_SIZE 5000

int main(int argc, char const* argv[])
{
	InitWindow(screenWidth, screenHeight, "Arduino movement");

	Camera camera = { 0 };

	initCamera(&camera);

	Model model = createModel();

	float pitch = 0.0f;

	float roll = 0.0f;

	float yaw = 0.0f;

	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;

	Vector3 position = { posX,posY,posZ};

	SetTargetFPS(120);


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

//	 COMMUNICATION BETWEEN CLIENT 1 & 2
	char *token;
	char delim[1] = ";";
	while (1) {
		bzero(buffer, BUFF_SIZE);
		valread = recv(client1, buffer, BUFF_SIZE, 0);
		if (valread > 0) {
			printf("Recv: %s\n", buffer);
			token = strtok(buffer, ";");
			while (token != NULL) {
				send(client2, token, strlen(token), 0);
				send(client2, delim, 1, 0);
				token = strtok(NULL, ";");
			}
		}
	}

	// COMMUNICATION BETWEEN CLIENT 1 & 2
	char *token;
	char delim[1] = ";";
	while (!WindowShouldClose()) {
		bzero(buffer, BUFF_SIZE);
		valread = recv(client1, buffer, BUFF_SIZE, 0);
		if (valread > 0) {
			strtok(buffer, "Gx");
			token = strtok(NULL, "Gy");
			pitch = atof(token);
			token = strtok(NULL, "Gz");
			roll = atof(token);
			token = strtok(NULL, "Ax");
			yaw = atof(token);
			token = strtok(NULL, "Ay");
			posX = atof(token);
			token = strtok(NULL, "Az");
			posY = atof(token);
			token = strtok(NULL, "G");
			posZ = atof(token);
			token = strtok(NULL, ";");

			send(client2, token, strlen(token), 0);
			send(client2, delim, strlen(delim), 0);
		}
		handleInput(&pitch, &roll, &yaw, &position);

		updateModel(&model, pitch, roll, yaw);
		camera.position = (Vector3){ position.x, position.y + 50.0f, position.z - 60.0f };
		camera.target = (Vector3){ position.x, position.y, position.z };
		draw(&model, &camera);
	}

	// CLOSE CLIENT CONNECTION
	CloseWindow();
	printf("Exiting program...\n");
	close(client1);
	close(client2);
	// CLOSE SOCKET
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
