/*
 * protocol-memory-kernel.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_
#define SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
//prefijo = KC ; between 400 - 499

#define COD_HANDSHAKE_KERNEL 400

#define COD_SAVE_NEW_PROGRAM 401 //para almacenar el cod ansisop
#define COD_SAVE_NEW_PROGRAM_OK 402 //respuesta ok para el cod COD_SAVE_NEW_PROGRAM
#define COD_PAGE_SIZE_REQUEST 403 //para solicitar el tamaño de pagina
#define COD_PAGE_SIZE_RESPONSE 404 //para devolver el tmaño de pagina

#define COD_MEMORY_FULL 409 //indica que no hay mas espacio

/**
 * todo seguir definiendo
 */


/**
 * Estructuras y funciones recervar paginas para un nuevo proceso
 */
typedef struct {
	uint32_t pid;
	uint32_t stackSize;
	uint32_t sourceCodeSize;
	char* sourceCode;
} t_new_sourceCode_request;
t_new_sourceCode_request* create_new_sourceCode_request(uint32_t pid, uint32_t stackSize, uint32_t sourceCodeSize, char* sourceCode);
void destroy_new_sourceCode_request(t_new_sourceCode_request* request);
size_t size_new_sourceCode_request(t_new_sourceCode_request* request);
char* serialize_new_sourceCode_request(t_new_sourceCode_request* request);
t_new_sourceCode_request* deserialize_new_sourceCode_request(char* stream);
/**
 * FIN
 */

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_ */
