/*
 * package.h
 *
 *  Created on: 20/4/2016
 *      Author: utnso
 */

#ifndef SOCKETS_PACKAGE_H_
#define SOCKETS_PACKAGE_H_

#include <stdint.h>

#define MAX_PACKAGE_SIZE 1024	//El servidor no admitira paquetes de mas de 1024 bytes

/*
 * 	Es importante destacar que utilizamos el tipo uint_32, incluida en el header <stdint.h> para mantener un estandar en la cantidad
 * 	de bytes del paquete.
 */
typedef struct Package {
	uint32_t msgCode;
	uint32_t message_long;
	char* message;
} Package;

#endif /* SOCKETS_PACKAGE_H_ */
