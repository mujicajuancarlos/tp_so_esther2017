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
#include <stdint.h>
#include <string.h>
#include <parser/metadata_program.h>
#include <commons/string.h>

/**
 * @NAME: serialize_data
 * @DESC: serializador generico para tipos basicos (no structs y tampoco char*)
 * @PARAMS:
 * 		char* buffer de donde se deposita el valor
 * 		void* value puntero a tipo de dato a serializar (los bytes del value se copian al stream)
 * 		uint32_t size_to_copy tamaño en bytes del valor a recuperar del stream (sizeOf(tu_tipo))
 * 		uint32_t* offset puntero a uint32_t que indica el desplazamiento en base a la posicion 0 del buffer
 */
void serialize_and_copy_value(char* buffer, void* value, size_t size_to_copy, uint32_t* offset);

/**
 * @NAME: deserialize_data
 * @DESC: deserializador generico para tipos basicos (no structs y tampoco char*)
 * @PARAMS:
 * 		void* value puntero a tipo de dato a deserializar (aca se va almacenar el value)
 * 		char* buffer de donde se obtiene el valor
 * 		uint32_t size_to_copy tamaño en bytes del valor a copiar (sizeOf(tu_tipo))
 * 		uint32_t* offset puntero a uint32_t que indica el desplazamiento en base a la posicion 0 del buffer
 *
 */
void deserialize_and_copy_value(void* value, char* buffer, size_t size_to_copy, uint32_t* offset);

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

/**
 * @NAME: deserialize_string
 * @DESC: recibe un stream y el tamaño que deseas extraer y devuelve un string
 * @PARAMS:
 * 		char* stream
 * 		size_t tamañano del string
 * @RETURN:
 * 		string, NO OLVIDAR HACER FREE DEL string
 */
char* deserialize_string(char* stream, size_t length);

/**
 * @NAME: sizeOf_metadata_program
 * @DESC: devuelvo el tamaño de toda la estructura
 * @PARAMS:
 * 		t_metadata_program* metadata
 * @RETURN:
 * 		uint32_t
 */
uint32_t sizeOf_metadata_program(t_metadata_program* metadata);

/**
 * @NAME: serialize_metadata_program
 * @DESC: devuelvo un stream que representa la metadata
 * @PARAMS:
 * 		t_metadata_program* metadata
 * @RETURN:
 * 		char* stream
 */
char* serialize_metadata_program(t_metadata_program* metadata);

/**
 * @NAME: deserialize_metadata_program
 * @DESC: devuelvo una estructura a partir del stream
 * @PARAMS:
 * 		char* stream
 * @RETURN:
 * 		t_metadata_program* metadata
 */
t_metadata_program* deserialize_metadata_program(char* stream);

#endif /* SRC_DC_COMMONS_SERIALIZATION_H_ */
