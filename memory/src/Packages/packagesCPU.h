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
void comunicacionConCPU(int fileDescriptor);

//Recibe y desempaqueta los packages provenientes de una CPU
int packagesReceptionCPU(int fd);

//Empaqueta y envia packages al CPU
void packagesSenderCPU(int cpufd);


#endif /* PACKAGES_PACKAGESCPU_H_ */
