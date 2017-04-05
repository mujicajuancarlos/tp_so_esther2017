/*
 * configuration.c
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#include "configuration.h"

Configuration* config_with(char *config_file){

	Configuration* config = malloc(sizeof(Configuration));

	t_config* nConfig = config_create(config_file ? config_file : DEFAULT_CONFIG_PATH);
	if( nConfig==NULL ){
		//para debuggear desde eclipse
		nConfig = config_create(ECLIPSE_CONFIG_PATH);
		if(nConfig==NULL){
			printf("No se encontro el archivo de configuracion.\n");
			exit (1);
		}
	}

	config->ip_kernel=config_get_int_value(nConfig,IP_KERNEL);
	config->puerto_kernel=config_get_int_value(nConfig,PUERTO_KERNEL);

	config_destroy(nConfig);

	return config;
}

Configuration* getConfiguration(){
	return config;
}

