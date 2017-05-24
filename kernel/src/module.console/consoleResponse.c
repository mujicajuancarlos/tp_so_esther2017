/*
 * consoleResponse.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "consoleResponse.h"


void consoleResponseRepulseMessage(Process* newProcess) {

	Package* package;
	bool running = true;
	while (running) {

		package = createAndReceivePackage(newProcess->fileDescriptor);

		switch (package->msgCode)
		{
			case COD_KC_CANT_RUN_PROGRAM_REQUEST:
				logInfo("Solicitud rechazada, No se pudó iniciar el programa");

				/*printMessage("Solicitud rechazada, No se pudó iniciar el programa",
						message);
				*/
				running=false;
			break;
		}

		destroyPackage(package);
	}

	destroyProcess(newProcess);
	close(newProcess->fileDescriptor);
}
