/*
 * handler-memory.c
 *
 *  Created on: 27/4/2017
 *      Author: utnso
 */

#include "memory.h"

void* handleMemory(Configuration *config, memory_struct args){

	/*Creo Server con las funciones de dc-commons*/
		logInfo("Esperando conexiones...");

		args.socketServer = crearSocketServer(args.config->puerto);
		if (args.socketServer == -1) {
			logError("No se pudo crear el server de memoria");
			exit(-1);
		}
		logInfo("Server Socket de memoria esta escuchando");

		while(args.socketServer !=- 1){

			int accepted = aceptarConexionCliente(args.socketServer);
			int recibido = atiendoCliente(accepted);

			if (recibido == 3)
			{
				puts("Se conecto una CPU");

				list_add(args.listaCPUs,accepted);
				pthread_t hiloCPU;
				pthread_create(&hiloCPU,NULL,(void*) comunicacionConCPU,accepted);
			}
			else
			{
				puts("Se conecto un Kernell");
				args.socketClientKernel = accepted;
				pthread_t hiloMemoria;
				pthread_create(&hiloMemoria,NULL,(void*)comunicacionConKernel,args.socketClientKernel);

			}
		}

}
