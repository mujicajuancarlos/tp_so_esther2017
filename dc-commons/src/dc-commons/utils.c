/*
 * utils.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "utils.h"

char* readFile(char* pathFile, long* fsize) {
	FILE *file;
	char* buffer;

	if ((file = fopen(pathFile, "r")) == NULL) {
		logError("Error al abrir el archivo %s", pathFile);
		exit(EXIT_FAILURE);
	}

	logDebug("Leyendo el archivo %s", pathFile);

	//Obtengo el tamanio del archivo
	fseek(file, 0, SEEK_END);
	*fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Cargo el archivo en el buffer
	buffer = malloc(*fsize + 1);
	fread(buffer, *fsize, 1, file);
	fclose(file);

	//Agrego el caracter de fin al buffer
	buffer[*fsize] = 0;

	logDebug("Archivo leido: \n %s", buffer);

	return buffer;
}

void writeFile(char* buffer, char* pathFile, long fsize) {
	FILE *file;

	file = fopen(pathFile, "w");  // w for write, b for binary

	fwrite(buffer, (size_t) 1, (size_t) fsize, file); // write 10 bytes from our buffer

	fclose(file);
}

bool file_exists(char* filePath, char* mode) {
	FILE *file;
	file = fopen(filePath, mode);
	if (file != NULL) {
		fclose(file);
		return true;
	}
	return false;
}

char* getStdinString() {

	unsigned int maxlen = 16, size = 16;
	char* buffer = malloc(maxlen);

	int ch = EOF;
	int pos = 0;

	/* Read input one character at a time, resizing the buffer as necessary */
	while ((ch = getchar()) != '\n' && ch != EOF) {
		buffer[pos++] = ch;
		/* Next character to be inserted needs more memory */
		if (pos == size) {
			size = pos + maxlen;
			buffer = (char*) realloc(buffer, size);
		}
	}
	buffer[pos] = '\0'; /* Null-terminate the completed string */

	return buffer;
}

char** get_user_commands() {
	char* buffer = getStdinString();
	char** commands = string_split(buffer, " ");
	free(buffer);
	return commands;
}

void free_user_commands(char** array) {
	int i = 0;
	while (array[i] != NULL) {
		free(array[i++]);
	}
	free(array);
}

bool equal_user_command(char* userCommand, char* expectedCommand) {
	return string_equals_ignore_case(userCommand, expectedCommand);
}
