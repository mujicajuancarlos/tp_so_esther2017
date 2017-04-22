/*
 * packagesKernel.h
 *
 *  Created on: 20/4/2017
 *      Author: utnso
 */

#ifndef PACKAGES_PACKAGESKERNEL_H_
#define PACKAGES_PACKAGESKERNEL_H_

#include "memory.h"
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>

// Memory se pone a recibir y enviar packages con el Kernel
void comunicacionConKernel(int socketKernelfd);


//Recibe y desempaqueta los packages provenientes del Kernel
void packagesReceptionKernel(int kernelfd);

//Empaqueta y envia packages al Kernel
void packagesSenderKernel(int kernelfd,int code);

void saludoAKernel(int sck);


#endif /* PACKAGES_PACKAGESKERNEL_H_ */
