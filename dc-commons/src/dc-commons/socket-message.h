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

#include <commons/error.h>
#include <dc-commons/package.h>

#define SEND_OR_RECEIVE_FAILURE -1
#define SEND_OR_RECEIVE_SUCCESS 0

/**
 * Funciones
 */

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
 *		message
 *		sizeOfMessage
 * @RETURN: la cantidad de bytes enviados, o -1 si hubo algun error
 */
int sendMessage(int fileDescriptor, char *message, int sizeOfMessage);

/**
 * @NAME: receiveMessage
 * @DESC: Recibe datos de un file descriptor hasta alcanzar el tamaño definido, los almacena en el buffer
 * @PARAMS:
 *		fileDescriptor
 *		message
 *		sizeOfMessage
 * @RETURN: la cantidad de bytes recibidos, o -1 si hubo algun error
 */
int receiveMessage(int fileDescriptor, char *message, int sizeOfMessage);

#endif /* SRC_DC_COMMONS_SOCKET_MESSAGE_H_ */
