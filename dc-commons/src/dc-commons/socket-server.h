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
#define	LOCAL_HOST	"127.0.0.1"
/**
 * Funciones
 */

/**
 * @NAME: crearSocketServer
 * @DESC: Crea un socket para localhost y devuelve el file descriptor
 * @PARAMS:
 *		puerto
 * @RETURN: FD del socket creado o -1 si hubo un error en la creacion
 */
int crearSocketServer(int port);

/**
 * @NAME: aceptarConexionCliente
 * @DESC: Crea un socket para atender la nueva conexion que llego al socket server
 * 			Esta funcion sirve para sockets AF_INET o AF_UNIX.
 * @PARAMS:
 * 		socketServerFileDescriptor
 * @RETURN: FD del nuevo socket o -1 si hubo un error
 */
int aceptarConexionCliente(int socketServerFileDescriptor);

#endif /* SRC_DC_COMMONS_SOCKET_SERVER_H_ */
