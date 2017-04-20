/*
 * packagesKernel.c
 *
 *  Created on: 20/4/2017
 *      Author: utnso
 */

#include "packagesKernel.h"

void comunicacionConKernel(int socketKernelfd){

	int init = 1;
	int exit = 1;

	while (init){
		packagesReceptionKernel(socketKernelfd, init);
	}
	while (exit){
		packagesSenderKernel(socketKernelfd, exit);
	}
}

void packagesReceptionKernel(int kernelfd, int continuador){

	Package *packageRecv;

	if(receivePackage(kernelfd, packageRecv) != 0){
	switch(packageRecv->msgCode)
	{
	case COD_ESCRITURA_PAGE:
		//Hace lo que tiene que hacer
		break;
	case COD_ASIGN_PAGE_PROCESS:
		//Hace lo que tiene que hacer
		break;
	case COD_FINALIZAR_PROGRAMA:
		continuador = 0;
		logDebug("El Kernel informa que finalizó el programa");
		break;
	case COD_SALUDO:
		//Hace lo que tenga que hacer

	}
	destroyPackage(packageRecv);
}
}

void packagesSenderKernel(int kernelfd, int exit){

}

