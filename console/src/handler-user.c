/*
 * handler-user.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(console_struct* consoleStruct) {

	int exit = 0;
	bool shouldCompareCommand;
	while (exit == 0) {
		printf(" > ");
		char** commands = get_user_commands();
		shouldCompareCommand = true;

		if (equal_user_command(commands[0], COD_CONSOLE_HELP, &shouldCompareCommand)) {
			pthread_mutex_lock(&(consoleStruct->stdoutMutex));
			printCommandsHelp();
			pthread_mutex_unlock(&(consoleStruct->stdoutMutex));
		}

		if (equal_user_command(commands[0], COD_CONSOLE_RESET, &shouldCompareCommand)) {
			puts("reseteo");
		}

		if (equal_user_command(commands[0], COD_CONSOLE_EXIT, &shouldCompareCommand)) {
			puts("exit");
			exit = 1;
		}

		if (equal_user_command(commands[0], COD_CONSOLE_CLEAR, &shouldCompareCommand)) {
			pthread_mutex_lock(&(consoleStruct->stdoutMutex));
			system("clear");
			pthread_mutex_unlock(&(consoleStruct->stdoutMutex));
		}

		if (equal_user_command(commands[0], COD_CONSOLE_START_PROGRAM, &shouldCompareCommand)) {
			handleCommand_start_program(consoleStruct, commands, &shouldCompareCommand);
		}

		if (equal_user_command(commands[0], COD_CONSOLE_INFO_PROGRAM,
				&shouldCompareCommand)) {
			pthread_mutex_lock(&(consoleStruct->stdoutMutex));
			puts("info");
			pthread_mutex_unlock(&(consoleStruct->stdoutMutex));
		}

		if (equal_user_command(commands[0], COD_CONSOLE_STOP_PROGRAM, &shouldCompareCommand)) {
			puts("stop");
			//COD_KC_STOP_PROGRAM_REQUEST
			//handleCommand_end_program(consoleStruct, commands, &shouldCompareCommand);
		}

		if (shouldCompareCommand) {
			pthread_mutex_lock(&(consoleStruct->stdoutMutex));
			printf("«%s» no es un comando válido. Si necesita ayuda use: «%s»\n",commands[0],COD_CONSOLE_HELP);
			pthread_mutex_unlock(&(consoleStruct->stdoutMutex));
		}

		free_user_commands(commands);
	}
}

void handleCommand_start_program(console_struct* consoleStruct, char** commands, bool *shouldCompareCommand){
	if(commands[1]!=NULL && commands[2]==NULL){
		if(file_exists(commands[1],"r")){
			logInfo("El usuario solicitó ejecutar el programa ansisop %s",commands[1]);
			pthread_t hiloPrograma;
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			logInfo("Creando una nueva estructura de programa para  %s", commands[1]);
			Program* program = createNewProgram(consoleStruct, commands[1]);//no olvidar librerar memoria al finalizar el hilo
			logDebug("Creando el hilo para ejectar %s",commands[1]);
			pthread_create(&hiloPrograma, &threadAttr, (void*)handleNewProgram, program);
			logDebug("Hilo ejecutando ejectar %s",commands[1]);
		} else
			printFileNotFound(commands[1]);
	}else
		printInvalidArguments(commands[0]);
}

void printFileNotFound(char* filePath){
	printf("No se pudo acceder al archivo «%s».\n",filePath);
}

void printInvalidArguments(char* command){
	printf("Los argumentos ingresados no son validos para el comando «%s». Si necesita ayuda use: «%s»\n", command, COD_CONSOLE_HELP);
}

void printInvalidOptions(char* command){
	printf("La opcion ingresada no es valida para el comando «%s». Si necesita ayuda use: «%s»\n", command, COD_CONSOLE_HELP);
}

void printCommandsHelp() {
	printf("\nForma de uso: «command» [«option»] [«args»]\n\n");
	puts("Lista de comandos permitidos:");

	printf("\t%s «path»\t\tInicia el programa ansisop descrito en «path»\n", COD_CONSOLE_START_PROGRAM);

	printf("\t%s %s\t\tMuestra informacion de todos los programas en ejecución\n", COD_CONSOLE_INFO_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tMuestra informacion del programa con «pid»\n", COD_CONSOLE_INFO_PROGRAM, OPT_PID);

	printf("\t%s %s\t\tDetiene la ejecución de todos los programas\n", COD_CONSOLE_STOP_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tDetiene la ejecución del programa con «pid»\n", COD_CONSOLE_STOP_PROGRAM, OPT_PID);

	printf("\t%s\t\t\tLimpia la pantalla\n", COD_CONSOLE_CLEAR);
	printf("\t%s\t\t\tRestablece el proceso por completo\n", COD_CONSOLE_RESET);
	printf("\t%s\t\t\tFinaliza el proceso\n", COD_CONSOLE_EXIT);
}

void printWelcome() {
	puts("_____________________________________________________");
	puts("_________ Bienvenido a Esther - Death Coders ________");
	puts("________________ UTN-FRBA SO 1C-2017 ________________");
	puts("_____________________________________________________");
	puts("");
}
