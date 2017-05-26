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
#include "serialization.h"
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
#define COD_NEW_PROCESS_RESPONSE_OK 404

/*
 * Solicitar bytes de una página
 *	Parámetros: [Identificador del Programa], [#página], [offset] y [tamaño]
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
#define COD_END_PROCESS_REQUEST 412
#define COD_END_PROCESS_RESPONSE 413

/**
 * STATUS CODES (INTERNOS)
 */

#define ERROR_MEMORY_FULL 450
#define ERROR_SEGMENTATION_FAULT 451


typedef struct {
	uint32_t pid;
	uint32_t size;
} t_AddPagesToProcess;

typedef struct {
	uint32_t pid;
	uint32_t pageNumber;
	uint32_t offset;
	uint32_t size;
	char* buffer;
} t_PageBytes;

size_t sizeof_t_AddPagesToProcess();
size_t sizeof_t_GetPageBytes();

t_AddPagesToProcess* create_t_AddPagesToProcess(int pid, int size);

char* serialize_t_PageBytes(t_PageBytes* object);
t_PageBytes* deserialize_t_PageBytes(char* stream);

size_t sizeof_t_PageBytes(t_PageBytes* object);

t_PageBytes* create_t_PageBytes(uint32_t size, char* buffer);
void destroy_t_PageBytes(t_PageBytes* object);

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_ */
