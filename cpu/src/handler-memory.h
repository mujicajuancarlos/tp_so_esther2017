/*
 * handler-memory.h
 *
 *  Created on: 24/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_MEMORY_H_
#define HANDLER_MEMORY_H_

#include "cpu.h"


//Recibe y desempaqueta los packages provenientes de la Memory
void packagesReceptionMemory(int fd);

//Empaqueta y envia packages a la Memory
void packagesSenderMemory(int memoryfd, int code);
void mensajeParaConectarseAMemory(int memory);


#endif /* HANDLER_MEMORY_H_ */
