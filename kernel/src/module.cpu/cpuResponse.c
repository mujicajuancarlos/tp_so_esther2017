/*
 * cpuResponse.c
 *
 *  Created on: 31/5/2017
 *      Author: utnso
 */

#include "cpuResponse.h"

/*
 * me encargo de notificar a la cpu el estado de ejecucion del la syscall semaforos
 */
void notifyUpdateSemaphoreStatus(CPU* cpu,bool hasError, bool shouldLock){
	Package* package;
	char* buffer;
	if(!hasError){
		buffer = serialize_bool(shouldLock);
		package = createAndSendPackage(cpu->fileDescriptor,COD_SYSCALL_SUCCESS,sizeof(bool),buffer);
		free(buffer);
	}else{
		package = createAndSendPackage(cpu->fileDescriptor,COD_SYSCALL_FAILURE,0,NULL);
	}
	destroyPackage(package);
}
