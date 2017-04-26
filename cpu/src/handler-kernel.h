/*
 * handler-kernel.h
 *
 *  Created on: 15/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_KERNEL_H_
#define HANDLER_KERNEL_H_

#include <stddef.h>
#include <errno.h>

#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>

#include "cpu.h"

/**
 * Maneja la conexion princial del KERNEL
 */
void handleKernel(cpu_struct *args);

/**
 * Manejo los pedidos del KERNEL
 */
void packagesReceptionKernel(cpu_struct *args, Package *package);
void packagesSenderKernel(int memoryfd, int code);

#endif /* HANDLER_KERNEL_H_ */
