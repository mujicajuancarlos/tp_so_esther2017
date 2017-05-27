/*
 * protocol-memory-kernel.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_
#define SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "protocol-memory-core.h"
#include "protocol-errors.h"

//prefijo = KC ; between 400 - 499

#define COD_HANDSHAKE_KERNEL 400

//definicion de las operaciones de la memoria (pag 26)

/*
 * Inicializar programa
 *	Parámetros: [Identificador del Programa] [Páginas requeridas]
 */
#define COD_NEW_PROCESS_REQUEST 403
#define COD_NEW_PROCESS_RESPONSE_OK 404

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


typedef struct {
	uint32_t pid;
	uint32_t size;
} t_AddPagesToProcess;

size_t sizeof_t_AddPagesToProcess();
t_AddPagesToProcess* create_t_AddPagesToProcess(int pid, int size);

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_ */
