#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h> // Подключаем библиотеку для работы с потоками
#include <limits.h>
#include <errno.h>

#define PORT 8080

void *client_handler(void *socket_desc) {
	int client_socket = *((int*)socket_desc);
	char buffer[1024] = {0};
	int valread;

	valread = read(client_socket, buffer, 1024);
	send(client_socket, buffer, strlen(buffer), 0);

	char *endptr;
	errno = 0;
	long client_number = strtol(buffer, &endptr, 10);

	// Проверка ошибок при преобразовании и сравнение чисел
	if((errno == ERANGE && (client_number == LONG_MAX || client_number == LONG_MIN)) || (errno != 0 && client_number == 0)) {
		perror("strtol");
	}
	else if(endptr == buffer) {
		fprintf(stderr, "No digits were found\n");
	}
	else {
		int random_number = 5; //random_num(); // Генерация случайного числа
		if(client_number == random_number) {
                    char msg[] = "yes this is the number\n";
                    send(client_socket, msg, strlen(msg), 0);
                } else {
                    char msg[] = "no this is not the number\n";
                    send(client_socket, msg, strlen(msg), 0);
                }
	}

	close(client_socket);
	return NULL;
	}

int main() {
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 3);

	while(1) {
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

		pthread_t thread_id;
		if(pthread_create(&thread_id, NULL, client_handler, (void*)&new_socket) < 0) {
			perror("could not create thread");
			return 1;
		}

		pthread_detach(thread_id);
	}

	return 0;
}
