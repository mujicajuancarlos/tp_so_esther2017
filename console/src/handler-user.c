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
		printNewLine();
		char** commands = get_user_commands();
		shouldCompareCommand = true;

		if (*commands != NULL) {
			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_CONSOLE_HELP)) {
				shouldCompareCommand = false;
				printCommandsHelp();
			}

			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_CONSOLE_EXIT)) {
				shouldCompareCommand = false;
				exit = true;
			}

			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_CONSOLE_CLEAR)) {
				shouldCompareCommand = false;
				clearScreem();
			}

			if (shouldCompareCommand && equal_user_command(commands[0],
			COD_CONSOLE_START_PROGRAM)) {
				shouldCompareCommand = false;
				handleCommand_start_program(consoleStruct, commands);
			}

			if (shouldCompareCommand && equal_user_command(commands[0],
			COD_CONSOLE_INFO_PROGRAM)) {
				shouldCompareCommand = false;
				handleCommand_info_program(consoleStruct, commands);
			}

			if (shouldCompareCommand && equal_user_command(commands[0],
			COD_CONSOLE_STOP_PROGRAM)) {
				shouldCompareCommand = false;
				handleCommand_end_program(consoleStruct, commands);
			}
		}

		if (shouldCompareCommand) {
			printInvalidCommand(commands[0]);
		}

		free_user_commands(commands);
	}
}

void handleCommand_start_program(console_struct* consoleStruct, char** commands) {
	if (commands[1] != NULL && commands[2] == NULL) {
		if (existFile(commands[1], "r")) {
			logInfo("El usuario solicitó ejecutar el programa ansisop %s",
					commands[1]);
			pthread_t hiloPrograma;
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			logInfo("Creando una nueva estructura de programa para  %s",
					commands[1]);
			Program* program = createNewProgram(consoleStruct, commands[1]); //no olvidar librerar memoria al finalizar el hilo
			logDebug("Creando el hilo para ejectar %s", commands[1]);
			pthread_create(&hiloPrograma, &threadAttr, (void*) handleNewProgram,
					program);
			logDebug("Hilo ejecutando %s", commands[1]);
		} else
			printFileNotFound(commands[1]);
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_info_program(console_struct* consoleStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_ALL)) {
			handleCommand_info_all_program(consoleStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_PID)) {
			handleCommand_info_by_pid_program(consoleStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_info_all_program(console_struct* consoleStruct,
		char** commands) {
	if (commands[2] == NULL) {
		void printElement(void* element) {
			Program* anProgram = (Program*) element;
			printProgram(anProgram);
		}
		lockPrinter();
		printHeaderProgram();
		pthread_mutex_lock(&(consoleStruct->programsListMutex));
		list_iterate(consoleStruct->listaProgramas, printElement);
		pthread_mutex_unlock(&(consoleStruct->programsListMutex));
		unlockPrinter();
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_info_by_pid_program(console_struct* consoleStruct,
		char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
		int pid = atoi(commands[2]);
		bool condicion(void* element) {
			Program* anProgram = (Program*) element;
			return pid_isEqual(anProgram, pid);
		}
		pthread_mutex_lock(&(consoleStruct->programsListMutex));
		Program* program = list_find(consoleStruct->listaProgramas, condicion);
		pthread_mutex_unlock(&(consoleStruct->programsListMutex));
		if (program != NULL) {
			lockPrinter();
			printHeaderProgram();
			printProgram(program);
			unlockPrinter();
		} else {
			printPidNotFound(pid);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_end_program(console_struct* consoleStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_ALL)) {
			handleCommand_end_all_program(consoleStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_PID)) {
			handleCommand_end_by_pid_program(consoleStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_end_all_program(console_struct* consoleStruct,
		char** commands) {
	if (commands[2] == NULL) {
		void actionByElement(void* element) {
			Program* anProgram = (Program*) element;
			sendEndProgramRequest(anProgram, consoleStruct);
		}
		pthread_mutex_lock(&(consoleStruct->programsListMutex));
		list_iterate(consoleStruct->listaProgramas, actionByElement);
		pthread_mutex_unlock(&(consoleStruct->programsListMutex));
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_end_by_pid_program(console_struct* consoleStruct,
		char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
		int pid = atoi(commands[2]);
		bool condicion(void* element) {
			Program* anProgram = (Program*) element;
			return pid_isEqual(anProgram, pid);
		}
		pthread_mutex_lock(&(consoleStruct->programsListMutex));
		Program* program = list_find(consoleStruct->listaProgramas, condicion);
		pthread_mutex_unlock(&(consoleStruct->programsListMutex));
		if (program != NULL) {
			sendEndProgramRequest(program, consoleStruct);
		} else {
			printPidNotFound(pid);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void printCommandsHelp() {

	lockPrinter();

	char* patter = "%10s %-5s %-5s\t- %-40s\n";
	printf("\nForma de uso: «command» [«option»] [«args»]\n\n");
	puts("Lista de comandos permitidos:");

	printf(patter,
	COD_CONSOLE_START_PROGRAM, "«path»", "", "Inicia el programa ansisop descrito en «path»");

	printf(patter,
	COD_CONSOLE_INFO_PROGRAM, OPT_ALL, "",
			"Muestra informacion de todos los programas en ejecución");
	printf(patter,
	COD_CONSOLE_INFO_PROGRAM, OPT_PID, "«pid»",
			"Muestra informacion del programa con «pid»");

	printf(patter,
	COD_CONSOLE_STOP_PROGRAM, OPT_ALL, "",
			"Detiene la ejecución de todos los programas");
	printf(patter,
	COD_CONSOLE_STOP_PROGRAM, OPT_PID, "«pid»",
			"Detiene la ejecución del programa con «pid»");


	printf(patter, COD_CONSOLE_CLEAR, "", "", "Limpia la pantalla");

	printf(patter, COD_CONSOLE_EXIT, "", "", "Finaliza el proceso");

	unlockPrinter();
}



