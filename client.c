#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Crie um socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Conecte-se ao servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Erro ao conectar ao servidor");
        exit(1);
    }

    int adivinhacao;
    int tentativas = 0;

    while (1) {
        printf("Tentativa %d: ", tentativas + 1);
        scanf("%d", &adivinhacao);

        send(clientSocket, &adivinhacao, sizeof(int), 0);
        recv(clientSocket, &tentativas, sizeof(int), 0);

        if (adivinhacao == tentativas) {
            printf("Correto! Você adivinhou em %d tentativas.\n", tentativas);
            break;
        } else {
            printf("Incorreto. Tente novamente.\n");
        }

        if (tentativas >= 3) {
            printf("Você excedeu o número máximo de tentativas.\n");
            break;
        }
    }

    close(clientSocket);

    return 0;
}
