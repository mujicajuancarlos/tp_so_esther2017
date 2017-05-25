/*
 * socket-message.h
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_SOCKET_MESSAGE_H_
#define SRC_DC_COMMONS_SOCKET_MESSAGE_H_

#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <commons/error.h>
#include "dc-commons/logger.h"

#include "package.h"

#define SEND_OR_RECEIVE_FAILURE -1
#define SEND_OR_RECEIVE_SUCCESS 0




/**
 * Funciones
 */


/**
 * @NAME: createAndSendPackage
 * @DESC: crea y envia por socket un package, si devuelve NULL significa que hubo error. IMPORTANTE:
 * 	No olvidar ejecutar destroyPackage despues de utilizar los datos del mismo
 * @PARAMS:
 *		fileDescriptor
 *		msgCode -> codigo de operacion
 *		size -> tamaño del stream
 *		stream -> puntero a una cadena de bytes, su tamaño esta determinado por size.
 * @RETURN: puntero a package si el envio fue OK, NULL si hubo algun error;
 */
Package* createAndSendPackage(int fileDescriptor, uint32_t msgCode,
		uint32_t size, char* stream);

/**
 * @NAME: createAndReceivePackage
 * @DESC: crea y recibe por socket un package, si devuelve NULL significa que hubo error. IMPORTANTE:
 * 	No olvidar ejecutar destroyPackage despues de utilizar los datos del mismo
 * @PARAMS:
 *		fileDescriptor
 * @RETURN: puntero a package si la recepcion fue OK, NULL si hubo algun error;
 */
Package* createAndReceivePackage(int fileDescriptor);


/**
 * @NAME: sendPackage
 * @DESC: Recive un file descriptor u un puntero a package,
 * serializa el contenido completo de package y lo envia al file descriptor.
 * NO SE ENCARGA DE LIBERAR MEMORIA DE PACKAGE, por lo tanto debe hacerse despues de llamar a esta funcion
 * @PARAMS:
 *		fileDescriptor
 *		puntero a package
 * @RETURN: 0 si el envio fue exitoso, o -1 si hubo algun error
 */
int sendPackage(int fileDescriptor, Package *package);

/**
 * @NAME: receivePackage
 * @DESC: Recive un file descriptor u un puntero a package,
 * leer del file descriptor, deserealiza el contenido completo y lo almacena en pakage
 * @PARAMS:
 *		fileDescriptor
 *		puntero a package
 * @RETURN: 0 si la recepcion fue exitosa, o -1 si hubo algun error
 */
int receivePackage(int fileDescriptor, Package *package);

/**
 * @NAME: sendMessage
 * @DESC: Escribe en un file descriptor, y garantiza que todos los bytes sean enviados
 * @PARAMS:
 *		fileDescriptor
 *		buffer
 *		sizeOfMessage
 *		int flags
 * @RETURN: la cantidad de bytes enviados, o -1 si hubo algun error
 */
int sendMessage(int socket, char *buffer, int sizeOfMessage, int flags);

/**
 * @NAME: receiveMessage
 * @DESC: Recibe datos de un file descriptor hasta alcanzar el tamaño definido, los almacena en el buffer
 * @PARAMS:
 *		fileDescriptor
 *		buffer
 *		sizeOfMessage
 *		flags
 * @RETURN: la cantidad de bytes recibidos, o -1 si hubo algun error
 */
int receiveMessage(int fileDescriptor, char *buffer, int sizeOfMessage, int flags);

#endif /* SRC_DC_COMMONS_SOCKET_MESSAGE_H_ */
