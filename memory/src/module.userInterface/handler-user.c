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
		}

		if (shouldCompareCommand) {
			printInvalidCommand(commands[0]);
		}

		free_user_commands(commands);
	}
}

void handleCommand_dump(memory_struct* memoryStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_ALL)) {
			handleCommand_dump_all(memoryStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_PAGE)) {
			handleCommand_dump_page(memoryStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_dump_all(memory_struct* memoryStruct, char** commands) {
	if (commands[2] == NULL) {
/*
 * TODO imprimir todas las paginas
 */
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_dump_page(memory_struct* memoryStruct,
		char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
/*
 * TODO buscar e imprimir una pagina
 */
	} else
		printInvalidArguments("", commands[0]);
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
