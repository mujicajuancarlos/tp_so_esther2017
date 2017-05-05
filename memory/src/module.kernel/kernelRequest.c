/*
 * handler-kernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "kernelRequest.h"

/**
 * esta funcion es invocada cuando se solicita generar espacio para un nuevo programa
 */
void startNewProcess(Package* package){
	t_new_sourceCode_request* request = deserialize_new_sourceCode_request(package->stream);

	request->pid;
	request->stackSize;
	request->sourceCodeSize;
	request->sourceCode;




	/**
	 *
	 *
	 *
	 * reservo paginas para este proceso y respondo al kernel
	 * 		- si pude reservar lo solicitado entonces le indico al kernel cuales fueron las paginas reservadas (lista o array de paginas)
	 * 		- si no pude reservar devuelvo lista vacia o mejor un error x ej: COD_MEMORY_FULL
	 *
	 *
	 *
	 */

	destroy_new_sourceCode_request(request);
}
