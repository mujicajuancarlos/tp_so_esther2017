#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define PORT 3550
#define BACKLOG 2 //Numero maximo de conexiones

int main(void){
	int newSocket;
	int newSocket2;
	struct sockaddr_in cliente;
	struct sockaddr_in servidor;
	int sin_size;
	//struct sockaddr;

	servidor.sin_family = AF_INET();
	servidor.sin_port = htons(PORT);
	servidor.sin_addr = INADDR_ANY();
	bzero(&(cliente.sin_zero),8);
//CREACION DE SOCKET CON COMPROBACION DE ERROR---------------------------------------------------------------------------------
	if((newSocket = socket(AF_INET(),SOCK_STREAM(),0)) == -1){
		printf("Error en Socket()");
		exit(-1);
	}

	//ASOCIACION DE PUERTO CON COMPROBACION DE ERROR---------------------------------------------------------------------------------
	if(bind(newSocket,(struct sockaddr *)&servidor,sizeof(struct sockaddr)) == -1) {
		printf("Error en bind()");
		exit(-1);
	}
	//LLAMADA A LISTEN CON COMPROBACION DE ERROR---------------------------------------------------------------------------------
	if(listen(newSocket,BACKLOG) == -1){
		printf("Error de listen()");
		exit(-1);
	}
//ACEPTA Y DA UN NUEVO SOCKET PARA CONECTAR CON CLIENTE----------------------------------------------------
	while(1) {
		sin_size = sizeof(struct sockaddr_in);
		if((newSocket2 = accept(newSocket,(struct sockaddr *)&cliente,&sin_size)) == -1){
			printf("Error en accept()");
			exit(-1);
	}

	printf("Se obtuvo una conezion desde %s",inet_ntoa(cliente.sin_addr));
	send(newSocket2,"Bienvenido al servidor.",22,0);//Jugar con variables para el mensaje y la longitud de no ser msj fijo
	close(newSocket2);
}
}
