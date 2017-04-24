/*
 * packagesCPU.c
 *
 *  Created on: 20/4/2017
 *      Author: utnso
 */
#include <dc-commons/protocol-memory-cpu.h>
#include <dc-commons/package.h>
#include "packagesCPU.h"


void comunicacionConCPU(int fileDescriptor){

	int conexionInterrumpida = 1;

	while(conexionInterrumpida){

		conexionInterrumpida = packagesReceptionCPU(fileDescriptor); //Si retorna 0, sale del while

	}

}

int packagesReceptionCPU(int fd){

	Package *packageRecv;

		if(receivePackage(fd, packageRecv) != 0){ //Los sends packages van dentro de cada caso, armandose como se debe en cada uno
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
		return 1;

		}else {
			puts("Conexion interrumpida ... ");

			return 0;

		}
}

void packagesSenderCPU(unsigned long int hiloCPU, int expulsador){

}
