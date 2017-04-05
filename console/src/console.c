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
#include "configuration.h"

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuracion");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	printf("IP_KERNEL= %d\nPUERTO_KERNEL = %d\n",config->ip_kernel,config->puerto_kernel);

	return EXIT_SUCCESS;
}
