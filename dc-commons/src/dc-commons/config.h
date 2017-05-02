/*
 * config.h
 *
 *  Created on: 17/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_CONFIG_H_
#define SRC_DC_COMMONS_CONFIG_H_

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/config.h>
#include <commons/error.h>

/**
 * Functions
 */

/**
 * @NAME: config_set_string_valid_value
 * @DESC: ejecuta config_get_string_value de las commons solo si el key existe en temporalConfig si no existe muestra error y termina el programa
 * @PARAMS:
 * 		puntero a char donde se almacena el valor obtenido del archivo de config
 * 		direccion de memoria de la configuracion temporal
 * 		direccion de memoria de la llave
 */
void config_set_string_valid_value(char* *value, t_config* temporalConfig, char* key);

/**
 * @NAME: config_set_string_valid_value
 * @DESC: ejecuta config_get_int_value de las commons solo si el key existe en temporalConfig si no existe muestra error y termina el programa
 * @PARAMS:
 * 		puntero a int donde se almacena el valor obtenido del archivo de config
 * 		direccion de memoria de la configuracion temporal
 * 		direccion de memoria de la llave
 */
void config_set_int_valid_value(int *value, t_config* temporalConfig, char* key);

/**
 * @NAME: config_set_bool_valid_value
 * @DESC: ejecuta config_get_int_value de las commons solo si el key existe en temporalConfig si no existe muestra error y termina el programa
 * @PARAMS:
 * 		puntero a bool donde se almacena el valor obtenido del archivo de config
 * 		direccion de memoria de la configuracion temporal
 * 		direccion de memoria de la llave
 */
void config_set_bool_valid_value(bool *value, t_config* temporalConfig,
		char* key);

/**
 * @NAME: printError
 * @DESC: funcion interna para mostrar error y terminar el programa
 * @PARAMS: direccion de memoria de la llave
 */
void printError(char* key);

#endif /* SRC_DC_COMMONS_CONFIG_H_ */
