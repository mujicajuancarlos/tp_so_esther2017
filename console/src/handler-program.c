/*
 * handler-program.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "handler-program.h"

int handleProgram(console_struct* args) {

	Program program;

	program.fd_client = crearSocketCliente(args->config->ip_kernel,
			args->config->puerto_kernel);

	if (program.fd_client == -1) {
		logError("No se pudo establecer conexion con el kernel");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
