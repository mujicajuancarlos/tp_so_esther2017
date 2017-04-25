/*
 * handler-user.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(console_struct* consoleStruct) {

	bool exit = false;
	bool shouldCompareCommand;
	while (!exit) {
		printNewLine(consoleStruct->stdoutMutex);
		char** commands = get_user_commands();
		shouldCompareCommand = true;

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_HELP)) {
				shouldCompareCommand = false;
				printCommandsHelp(consoleStruct->stdoutMutex);
			}

			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_RESET)) {
				shouldCompareCommand = false;
				puts("reseteo");
			}

			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_EXIT)) {
				shouldCompareCommand = false;
				puts("exit");
				exit = true;
			}

			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_CLEAR)) {
				shouldCompareCommand = false;
				pthread_mutex_lock(&(consoleStruct->stdoutMutex));
				system("clear");
				pthread_mutex_unlock(&(consoleStruct->stdoutMutex));
			}

			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_START_PROGRAM)) {
				shouldCompareCommand = false;
				handleCommand_start_program(consoleStruct, commands);
			}

			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_INFO_PROGRAM)) {
				shouldCompareCommand = false;
				pthread_mutex_lock(&(consoleStruct->stdoutMutex));
				puts("info");
				pthread_mutex_unlock(&(consoleStruct->stdoutMutex));
			}

			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_STOP_PROGRAM)) {
				shouldCompareCommand = false;
				puts("stop");
				//COD_KC_STOP_PROGRAM_REQUEST
				//handleCommand_end_program(consoleStruct, commands, &shouldCompareCommand);
			}
		}

		if (shouldCompareCommand) {
			pthread_mutex_lock(&(consoleStruct->stdoutMutex));
			printf(
					"«%s» no es un comando válido. Si necesita ayuda use: «%s»",
					commands[0], COD_CONSOLE_HELP);
			pthread_mutex_unlock(&(consoleStruct->stdoutMutex));
		}

		free_user_commands(commands);
	}
}

void handleCommand_start_program(console_struct* consoleStruct, char** commands) {
	if (commands[1] != NULL && commands[2] == NULL) {
		if (file_exists(commands[1], "r")) {
			logInfo("El usuario solicitó ejecutar el programa ansisop %s",
					commands[1]);
			pthread_t hiloPrograma;
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			logInfo("Creando una nueva estructura de programa para  %s",
					commands[1]);
			Program* program = createNewProgram(consoleStruct, commands[1]);//no olvidar librerar memoria al finalizar el hilo
			logDebug("Creando el hilo para ejectar %s", commands[1]);
			pthread_create(&hiloPrograma, &threadAttr, (void*) handleNewProgram,
					program);
			logDebug("Hilo ejecutando %s", commands[1]);
		} else
			printFileNotFound(consoleStruct->stdoutMutex, commands[1]);
	} else
		printInvalidArguments(consoleStruct->stdoutMutex, commands[0]);
}
