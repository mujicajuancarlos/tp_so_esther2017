/*
 * utils.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "utils.h"

char* readFile(char* pathFile){
	FILE *file;
	char* buffer;
	long fsize;

	if((file=fopen(pathFile,"r"))==NULL){
		logError("Error al abrir el archivo %s",pathFile);
		exit (EXIT_FAILURE);
	}

	logDebug("Leyendo el archivo %s",pathFile);

	//Obtengo el tamanio del archivo
	fseek(file, 0, SEEK_END);
	fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Cargo el archivo en el buffer
	buffer = malloc(fsize + 1);
	fread(buffer, fsize, 1, file);
	fclose(file);

	//Agrego el caracter de fin al buffer
	buffer[fsize] = 0;

	logDebug("Archivo leido: \n %s", buffer);

	return buffer;
}

bool file_exists(char* filePath, char* mode) {
	FILE *file;
	file = fopen(filePath, mode);
	if (file!=NULL) {
		fclose(file);
		return true;
	}
	return false;
}

char** get_user_commands(int max_buffer){
	char* buffer = malloc(sizeof(char) * max_buffer);
	fgets(buffer,max_buffer,stdin);
	int length = string_length(buffer);
	if(length > 0 && buffer[--length] == '\n')
		buffer[length] = '\0';
	char** commands = string_split(buffer," ");
	free(buffer);
	return commands;
}

bool equal_user_command(char* userCommand, char* expectedCommand, bool *shouldCompareCommand){
	if(*shouldCompareCommand){
		if(string_equals_ignore_case(userCommand, expectedCommand)){
			*shouldCompareCommand = false;
			return true;
		}
	}
	return false;
}
