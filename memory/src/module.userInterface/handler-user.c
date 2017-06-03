/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(memory_struct* memoryStruct){

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
			COD_CONSOLE_DUMP)) {
				shouldCompareCommand = false;
				handleCommand_dump(memoryStruct, commands);
			}

			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_SLEEP)) {
				shouldCompareCommand = false;
				handleCommand_set_sleep (memoryStruct, commands);
			}
		}

		if (shouldCompareCommand) {
			printInvalidCommand(commands[0]);
		}

		free_user_commands(commands);
	}
}

void handleCommand_dump(memory_struct* memoryStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_CACHE)) {
			handleCommand_dump_cache(memoryStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_STRUCTURE)) {
			handleCommand_dump_structure(memoryStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_CONTENT)) {
					handleCommand_dump_content(memoryStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_dump_cache(memory_struct* memoryStruct, char** commands) {
	if (commands[2] == NULL) {
		printf ("\ndump de cache\n");
/*
 * TODO dump completo de la cache
 */
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_dump_structure(memory_struct* memoryStruct,
		char** commands) {
	if (commands[2] == NULL) {
		printf ("\ estado de tabla de paginas y listado de procesos activos\n");
/*
 * TODO estado de tabla de paginas y listado de procesos activos
 */
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_dump_content(memory_struct* kernelStruct, char** commands) {
	if (commands[2] == NULL) {
		printf ("\ datos almacenados en memoria de todos los proc o de alguno en particular\n");
/*
 * TODO datos almacenados en la memoria de todos los procesos o de alguno en particular
 */
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_set_sleep (memory_struct* memoryStruct, char** commands) {

	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_MILISECONDS)) {
			printf ("\n el sleep se ha modificado \n");
			handleCommand_set_sleep_to_value(memoryStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);

}

void handleCommand_set_sleep_to_value (memory_struct* memoryStruct, char** commands) {
	int maxValue = 100;
	int minValue = 0;


	if (commands[2] != NULL && commands[3] == NULL) {
		int newValue = atoi (commands[2]);

		if (newValue > maxValue)
			memoryStruct->memorySleep = maxValue;
		else if (newValue < minValue)
			memoryStruct->memorySleep = minValue;
		else
			memoryStruct->memorySleep = newValue;
	}

	else
		printInvalidArguments("", commands[0]);

	printf ("%i\n", memoryStruct->memorySleep);
}

void printCommandsHelp() {

	lockPrinter();

	char* patter = "%10s %-5s\t- %-40s\n";
	printf("\nForma de uso: «command» [«option»] \n\n");
	puts("Lista de comandos permitidos:");

	printf(patter,
	COD_CONSOLE_DUMP, OPT_ALL,
			"Muestra informacion de todos las paginas");
	printf(patter,
			COD_CONSOLE_DUMP, OPT_PAGE,
			"Muestra informacion de una página especifica «page»");

	printf(patter, COD_CONSOLE_CLEAR, "", "", "Limpia la pantalla");

	printf(patter, COD_CONSOLE_EXIT, "", "", "Finaliza el proceso");

	unlockPrinter();
}
