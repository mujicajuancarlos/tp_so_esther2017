/*
 * config.c
 *
 *  Created on: 17/4/2017
 *      Author: utnso
 */

#include "config.h"

void config_set_string_valid_value(char* *value, t_config* temporalConfig,
		char* key) {
	if (config_has_property(temporalConfig, key))
		*value = strdup(config_get_string_value(temporalConfig, key));
	else
		printError(key);

}

void config_set_int_valid_value(int *value, t_config* temporalConfig,
		char* key) {
	if (config_has_property(temporalConfig, key))
		*value = config_get_int_value(temporalConfig, key);
	else
		printError(key);
}

void printError(char* key) {
	error_show("Archivo de configuracion incompleto, no se encontro -> %s",
			key);
	exit(1);
}
