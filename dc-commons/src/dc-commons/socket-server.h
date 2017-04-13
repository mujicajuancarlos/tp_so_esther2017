/*
 * socket-server.h
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_SOCKET_SERVER_H_
#define SRC_DC_COMMONS_SOCKET_SERVER_H_

/**
 * locales
 */
#include "package.h"
/**
 * commons
 */
#include <commons/error.h>
/**
 *
 */
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * definitions
 */
#define	SOCKET_FAILURE	-1	/* Failing exit status.  */

/**
 * Funciones
 */

/**
 * @NAME: crearSocketServer
 * @DESC: Crea un socket y devuelve el file descriptor
 * @PARAMS:
 * 		ip
 *		puerto
 */
int crearSocketServer(const char* ip, int port);

/**
 * @NAME: aceptarConexionCliente
 * @DESC: Crea un socket para atender la nueva conexion que llego al socket server
 * @PARAMS:
 * 		socketServerFileDescriptor
 */
int aceptarConexionCliente (int socketServerFileDescriptor);

#endif /* SRC_DC_COMMONS_SOCKET_SERVER_H_ */
