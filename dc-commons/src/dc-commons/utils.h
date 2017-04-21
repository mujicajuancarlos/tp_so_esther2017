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
#include "logger.h"

/**
 * @NAME: readFile
 * @DESC: devuelve el contenido del archivo pathFile
 * @PARAMS:
 *		char* path del archivo
 * @RETURN: char* contenido del archivo
 */
char* readFile(char* pathFile);

/**
 * @NAME: file_exists
 * @DESC: indica si un archivo existe y puede ser accedido
 * @PARAMS:
 *		char* path del archivo
 *		char* modo de apertura
 * @RETURN: true si existe y puede ser manipulado en el modo indicado
 */
bool file_exists(char* filePath, char* mode);

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
