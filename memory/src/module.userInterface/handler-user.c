/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(memory_struct* memoryStruct) {

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

			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_CONSOLE_SLEEP)) {
				shouldCompareCommand = false;
				handleCommand_set_sleep(memoryStruct, commands);
			}
			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_CONSOLE_FLUSH)) {
				shouldCompareCommand = false;
				handleCommand_flush(memoryStruct, commands);
			}
			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_CONSOLE_SIZE)) {
				shouldCompareCommand = false;
				handleCommand_size(memoryStruct, commands);
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
		printf("\ndump de cache\n");
		/*
		 * TODO dump completo de la cache
		 */
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_dump_structure(memory_struct* memoryStruct, char** commands) {
	if (commands[2] == NULL) {
		printf("\nestado de tabla de paginas y listado de procesos activos\n");
		/*
		 * TODO estado de tabla de paginas y listado de procesos activos
		 */
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_dump_content(memory_struct* memoryStruct, char** commands) {
	if (commands[2] != NULL) {
		if (equal_user_command(commands[2], OPT_ALL)) {
			handleCommand_dump_content_all(memoryStruct, commands);
		}
		if (equal_user_command(commands[2], OPT_DESIRED_PID)) {
			handleCommand_dump_content_pid(memoryStruct, commands);
		}
		/*
		 * TODO datos almacenados en la memoria de todos los procesos o de alguno en particular
		 */
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_dump_content_all(memory_struct* memoryStruct,
		char** commands) {
	if (commands[3] == NULL) {
		printf("\n dump de contenido completo \n");
	} else
		printInvalidArguments(commands[3], commands[0]);
}

void handleCommand_dump_content_pid(memory_struct* memoryStruct,
		char** commands) {
	if (commands[3] != NULL && commands[4] == NULL) {
		int pid = atoi(commands[3]);
		printf("\n dump de contenido completo de proceso %i \n", pid);
	} else
		printInvalidArguments(commands[3], commands[0]);
}

void handleCommand_set_sleep(memory_struct* memoryStruct, char** commands) {

	if (commands[1] != NULL && commands[2] == NULL) {
		int sleep = atoi(commands[1]);
		memoryStruct->memorySleep = sleep;
		printf("\nEl valor de retardo fue modificado a %i\n", sleep);
	} else
		printInvalidArguments("", commands[0]);

}

void handleCommand_flush(memory_struct* memoryStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_CACHE)) {
			handleCommand_flush_cache(memoryStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_flush_cache(memory_struct* memoryStruct, char** commands) {
	if (commands[2] == NULL) {
		printf("\n se hace flush de cache\n");
		/*
		 * TODO flush completo de la cache
		 */
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_size(memory_struct* memoryStruct, char** commands) {
	if (commands[1] != NULL) {
		if (equal_user_command(commands[1], OPT_MEMORY)) {
			handleCommand_size_memory(memoryStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_DESIRED_PID)) {
			handleCommand_size_pid(memoryStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_size_memory(memory_struct* memoryStruct, char** commands) {
	printf("\nEl tamaño de la memoria es de X \n");
}

void handleCommand_size_pid(memory_struct* memoryStruct, char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
		int pid = atoi(commands[2]);
		printf("\n El tamaño del proceso %i es de X\n", pid);
	} else
		printInvalidArguments("", commands[0]);
}

void printCommandsHelp() {

	lockPrinter();

	char* patter = "%10s %-5s\t- %-40s\n";
	printf("\nForma de uso: «command» [«option»] \n\n");
	puts("Lista de comandos permitidos:");

	printf(patter, COD_CONSOLE_DUMP, OPT_CACHE,
			"Dump completo de memoria cache");

	printf(patter, COD_CONSOLE_DUMP, OPT_STRUCTURE,
			"Muestra tabla de páginas y listado de procesos activos");

	printf(patter, COD_CONSOLE_DUMP, OPT_CONTENT,
			"Muestra datos almacenados en memoria de todos los procesos «-all» o de un proceso en particular «-pid»");

	printf(patter, COD_CONSOLE_FLUSH, OPT_CACHE,
			"Limpia completamente el contenido de la cache");

	printf(patter, COD_CONSOLE_FLUSH, OPT_CACHE,
			"Limpia completamente el contenido de la cache");

	printf(patter, COD_CONSOLE_SLEEP, "",
			"Setea nuevo valor para retardo en milisegundos (valor entero).");

	printf(patter, COD_CONSOLE_SIZE, OPT_DESIRED_PID,
			"Tamaño total de un proceso");

	printf(patter, COD_CONSOLE_CLEAR, "", "", "Limpia la pantalla");

	printf(patter, COD_CONSOLE_EXIT, "", "", "Finaliza el proceso");

	unlockPrinter();
}
