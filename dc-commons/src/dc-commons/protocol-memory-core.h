/*
 * protocol-memory-core.h
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_MEMORY_CORE_H_
#define SRC_DC_COMMONS_PROTOCOL_MEMORY_CORE_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "serialization.h"

//; between 600 - 699 Representa los codigos de mensaje para protocolo lo memori
// tanto protocolo-memory-cpy y protocolo-memory-kernel extienden de este protocolo

/*
 * Solicitar el tamaño de pagina
 *	Parámetros: n/a
 */
#define COD_PAGE_SIZE_REQUEST 601 //para solicitar el tamaño de pagina
#define COD_PAGE_SIZE_RESPONSE 602 //para devolver el tmaño de pagina

/*
 * Solicitar bytes de una página
 *	Parámetros: [Identificador del Programa], [#página], [offset] y [tamaño]
 */
#define COD_GET_PAGE_BYTES_REQUEST 603
#define COD_GET_PAGE_BYTES_RESPONSE 604

/*
 * Almacenar bytes en una página
 *	Parámetros: [Identificador del Programa], [#página], [offset], [tamaño] y [buffer]
 */
#define COD_SAVE_PAGE_BYTES_REQUEST 605
#define COD_SAVE_PAGE_BYTES_RESPONSE 606

typedef struct {
	uint32_t pid;
	uint32_t pageNumber;
	uint32_t offset;
	uint32_t size;
	char* buffer;
} t_PageBytes;

char* serialize_t_PageBytes(t_PageBytes* object);
t_PageBytes* deserialize_t_PageBytes(char* stream);

size_t sizeof_t_PageBytes(t_PageBytes* object);

t_PageBytes* create_t_PageBytes(uint32_t pid, uint32_t pageNumber, uint32_t offset, uint32_t size, char* buffer);
void destroy_t_PageBytes(t_PageBytes* object);

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_CORE_H_ */
