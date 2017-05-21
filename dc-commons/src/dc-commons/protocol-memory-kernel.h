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

/*
 * Solicitar el tamaño de pagina
 *	Parámetros: n/a
 */
#define COD_PAGE_SIZE_REQUEST 401 //para solicitar el tamaño de pagina
#define COD_PAGE_SIZE_RESPONSE 402 //para devolver el tmaño de pagina


//definicion de las operaciones de la memoria (pag 26)

/*
 * Inicializar programa
 *	Parámetros: [Identificador del Programa] [Páginas requeridas]
 */
#define COD_NEW_PROCESS_REQUEST 403
#define COD_NEW_PROCESS_RESPONSE 404

/*
 * Solicitar bytes de una página
 *	Parámetros: [Identificador del Programa] [Páginas requeridas]
 */
#define COD_GET_PAGE_BYTES_REQUEST 405
#define COD_GET_PAGE_BYTES_RESPONSE 406

/*
 * Almacenar bytes en una página
 *	Parámetros: [Identificador del Programa], [#página], [offset], [tamaño] y [buffer]
 */
#define COD_SAVE_PAGE_BYTES_REQUEST 408
#define COD_SAVE_PAGE_BYTES_RESPONSE 409

/*
 * Asignar Páginas a Proceso
 *	Parámetros: [Identificador del Programa] [Páginas requeridas]
 */
#define COD_ADD_PROCESS_PAGES_REQUEST 410
#define COD_ADD_PROCESS_PAGES_RESPONSE 411

/*
 * Finalizar programa
 *	Parámetros: [Identificador del Programa]
 */
#define COD_END_PROCESS_REQUEST 410
#define COD_END_PROCESS_RESPONSE 411


/*
 * codigo de error para indicar que no hay mas memoria todo mover a un archivo global de errores
 */
#define COD_MEMORY_FULL 444 //indica que no hay mas espacio

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
t_new_sourceCode_request* create_new_sourceCode_request(uint32_t pid,
		uint32_t stackSize, uint32_t sourceCodeSize, char* sourceCode);
void destroy_new_sourceCode_request(t_new_sourceCode_request* request);
size_t size_new_sourceCode_request(t_new_sourceCode_request* request);
char* serialize_new_sourceCode_request(t_new_sourceCode_request* request);
t_new_sourceCode_request* deserialize_new_sourceCode_request(char* stream);
/**
 * FIN
 */

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_ */
