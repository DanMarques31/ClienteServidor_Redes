#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // Inicialize a semente aleatória com o tempo atual
    srand(time(NULL));

    // Gere um número aleatório no intervalo de 1 a 10
    int numeroAdivinhar = (rand() % 10) + 1;

    printf("Número a ser adivinhado: %d\n", numeroAdivinhar);

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Crie um socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Ligue o socket ao endereço e porta
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Erro ao ligar o socket");
        exit(1);
    }

    // Espere por conexões
    listen(serverSocket, 5);
    printf("Aguardando conexões...\n");

    // Aceite uma conexão
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);

    if (clientSocket == -1) {
        perror("Erro ao aceitar a conexão");
        exit(1);
    }

    printf("Conexão estabelecida com um cliente.\n");

    int tentativas = 0;
    int adivinhacao;

    while (1) {
        recv(clientSocket, &adivinhacao, sizeof(int), 0);
        tentativas++;

        if (adivinhacao == numeroAdivinhar) {
            printf("Cliente acertou em %d tentativas.\n", tentativas);
            send(clientSocket, &tentativas, sizeof(int), 0);
            break;
        } else {
            send(clientSocket, &tentativas, sizeof(int), 0);
        }

        if (tentativas >= 3) {
            printf("Cliente excedeu o número máximo de tentativas. O número correto era %d.\n", numeroAdivinhar);
            break;
        }
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
