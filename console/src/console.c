/*
 ============================================================================
 Name        : console.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "console.h"

int kernelClientFileDescriptor;

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	puts("Configuración del proceso:");
	printf("IP_KERNEL= %s\nPUERTO_KERNEL = %d\n\n", config->ip_kernel,
			config->puerto_kernel);

	puts("Creando el logger console.log ...");
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso consola");

	logInfo("Creando Conexiones");
	kernelClientFileDescriptor = crearSocketCliente(config->ip_kernel,
			config->puerto_kernel);

	if (kernelClientFileDescriptor == -1) {
		logError("No se pudo establecer conexion con el kernel");
		return EXIT_FAILURE;
	}

	//forma de implementar el envio de informacion entre procesos
	int status;
	uint32_t size_message = 200;
	uint32_t code = COD_SALUDO;
	int exit = 0;
	while (exit == 0) {
		printf("\nIngrese un mensaje:\n");
		char* message = malloc(sizeof(char) * size_message);
		fgets(message, size_message, stdin);
		if (strcmp(message, "") == 0 || strcmp(message, "exit") == 0)
			exit = 1;
		else {
			Package *package = createPackage(code, message, size_message);
			status = sendPackage(kernelClientFileDescriptor, package);
			if (status != -1)
				printf("\nEnvio OK\n");
			destroyPackage(package);
		}
		free(message);
	}

	return EXIT_SUCCESS;
}
