/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(kernel_struct* kernelStruct){

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
			COD_CONSOLE_INFO_PROGRAM)) {
				shouldCompareCommand = false;
				handleCommand_info_program(kernelStruct, commands);
			}

			if (shouldCompareCommand && equal_user_command(commands[0],
			COD_CONSOLE_STOP_PROGRAM)) {
				shouldCompareCommand = false;
				handleCommand_end_program(kernelStruct, commands);
			}
		}

		if (shouldCompareCommand) {
			printInvalidCommand(commands[0]);
		}

		free_user_commands(commands);
	}
}

void handleCommand_info_program(kernel_struct* kernelStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_ALL)) {
			handleCommand_info_all_program(kernelStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_PID)) {
			handleCommand_info_by_pid_program(kernelStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_info_all_program(kernel_struct* kernelStruct, char** commands) {
	if (commands[2] == NULL) {
//		void printElement(void* element) {
//			Process* anProgram = (Process*) element;
//			printProgram(anProgram);
//		}
//		lockPrinter();
//		printHeaderProgram();
//		pthread_mutex_lock(&(kernelStruct->));
//		list_iterate(kernelStruct->listaProgramas, printElement);
//		pthread_mutex_unlock(&(kernelStruct->programsListMutex));
//		unlockPrinter();
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_info_by_pid_program(kernel_struct* kernelStruct,
		char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
//		int pid = atoi(commands[2]);
//		bool condicion(void* element) {
//			Program* anProgram = (Program*) element;
//			return pid_isEqual(anProgram, pid);
//		}
//		pthread_mutex_lock(&(kernelStruct->programsListMutex));
//		Program* program = list_find(kernelStruct->listaProgramas, condicion);
//		pthread_mutex_unlock(&(kernelStruct->programsListMutex));
//		if (program != NULL) {
//			lockPrinter();
//			printHeaderProgram();
//			printProgram(program);
//			unlockPrinter();
//		} else {
//			printPidNotFound(pid);
//		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_end_program(kernel_struct* kernelStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_ALL)) {
			handleCommand_end_all_program(kernelStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_PID)) {
			handleCommand_end_by_pid_program(kernelStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_end_all_program(kernel_struct* kernelStruct,
		char** commands) {
//	if (commands[2] == NULL) {
//		void actionByElement(void* element) {
//			Program* anProgram = (Program*) element;
//			sendEndProgramRequest(anProgram, kernelStruct);
//		}
//		pthread_mutex_lock(&(kernelStruct->programsListMutex));
//		list_iterate(kernelStruct->listaProgramas, actionByElement);
//		pthread_mutex_unlock(&(kernelStruct->programsListMutex));
//	} else
//		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_end_by_pid_program(kernel_struct* kernelStruct,
		char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
//		int pid = atoi(commands[2]);
//		bool condicion(void* element) {
//			Program* anProgram = (Program*) element;
//			return pid_isEqual(anProgram, pid);
//		}
//		pthread_mutex_lock(&(kernelStruct->programsListMutex));
//		Program* program = list_find(kernelStruct->listaProgramas, condicion);
//		pthread_mutex_unlock(&(kernelStruct->programsListMutex));
//		if (program != NULL) {
//			sendEndProgramRequest(program, kernelStruct);
//		} else {
//			printPidNotFound(pid);
//		}
	} else
		printInvalidArguments("", commands[0]);
}

void printCommandsHelp() {

	lockPrinter();

	char* patter = "%10s %-5s %-5s\t- %-40s\n";
	printf("\nForma de uso: «command» [«option»] [«args»]\n\n");
	puts("Lista de comandos permitidos:");

	printf(patter,
	COD_CONSOLE_INFO_PROGRAM, OPT_ALL, "",
			"Muestra informacion de todos los procesos");
	printf(patter,
			COD_CONSOLE_INFO_PROGRAM, OPT_STATE, "«state»",
			"Muestra informacion de los procesos correspondientes a «state»");
	printf(patter,
				"", "", "",
				"Estados: NEW, READY, EXEC, BLOCK, EXIT.");
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