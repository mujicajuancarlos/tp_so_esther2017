/*
 ============================================================================
 Name        : socket.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "socket.h"
	
void crearConexiones(Configuration *config){
		int listenerfd;
		char *msg = "Hola conexión nueva";	
	//creo el socket escucha
		listenerfd = socket(AF_INET, SOCK_STREAM, 0);
		if (listenerfd == -1){
			puts("Error en la creación del socket");//
		}
	//Asocio el socket escucha a su puerto
		bind_port(listenerfd,config->puerto);
	//Escucho clientes
		if(listen(listenerfd,100) == -1){
			puts("No se puede escuchar en ese puerto (Intente en otro) \n");
			}
			puts("Se enlazó al puerto");
		while(1){
			struct sockaddr_in cliente;
			int *addrlen = sizeof(cliente);
			char *buffer = malloc(20);
				puts("Esperando al cliente...");
				int accepted = accept(listenerfd, (struct sockaddr *)&cliente, &addrlen);
				if(accepted == -1){
					puts("No se puede conectar con el socket aceptado");
				}
					puts("Atendiendo al cliente...");
		int bytes_recv = recv(accepted, buffer, 20, 0);
				if (bytes_recv < 0){
					puts("Error al recibir ");
				}
					buffer[bytes_recv]='\0';
					printf("Me llegaron %d bytes con %s", bytes_recv, buffer);
					printf("%s\n",msg);
					puts("Te escuché");
					free(buffer);
					close(accepted);
				}
					return ;
				}
void bind_port(int sockfd, int puerto){
			struct sockaddr_in my_addr;
			my_addr.sin_family = AF_INET;
			my_addr.sin_port = htons(puerto);
			my_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY usa mi dirección IP
			memset(&(my_addr.sin_zero), '\0', 8);
			int reuse = 1;
			if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1){
				puts("No es posible reutilizar el socket \n");
			}
			int binder = bind(sockfd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr));
			if (binder == -1){
				puts("Error al asociar el proceso a ese puerto"); //
			}
		}