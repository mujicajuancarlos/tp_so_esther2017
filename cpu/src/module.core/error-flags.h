/*
 * error-flags.h
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_ERROR_FLAGS_H_
#define MODULE_CORE_ERROR_FLAGS_H_

#define FLAG_OK 0
#define FLAG_STACKOVERFLOW 1
#define FLAG_SEGMENTATION_FAULT 2
#define FLAG_DISCONNECTED_MEMORY 3
#define FLAG_DISCONNECTED_KERNEL 4
#define FLAG_UNKNOWN_ERROR 5
#define FLAG_SINTAX_ERROR 6
#define FLAG_UNKNOWN_MESSAGE_CODE 7

/*
 * este flag no necesita informar al kernel porque es un syscall
 * el kernel ya realizo la accion correspondiente
 * lo que hace la cpu es destruir el contexto actual
 */
#define FLAG_SYSCALL_FAILURE 8
/*
 * este fag no necesita indicar al kernel
 * solos destruir su contexto para recibir nuevos procesos
 */
#define FLAG_PROCESS_BLOCK 9

#define FLAG_END_CPU 19
#define FLAG_END_PROGRAM 20

#endif /* MODULE_CORE_ERROR_FLAGS_H_ */
