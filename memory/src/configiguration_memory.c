/*
 * configiguration_memory.c
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#include "configuration_memory.h"

Configuration* config_with(char *config_file){

	Configuration* config = malloc(sizeof(Configuration));

	t_config* nConfig = config_create(config_file ? config_file : DEFAULT_CONFIG_PATH);
	if(nConfig==NULL){
		//para debuggear desde eclipse
		nConfig = config_create(ECLIPSE_CONFIG_PATH);
		if(nConfig==NULL){
			printf("No se encontro el archivo de configuracion.\n");
			exit (1);
		}
	}

	config->puerto=config_get_int_value(nConfig,PUERTO);
	config->marcos =config_get_int_value(nConfig,MARCOS);
	config->marco_size =config_get_int_value(nConfig,MARCO_SIZE);
	config->entradas_cache = config_get_int_value(nConfig,ENTRADAS_CACHE);
	config->cache_x_proceso = config_get_int_value(nConfig,CACHE_X_PROC);
	config->reemplazo_cache = strdup(config_get_string_value(nConfig,REEMPLAZO_CACHE));
	config->retardo_memoria = config_get_int_value(nConfig,RETARDO_MEMORIA);

	config_destroy(nConfig);

	return config;
}

Configuration* getConfiguration(){
	return config;
}
