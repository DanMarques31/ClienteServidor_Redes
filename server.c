#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORTA 8080
#define MAX_BUFFER 1024

int main() {
    
    int socket_server, socket1, socket2;
    struct sockaddr_in address; // Estrutura com detalhes do server (Endereço IP, porta)
    int opcao = 1;
    int addrlen = sizeof(address); // Tamanho da estrutura addres
    char buffer1[MAX_BUFFER] = {0}; // Buffers para armazenamento das mensagens recebidas dos clientes
    char buffer2[MAX_BUFFER] = {0};
    char *mensagem; // String usada para enviar mensagens aos clientes

    // Criação do socket do servidor
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("Falha na criação do socket"); // Se a chamada da func = 0 então falhou na criação do socket
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET; // Indica uso de endereço IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Permite conexão nos endereços da máquina
    address.sin_port = htons(PORTA); // Conversão da porta para formato de rede

    // Vinculando o socket à porta 8080
    if (bind(socket_server, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha na vinculação do socket à porta");
        exit(EXIT_FAILURE);
    }
    
    // Colocando o servidor para escutar
    if (listen(socket_server, 3) < 0) { // Até 3 conexões na fila
        perror("Falha na escuta");
        exit(EXIT_FAILURE);
    }

    printf("Aguardando conexões...\n");

    // Aceitando uma conexão
    if ((socket1 = accept(socket_server, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Falha na aceitação da conexão");
        exit(EXIT_FAILURE);
    }

    printf("Primeiro jogador conectado! Aguardando o segundo jogador...\n");

    // Aceitando uma segunda conexão
    if ((socket2 = accept(socket_server, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Falha na aceitação da segunda conexão");
        exit(EXIT_FAILURE);
    }

    printf("Segundo jogador conectado! Iniciando o jogo...\n");

    // Lógica do jogo
    srand(time(0));
    int num_adivinha = rand() % 15 + 1; // Gera número aleatório entre 1 e 15
    printf("O número gerado foi: %d\n", num_adivinha);

    int tentativas = 6; // Total de tentativas (3 para cada jogador)
    int turn = rand() % 2; // Decide aleatoriamente quem começa

    while(tentativas > 0) {

        int socket_atual = turn % 2 == 0 ? socket1 : socket2;
        int outro_socket = turn % 2 == 0 ? socket2 : socket1;
        char* buffer_atual = turn % 2 == 0 ? buffer1 : buffer2;

        // Indica ao cliente atual que é a sua vez de jogar
        mensagem = "Sua vez de jogar.";
        send(socket_atual , mensagem , strlen(mensagem) , 0 );

        // Indica ao outro cliente que não é a sua vez de jogar
        mensagem = "Aguarde a sua vez.";
        send(outro_socket , mensagem , strlen(mensagem) , 0 );

        read(socket_atual , buffer_atual, MAX_BUFFER);
        int guessed_number = atoi(buffer_atual);

        if(guessed_number == num_adivinha) {

            mensagem = "Parabéns! Você acertou o número!";
            send(socket_atual , mensagem , strlen(mensagem) , 0 );
            break;
        } 
        
        else if(guessed_number > num_adivinha) {
            mensagem = "O número que você precisa adivinhar é menor!";
        } 
        
        else {
            mensagem = "O número que você precisa adivinhar é maior!";
        }
         
         tentativas--;
         
        if(tentativas == 0) {

            mensagem = "Fim de jogo! Nenhum dos jogadores acertou o número.";
            send(socket1 , mensagem , strlen(mensagem) , 0 );
            send(socket2 , mensagem , strlen(mensagem) , 0 );
        } 
         
        else if(tentativas % 2 == 0) { // Apenas envia a mensagem quando ambos os jogadores tiverem respondido
            
            send(socket1 , mensagem , strlen(mensagem) , 0 );
            send(socket2 , mensagem , strlen(mensagem) , 0 );
             
            // Verifica se ambos os jogadores acertaram o número ao mesmo tempo
            if(atoi(buffer1) == num_adivinha && atoi(buffer2) == num_adivinha) {

                mensagem = "Empate! Ambos os jogadores acertaram o número ao mesmo tempo.";
                send(socket1 , mensagem , strlen(mensagem) , 0 );
                send(socket2 , mensagem , strlen(mensagem) , 0 );
                break;
            }
             
            memset(buffer1, 0, sizeof(buffer1));
            memset(buffer2, 0, sizeof(buffer2));
        }
         
        turn++;
    }

    return 0;
}
