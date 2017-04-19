/*
 * handler-user.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(console_struct* args) {

	int exit = 0;
	int size_message = 200;
	while (exit == 0) {
		printf("\n > ");
		char* message = malloc(sizeof(char) * size_message);
		fgets(message, size_message, stdin);

		if(strcmp(message, COD_HELP) == 0)
			puts("ayudaaaaa");

		if(strcmp(message, COD_START_PROGRAM) == 0){
			logInfo("Creando el hilo para ejecutar un programa");
			//pthread_t hiloPrograma;
			//pthread_create(&hiloPrograma, NULL, (void*) handleProgram, &args);
		}

		if(strcmp(message, COD_EXIT) == 0)
			exit = 1;


		free(message);
	}
}

void clearConsoleScreen() {
	system("clear");
}

void printWelcome() {
	puts("_____________________________________________________");
	puts("_________ Bienvenido a Esther - Death Coders ________");
	puts("________________ UTN-FRBA SO 1C-2017 ________________");
	puts("_____________________________________________________");
	puts("");
}
