/*
 * protocol-memory-kernel.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_
#define SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_

//prefijo = KC ; between 400 - 499

#define COD_HANDSHAKE_KERNEL 400

#define COD_SAVE_NEW_PROGRAM 401 //para almacenar el cod ansisop
#define COD_SAVE_NEW_PROGRAM_OK 402 //respuesta ok para el cod COD_SAVE_NEW_PROGRAM

#define COD_MEMORY_FULL 409 //indica que no hay mas espacio

/**
 * todo seguir definiendo
 */

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_ */
