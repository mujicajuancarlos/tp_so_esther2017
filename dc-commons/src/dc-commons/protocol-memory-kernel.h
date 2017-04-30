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

#define COD_INICIALIZAR_PROGRAMA 401
#define COD_ESCRITURA_PAGE 402 // Almacenar bytes en una página
#define COD_ASIGN_PAGE_PROCESS 403 // Asignar páginas a proceso
#define COD_FINALIZAR_PROGRAMA 404

#endif /* SRC_DC_COMMONS_PROTOCOL_MEMORY_KERNEL_H_ */
