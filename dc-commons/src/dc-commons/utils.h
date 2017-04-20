/*
 * utils.h
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_UTILS_H_
#define SRC_DC_COMMONS_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/string.h>

/**
 * @NAME: get_user_commands
 * @DESC: Lee de teclado una linea la separa y devuelve el array de comandos
 * @PARAMS:
 *		int max_buffer - define el tamaño maximo de la entrada por teclado
 * @RETURN: array con comandos
 */
char** get_user_commands(int max_buffer);

/**
 * @NAME: equal_user_command
 * @DESC: si shouldCompareCommand es true comparo userCommand y expectedCommand, si son iguales devuelvo true y actualizo shouldCompareCommand con false
 * 		en otros casos retorna falso
 * @PARAMS:
 *		userCommand string que ingreso el usuario
 *		expectedCommand string que se esperaba
 *		shouldCompareCommand booleano que define si la comparacion se realiza o no
 * @RETURN: bool
 */
bool equal_user_command(char* userCommand, char* expectedCommand, bool *shouldCompareCommand);

#endif /* SRC_DC_COMMONS_UTILS_H_ */
