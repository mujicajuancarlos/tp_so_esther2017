/*
 * protocol-kernel-console.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_KERNEL_CONSOLE_H_
#define SRC_DC_COMMONS_PROTOCOL_KERNEL_CONSOLE_H_

//prefijo = KC ; between 100 - 199

#define COD_KC_RUN_PROGRAM_REQUEST 101 //para solicitar el inicio de un nuevo ansisop
#define COD_KC_RUN_PROGRAM_RESPONSE 102 //para informar que el programa inicio bien
#define COD_KC_STOP_PROGRAM_REQUEST 103 //para solicitar la finalizacion de un proceso
#define COD_KC_STOP_PROGRAM_RESPONSE 104 //para responder si la finalizacion se realizo
#define COD_KC_PRINT_STDOUT 105 //para comunicar a la consola que se escribio en el stdout
#define COD_KC_END_PROGRAM 106 //para comunicar a la consola que un programa termino su ejecion
#define COD_KC_CANT_RUN_PROGRAM_RESPONSE 107 //para informar a la consola que el programa no pudo iniciar


#endif /* SRC_DC_COMMONS_PROTOCOL_KERNEL_CONSOLE_H_ */
