#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main() {

    int sock = 0;
    struct sockaddr_in serv_addr;
    
    char buffer[1024] = {0};
    
    // Criação do socket do cliente
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Falha na criação do socket \n");
        return -1;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convertendo endereço IPv4 e IPv6 para binário e armazenando no struct sockaddr_in
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nEndereço inválido ou não suportado \n");
        return -1;
    }
  
  	// Conectando ao servidor
  	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    	printf("\nConexão falhou \n");
    	return -1;
  	}

  	// Lógica do jogo
  	int attempts = 3; // Cada jogador tem três tentativas
  	
  	while(attempts > 0) {
        read(sock , buffer, 1024);
    
        if(strcmp(buffer, "Sua vez de jogar.") == 0) {

            printf("Digite um número entre 1 e 15: ");
            char number[3];
            fgets(number, 3, stdin);
            send(sock , number , strlen(number) , 0 );
            attempts--;
    } 
    
        else {
            printf("Aguarde a sua vez.\n");
        }
    	
    	if(strcmp(buffer, "Parabéns! Você acertou o número!") == 0) {
      		break;
    	}
    	
    	attempts--;
    	
    	if(attempts == 0) {
      		printf("Fim de jogo! Você não acertou o número.\n");
    	}
    	
    	memset(buffer, 0, sizeof(buffer));
  	}

  	return 0;
}
