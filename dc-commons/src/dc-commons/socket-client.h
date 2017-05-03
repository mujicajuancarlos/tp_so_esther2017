/*
 * socket-client.h
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_SOCKET_CLIENT_H_
#define SRC_DC_COMMONS_SOCKET_CLIENT_H_

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
#include <sys/socket.h>
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
 * @DESC: Crea un socket AF_INET para conectarse con la ip y puerto pasados por parametro.
 * devuelve el file descriptor del socket
 * @PARAMS:
 * 		ip
 *		puerto
 * @RETURN: FD del socket creado o -1 si hubo un error en la creacion
 */
int crearSocketCliente(const char* ip, int port);

#endif /* SRC_DC_COMMONS_SOCKET_CLIENT_H_ */
