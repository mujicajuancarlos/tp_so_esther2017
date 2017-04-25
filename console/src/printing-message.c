/*
 * printing-message.c
 *
 *  Created on: 25/4/2017
 *      Author: utnso
 */

#include "printing-message.h"


void printNewLine(pthread_mutex_t stdoutMutex){
	pthread_mutex_lock(&stdoutMutex);
	printf("\n > ");
	pthread_mutex_unlock(&stdoutMutex);
}

void printFileNotFound(pthread_mutex_t stdoutMutex, char* filePath) {
	pthread_mutex_lock(&stdoutMutex);
	printf("No se pudo acceder al archivo «%s».", filePath);
	pthread_mutex_unlock(&stdoutMutex);
}

void printInvalidArguments(pthread_mutex_t stdoutMutex, char* command) {
	pthread_mutex_lock(&stdoutMutex);
	printf(
			"Los argumentos ingresados no son validos para el comando «%s». Si necesita ayuda use: «%s»",
			command, COD_CONSOLE_HELP);
	pthread_mutex_unlock(&stdoutMutex);
}

void printInvalidOptions(pthread_mutex_t stdoutMutex, char* command) {
	pthread_mutex_lock(&stdoutMutex);
	printf(
			"La opcion ingresada no es valida para el comando «%s». Si necesita ayuda use: «%s»",
			command, COD_CONSOLE_HELP);
	pthread_mutex_unlock(&stdoutMutex);
}

void printCommandsHelp(pthread_mutex_t stdoutMutex) {
	pthread_mutex_lock(&stdoutMutex);
	printf("\nForma de uso: «command» [«option»] [«args»]\n\n");
	puts("Lista de comandos permitidos:");

	printf("\t%s «path»\t\tInicia el programa ansisop descrito en «path»\n",
			COD_CONSOLE_START_PROGRAM);

	printf(
			"\t%s %s\t\tMuestra informacion de todos los programas en ejecución\n",
			COD_CONSOLE_INFO_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tMuestra informacion del programa con «pid»\n",
			COD_CONSOLE_INFO_PROGRAM, OPT_PID);

	printf("\t%s %s\t\tDetiene la ejecución de todos los programas\n",
			COD_CONSOLE_STOP_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tDetiene la ejecución del programa con «pid»\n",
			COD_CONSOLE_STOP_PROGRAM, OPT_PID);

	printf("\t%s\t\t\tLimpia la pantalla\n", COD_CONSOLE_CLEAR);
	printf("\t%s\t\t\tRestablece el proceso por completo\n", COD_CONSOLE_RESET);
	printf("\t%s\t\t\tFinaliza el proceso\n", COD_CONSOLE_EXIT);
	pthread_mutex_unlock(&stdoutMutex);
}

void printWelcome() {
	puts("_____________________________________________________");
	puts("_________ Bienvenido a Esther - Death Coders ________");
	puts("________________ UTN-FRBA SO 1C-2017 ________________");
	puts("_____________________________________________________\n");
}
