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


long fileSize(char* pathFile);

/**
 * @NAME: readFile
 * @DESC: devuelve el contenido del archivo pathFile
 * @PARAMS:
 *		char* path del archivo
 * @RETURN: char* contenido del archivo
 */
char* readFile(char* pathFile, long startByte, long endByte);

void writeFile(char* buffer, long sizeBuffer, char* pathFile, long startByte);

/**
 * @NAME: file_exists
 * @DESC: indica si un archivo existe y puede ser accedido
 * @PARAMS:
 *		char* path del archivo
 *		char* modo de apertura
 * @RETURN: true si existe y puede ser manipulado en el modo indicado
 */
bool existFile(char* filePath, char* mode);

/**
 * @NAME: getStdinString
 * @DESC: aloca bloqes de memoria a segun lo va necesitando devuelve un string
 * @RETURN: retorna un string alocado en memoria dinamica
 */
char* getStdinString();

/**
 * @NAME: get_user_commands
 * @DESC: Lee de teclado una linea la separa y devuelve el array de comandos
 * @RETURN: array con comandos
 */
char** get_user_commands();

/**
 * @NAME: free_user_commands
 * @DESC: libera el array de char* generados por get_user_commands
 */
void free_user_commands(char** array);

/**
 * @NAME: equal_user_command
 * @DESC: si son iguales devuelvo true en otros casos retorna falso
 * @PARAMS:
 *		userCommand string que ingreso el usuario
 *		expectedCommand string que se esperaba
 * @RETURN: bool
 */
bool equal_user_command(char* userCommand, char* expectedCommand);

#endif /* SRC_DC_COMMONS_UTILS_H_ */
