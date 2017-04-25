/*
 * serialization.h
 *
 *  Created on: 24/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_SERIALIZATION_H_
#define SRC_DC_COMMONS_SERIALIZATION_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * @NAME: serialize_int
 * @DESC: recibe un int y devuelve un stream que lo representa
 * @PARAMS:
 * 		int value
 * @RETURN:
 * 		stream, NO OLVIDAR HACER FREE DEL STREAM
 */
char *serialize_int(int value);

/**
 * @NAME: stream
 * @DESC: recive un stream y retorna un int
 * @PARAMS:
 * 		stream
 * @RETURN:
 * 		int, NO HAGO FREE DEL STREAM, POR LO CUAL DEBES HACERLO VOS AFUERA DE ESTA FUNCION
 */
int deserialize_int(char* stream);

#endif /* SRC_DC_COMMONS_SERIALIZATION_H_ */
