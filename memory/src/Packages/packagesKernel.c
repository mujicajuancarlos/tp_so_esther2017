/*
 * packagesKernel.c
 *
 *  Created on: 20/4/2017
 *      Author: utnso
 */

#include "packagesKernel.h"
#include <dc-commons/protocol-memory-kernel.h>

void exceptionTo(int destinationfd, char *exception);

void comunicacionConKernel(int socketKernelfd){

	packagesSenderKernel(socketKernelfd,COD_SALUDO);
}



void packagesReceptionKernel(int kernelfd){

	Package* packageRecv = createEmptyPackage();

	if(receivePackage(kernelfd, packageRecv) != 0){
	switch(packageRecv->msgCode)
	{
	case COD_INICIALIZAR_PROGRAMA:

		break;
	case COD_ESCRITURA_PAGE:
		//Hace lo que tiene que hacer
		break;
	case COD_ASIGN_PAGE_PROCESS:
		//Hace lo que tiene que hacer
		break;
	case COD_FINALIZAR_PROGRAMA:
		logDebug("El Kernel informa que finalizó el programa");
		break;
	case COD_SALUDO:
		logInfo("El kernel %d me envio el siguiente saludo: %s", kernelfd,
				package->message);
		break;
	}
	destroyPackage(package);
}
}

void packagesSenderKernel(int kernelfd, int code){ //Cuando se le pide al proceso que haga alguna de las operaciones, se le puede mandar esta función con el código setteado (Se le podría cambiar el nombre a esta función)

	if(code < 0){

		puts("La especificada, no es una operacion valida");

	}
	else {
		//VER MÁS ABAJO
		switch(code)
		{
			case COD_ESCRITURA_PAGE:
				//En realidad, no tiene que devolver nada, pero podemos hacer un send a Kernel o CPU diciendo que fue exitoso
				break;
			case COD_ASIGN_PAGE_PROCESS:
				//Informar la imposibilidad de asignar páginas por falta de espacio

				exceptionTo(kernelfd,"No es posible asignar paginas por falta de espacio");
				break;
			case COD_SALUDO:
					saludoAKernel(kernelfd);
				break;

		}

	}

}

void saludoAKernel(int sck){
	char *mensaje = "Te conectaste kernel";
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

void exceptionTo(int destinationfd, char *exception){

	int len, bytes_sent;
	len = strlen(exception);

	bytes_sent = sendmsg(destinationfd,exception,len);

	if (bytes_sent != -1){

		puts("Eror en mandar exception");

	}
	else
	{
		puts("Se mando corectamente mensaje de exception");
	}

}

/*
typedef struct Package {
	uint32_t msgCode;
	uint32_t message_long;
	ker_mem_type* message;
} Package;

typedef struct k_m_type{
	uint32_t programID;
	uint32_t paginas_requeridas;
	uint32_t page_number;
	uint32_t offset;
	uint32_t tamanio;
	char* buffer;
};
*/
