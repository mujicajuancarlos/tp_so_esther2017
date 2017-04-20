/*
 * packagesCPU.h
 *
 *  Created on: 20/4/2017
 *      Author: utnso
 */

#ifndef PACKAGES_PACKAGESCPU_H_
#define PACKAGES_PACKAGESCPU_H_

#include "memory.h"

// Memory se pone a recibir y enviar packages con un CPU
void comunicacionConCPU(unsigned long int hiloCPU);

//Recibe y desempaqueta los packages provenientes de una CPU
void packagesReceptionCPU(unsigned long int hiloCPU, int continuador);

//Empaqueta y envia packages al CPU
void packagesSenderCPU(unsigned long int hiloCPU, int expulsador);


#endif /* PACKAGES_PACKAGESCPU_H_ */
