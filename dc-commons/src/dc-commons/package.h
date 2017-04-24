/*
 * package.h
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PACKAGE_H_
#define SRC_DC_COMMONS_PACKAGE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * Estructuras
 */

typedef struct Package {
	uint32_t msgCode;
	uint32_t size;
	char* stream;
} Package;

/**
 * Functions
 */

/**
 * @NAME: serializePackage
 * @DESC: recibe un puntero a package y retorna un puntero a char con el contenido serializado de package
 * @PARAMS:
 * 		puntero a package
 * @RETURN:
 * 		puntero a char
 */
char* serializePackage(Package *package);

/**
 * @NAME: createPackage
 * @DESC: Crea un package con los datos recibidos por parametro, usa malloc para generar la estructura y el mensaje
 * @PARAMS:
 * 		msgCode
 *		size
 *		stream
 * @RETURN: un punteto a package
 */
Package* createPackage(uint32_t msgCode, uint32_t size, char* stream );

/**
 * @NAME: createEmptyPackage
 * @DESC: Crea un package con los datos en NULL
 * @RETURN: un punteto a package
 */
Package* createEmptyPackage();

/**
 * @NAME: sizePackage
 * @DESC: calcula el tamaño completo de la estructura
 * @PARAMS:
 * 		puntero a package
 * @RETURN: suma de los sizeof de los atributo de la estructura
 */
int sizePackage(Package *package);

/**
 * @NAME: sizePackage
 * @DESC: calcula el tamaño completo de la estructura
 * @PARAMS:
 * 		puntero a package
 * @RETURN: suma de los sizeof de los atributo de la estructura
 */

/**
 * @NAME: destroyPackage
 * @DESC: libera memoria de la estructura y el mensaje
 * @PARAMS:
 * 		puntero a package
 */
void destroyPackage(Package* package);

#endif /* SRC_DC_COMMONS_PACKAGE_H_ */
