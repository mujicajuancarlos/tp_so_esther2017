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

#include <commons/error.h>

#define SEND_OR_RECEIVE_FAILURE -1

/**
 * Funciones
 */

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
