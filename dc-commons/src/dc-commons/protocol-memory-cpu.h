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

#define COD_SOLICITAR_BYTES_PAGE 301 // Solicitar bytes de una página
#define COD_ESCRITURA_PAGE 302 // Almacenar bytes en una página
#define COD_INFO_PROGRAMA_EXE 303 //?
#define COD_ACTUALIZAR_ESTRUCTURA 304 //?
#define COD_PROXIMA_SENTENCIA_A_EJECUTAR 305

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_CPU_H_ */
