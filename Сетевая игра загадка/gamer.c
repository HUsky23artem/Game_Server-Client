
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
int sock = 0;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};

if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
printf("\n Socket creation error \n");
return -1;
}

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);

if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
printf("\nInvalid address/ Address not supported \n");
return -1;
}

if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
printf("\nConnection Failed \n");
return -1;
}

int number;
printf("Enter a number: ");
scanf("%d", &number); // запрос на ввод числа у пользователя

char number_str[10]; // Массив символов для хранения строки с числом
sprintf(number_str, "%d", number); // Преобразовать число в строку

send(sock, number_str, strlen(number_str), 0); // Отправить строку с числом на сервер
read(sock, buffer, 1024);
printf("Message from server: %s\n", buffer);

return 0;
}
