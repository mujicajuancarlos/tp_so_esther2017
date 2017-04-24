/*
 * handlerFileSystem.c
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#include "handler-fileSystem.h"
#include <dc-commons/protocol-fileSystem-kernel.h>

void handleFileSystem(kernel_struct *args) {
	int fileDescriptor;
	logInfo("Creando socket para conectarse al fileSystem");

	fileDescriptor = crearSocketCliente(args->config->ip_fs,
			args->config->puerto_fs);

	if (fileDescriptor == -1)
		logError("No se pudo establecer la conexion con el fileSystem");
	else {
		char* message = "Hola soy el kernel";
		int len = strlen(message);
		if (send(fileDescriptor, message, len, 0) != -1) {
			puts("Se mando mensaje al fileSystem corectamente");

	Package* package = createEmptyPackage();
		int consultaReciv = receivePackage(fileDescriptor,package);
			if(consultaReciv != -1){
				printf("Recibido correctamente el mensaje del paquete es %s\n",package->stream);
				}
				else{
					puts("No se pudo recibir");
				}
			};
	}
}

void packagesSenderKernel(kernel_struct *args, int code) {
	/*Cuando se le pide al proceso que haga alguna de las operaciones,
	se le puede mandar esta función con el código setteado (Se le podría cambiar
	el nombre a esta función)*/
	if(code < 0){
		puts("La especificada, no es una operacion valida");
	} else {
	 switch(code)	{
		case COD_CREAR_ARCHIVO:
			//	Enviar un mensaje de que fue satisfactorio
			break;
		case COD_BORRAR_ARCHIVO:
			//	Verificar que existe ese archivo del path, borrarlo y mencionar el éxito del proceso
			break;
		case COD_OBTENER_DATOS:
			//	Si pide datos el Kernel, y el path está en modo lectura, se devolverá la cant de bytes definidos por el size en base al offset
			break;
		case COD_GUARDAR_DATOS:
			//	En caso de guardar y que no haya espacio
			break;
		case COD_SALUDO:

			break;
		}
	}
  }

void handleKernelRequest(kernel_struct*args, Package *package) {
	switch (package->msgCode) {
			case COD_VALIDAR_ARCHIVO:
				//	Verifica que el archivo del path exista
				break;
			case COD_CREAR_ARCHIVO:
				//	En caso de que no exista (y en el path se pueda escribir) se va a crear el archivo dentro de ese path
				break;
			case COD_BORRAR_ARCHIVO:
				//	En caso de que exista, borrará el archivo metadata y liberará bloques del bitmap
				break;
			case COD_OBTENER_DATOS:
				//	Si pide datos el Kernel, y el path está en modo lectura, se devolverá la cant de bytes definidos por el size en base al offset
				break;
			case COD_GUARDAR_DATOS:
				//	Si se encuentra en modo escritura, se almacenará en el path los bytes del buffer definidos por el size
				break;
			case COD_SALUDO:
				printf("Recibido Corectamente el mensaje del paquete es %s\n",
						package->stream);
			break;
		default:
			logError("El kernel solicito una accion no permitida");
			break;
		}
	destroyPackage(package);
}

