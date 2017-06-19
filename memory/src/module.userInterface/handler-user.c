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
		printf ("==================================\n");
		if (list_size (memoryStruct->cacheEntries) == 0) {
			printf ("No hay paginas cargadas en memoria cache\n");
		}
		else {
			int i;
			for (i = 0; i < list_size (memoryStruct->cacheEntries); i++) {
				cache_entry* entry = list_get (memoryStruct->cacheEntries, i);
				printf 	("| Pag %i   - pID %i   |\n", entry->procPage, entry->pid);
			}
		}
		printf ("==================================\n");
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_dump_structure(memory_struct* memoryStruct, char** commands) {
	if (commands[2] == NULL) {
		t_list* occupiedPages = list_create();
		int freePages = 0;

		bool onlyOccupiedPages(void* element) {
				memory_page* page = element;
				return (page->isFree == false);
		}
		occupiedPages = list_filter (memoryStruct->referenceTable, onlyOccupiedPages);
		freePages = list_size (memoryStruct->referenceTable) - list_size (occupiedPages);

		t_list* pids;
		pids = list_create();
		int getPid (void* element) {
			memory_page* page = element;
			return (page->pid);
		}
		pids = list_map (occupiedPages, getPid);
		int i;
		for (i = 0; i < list_size (pids); i++) {
			int j;
			for (j = i; j < list_size (pids); j++) {
				if (list_get(pids, i) == list_get(pids, j))
					list_remove (pids, j--);
			}
		}

		printf ("==================================\n");
		printf ("Total de paginas de memoria: %i\n", list_size (memoryStruct->referenceTable));
		printf ("Paginas de memoria ocupadas: %i\n", list_size (occupiedPages));
		printf ("Paginas de memoria libres:   %i\n", freePages);
		if (list_size (pids) == 0)
			printf ("Procesos activos:         ----\n");
		else {
			printf ("Procesos activos:");
			for (i = 0; i < list_size (pids); i++) {
				printf ("                  %i            \n", (int) list_get (pids, i));
			}
		}
		printf ("==================================\n");

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

	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_dump_content_all(memory_struct* memoryStruct,
		char** commands) {
	if (commands[3] == NULL) {

		bool isPageOccupied (void* element) {
			memory_page* page = element;
			return (page->isFree == false);
		}

		t_list* occupiedPages = list_create();
		occupiedPages = list_filter (memoryStruct->referenceTable, isPageOccupied);

		printf ("==================================\n");
		if (list_size (occupiedPages) == 0)
			printf ("No hay paginas en memoria asignadas a procesos aun\n");
		else {
			bool orderMethod (void* elementOne, void* elementTwo) {
				memory_page* pageOne = elementOne;
				memory_page* pageTwo = elementTwo;
				return (pageOne->pid <= pageTwo->pid);
			}

			list_sort (occupiedPages, orderMethod);

			printf ("Tenemos las siguientes paginas cargadas en memoria\n");
			int i;
			for (i = 0; i < list_size (occupiedPages); i++) {
				memory_page* page = list_get (occupiedPages, i);
				printf ("Proceso %i, pagina %i\n", page->pid, page->procPage);
			}
		}
		printf ("==================================\n");
	} else
		printInvalidArguments(commands[3], commands[0]);
}

void handleCommand_dump_content_pid(memory_struct* memoryStruct,
		char** commands) {
	if (commands[3] != NULL && commands[4] == NULL) {
		int pid = atoi(commands[3]);

		bool justThisProcess (void* element) {
			memory_page* page = element;
			return (page->pid == pid);
		}

		t_list* processPages = list_create ();
		processPages = list_filter (memoryStruct->referenceTable, justThisProcess);

		printf ("==================================\n");
		if (pid < 1000)
			printf ("pID invalido\n");
		else if (list_size (processPages) == 0)
			printf ("No se encuentra paginas del proceso %i cargadas en memoria\n", pid);
		else {
			printf("Para el proceso %i se encuentra las siguientes paginas cargadas en memoria:\n", pid);
			int i;
			for (i = 0; i < list_size (processPages); i++) {
				memory_page* page = list_get (processPages, i);
				printf ("Pagina %i del proceso %i\n", page->procPage, pid);
			}
		}
		printf ("==================================\n");
	} else
		printInvalidArguments(commands[3], commands[0]);
}

void handleCommand_set_sleep(memory_struct* memoryStruct, char** commands) {

	if (commands[1] != NULL && commands[2] == NULL) {
		int sleep = atoi(commands[1]);
		printf ("==================================\n");
		if (sleep < 0) {
			printf ("El valor de retardo minimo posible es de 0 milisegundos\n");
			sleep = 0;
		}
		memoryStruct->config->retardo_memoria = sleep;
		printf("El valor de retardo fue modificado a %i milisegundos\n", sleep);
		printf ("==================================\n");
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
		list_clean (memoryStruct->cacheEntries);
		printf ("==================================\n");
		printf("Memoria cache ha sido limpiada\n");
		printf ("==================================\n");
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
	printf ("==================================\n");
	printf("El tamaño de la memoria es de %i bytes\n", memoryStruct->memorySize);
	printf ("==================================\n");
}

void handleCommand_size_pid(memory_struct* memoryStruct, char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
		int pid = atoi(commands[2]);

		bool justThisProcess (void* element) {
			memory_page* page = element;
			return (page->pid == pid);
		}
		t_list* processPages = list_create();
		processPages = list_filter (memoryStruct->referenceTable, justThisProcess);
		uint32_t processSize = memoryStruct->config->marco_size * list_size (processPages);
		printf ("==================================\n");
		if (pid < 1000)
			printf("pID invalido\n");
		else if (list_size (processPages) == 0)
			printf("El proceso %i no ha sido cargado en memoria\n", pid);
		else
			printf("El tamaño del proceso en memoria es de %i bytes\n", processSize);
		printf ("==================================\n");
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

	printf(patter, COD_CONSOLE_SLEEP, "«valor»",
			"Setea nuevo valor para retardo en milisegundos (valor entero).");

	printf(patter, COD_CONSOLE_SIZE, OPT_MEMORY,
			"Tamaño total de memoria");

	printf(patter, COD_CONSOLE_SIZE, OPT_DESIRED_PID,
				"Tamaño total de un proceso en memoria");

	printf(patter, COD_CONSOLE_CLEAR, "", "", "Limpia la pantalla");

	printf(patter, COD_CONSOLE_EXIT, "", "", "Finaliza el proceso");

	unlockPrinter();
}
