/*
  * handler-memory.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include "handler-memory.h"

void saludoAMemory(int sck);

void packagesReceptionMemory(int fd) {

	Package *packageRecv = createAndReceivePackage(fd);

	/*if (packageRecv != NULL) {
		switch (packageRecv->msgCode) {
		case COD_SOLICITAR_BYTES_PAGE:
			//Solicitar bytes de una página... solicitarBytes(&hiloCPU) (dentro de esta funcion se hace lo que tiene resolver y arma un paquete con la respuesta
			break;
		case COD_ESCRITURA_PAGE:
			//Almacenar bytes en una página
			break;
		case COD_INFO_PROGRAMA_EXE:
			//Hace lo que tiene que hacer
			break;
		case COD_ACTUALIZAR_ESTRUCTURA:
			//Hace lo que tiene que hacer
			break;
		case COD_PROXIMA_SENTENCIA_A_EJECUTAR:
			break;
		}
	}*/
}

void packagesSenderMemory(int memoryfd, int code) {

	/*if (code < 0) {

		puts("La especificada, no es una operacion valida");

	} else {

		switch (code) {
		case COD_SOLICITAR_BYTES_PAGE:
			//Solicitar bytes de una página... solicitarBytes(&hiloCPU) (dentro de esta funcion se hace lo que tiene resolver y arma un paquete con la respuesta
			break;
		case COD_ESCRITURA_PAGE:
			//Almacenar bytes en una página
			break;
		case COD_INFO_PROGRAMA_EXE:
			//Hace lo que tiene que hacer
			break;
		case COD_ACTUALIZAR_ESTRUCTURA:
			//Hace lo que tiene que hacer
			break;
		case COD_PROXIMA_SENTENCIA_A_EJECUTAR:
			break;
		}
	}*/

}



void mensajeParaConectarseAMemory(int sck_memory) {
	char* message = "CPU";
	int len = strlen(message);
	if (send(sck_memory, message, len, 0) != -1) {
		puts("Se mando mensaje a memoria corectamente");
	};
}
