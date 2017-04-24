/*
 * handler-memory.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include <dc-commons/protocol-memory-cpu.h>
#include <dc-commons/package.h>
#include "handler-memory.h"


void saludoAMemory(int sck);

void packagesReceptionMemory(int fd){

	Package *packageRecv;

		if(receivePackage(fd, packageRecv) != 0){
		switch(packageRecv->msgCode)
		{
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
		case COD_SALUDO:
			//Hace lo que tiene que hacer
			break;
		}
		}
}

void packagesSenderMemory(int memoryfd, int code){

	if(code < 0){

		puts("La especificada, no es una operacion valida");

	}
	else {

		switch(code)
		{
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
				case COD_SALUDO:
					saludoAMemory(memoryfd);
				}
				}

}

void saludoAMemory(int sck){
	char *mensaje = "Te conectaste memory";
	int longMensaje = sizeof(mensaje);
	Package *paqueteParaEnviar = createPackage(COD_SALUDO, longMensaje, mensaje);
	int consultaEnvio = sendPackage(sck,paqueteParaEnviar);
	if(consultaEnvio != -1){
		puts("Se envio paquete corectamente");
		}
	else {
		puts("Error en el envio");
	}

}
