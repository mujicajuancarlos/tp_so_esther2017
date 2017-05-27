/*
 * protocol-memory-cpu.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_MEMORY_CPU_H_
#define SRC_DC_COMMONS_PROTOCOL_MEMORY_CPU_H_

//prefijo = KC ; between 300 - 399

#define COD_HANDSHAKE_CPU 300

/*
 * Solicitar el tamaño de pagina
 *	Parámetros: n/a
 */
#define COD_PAGE_SIZE_REQUEST 301 //para solicitar el tamaño de pagina
#define COD_PAGE_SIZE_RESPONSE 302 //para devolver el tmaño de pagina

/*
 * Solicitar bytes de una página
 *	Parámetros: [Identificador del Programa], [#página], [offset] y [tamaño]
 */
#define COD_GET_PAGE_BYTES_REQUEST 303
#define COD_GET_PAGE_BYTES_RESPONSE 304

/*
 * Almacenar bytes en una página
 *	Parámetros: [Identificador del Programa], [#página], [offset], [tamaño] y [buffer]
 */
#define COD_SAVE_PAGE_BYTES_REQUEST 305
#define COD_SAVE_PAGE_BYTES_RESPONSE 304


#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_CPU_H_ */
