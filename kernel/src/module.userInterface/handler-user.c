/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(kernel_struct* kernelStruct) {

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
			COD_CONSOLE_SET_NEW_MULTIPROGRAMMING_LEVEL)) {
				shouldCompareCommand = false;
				handleCommand_set_new_multiprogramming_level(kernelStruct,
						commands);
			}

			if (shouldCompareCommand && equal_user_command(commands[0],
			COD_CONSOLE_STOP_PROGRAM)) {
				shouldCompareCommand = false;
				handleCommand_end_program(kernelStruct, commands);
			}

			if (shouldCompareCommand && equal_user_command(commands[0],
			COD_CONSOLE_PLANING)) {
				shouldCompareCommand = false;
				handleCommand_lock_unlock(kernelStruct, commands);
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
			handleCommand_info_all_process(kernelStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_STATE)) {
			handleCommand_info_by_state_process(kernelStruct, commands);
		}
		if (equal_user_command(commands[1], OPT_PID)) {
			handleCommand_info_by_pid_process(kernelStruct, commands);
		}
	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_set_new_multiprogramming_level(kernel_struct* kernelStruct,
		char** commands) {

	if (commands[1] != NULL) {
		if (commands[2] == NULL) {

			int newValue = atoi(commands[1]);

			if ((newValue > 0) && (newValue <= MAX_MULTIPROGRAMMING_LVL)) {
				updateMultiprogrammingLevel(kernelStruct, newValue);
			} else if ((newValue == 0)
					&& (string_equals_ignore_case(commands[1], "0"))) { //Caso que sea 0, para evitar otras cadenas al utilizar atoi().
				updateMultiprogrammingLevel(kernelStruct, newValue);
			} else
				printInvalidArguments(commands[1], commands[0]);

		} else
			printInvalidArguments(commands[2], commands[0]);
	} else
		printInvalidArguments("", commands[0]);

}

void handleCommand_info_all_process(kernel_struct* kernelStruct,
		char** commands) {
	if (commands[2] == NULL) {
		t_planningStates* states = getStates();
		char* state;
		printHeaderProcess();
		void printElement(void* element) {
			Process* proces = element;
			printProcess(proces, state);
		}
		state = "new";
		list_iterate(states->new, printElement);
		state = "ready";
		list_iterate(states->ready->elements, printElement);
		state = "execute";
		list_iterate(states->execute, printElement);
		state = "block";
		list_iterate(states->block->elements, printElement);
		state = "exit";
		list_iterate(states->exit->elements, printElement);
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_info_by_state_process(kernel_struct* kernelStruct,
		char** commands) {
	if (commands[2] != NULL) {
		if (commands[3] == NULL) {
			char* state = commands[2];
			bool valid = false;
			t_planningStates* states = getStates();
			printHeaderProcess();
			void printElement(void* element) {
				Process* proces = element;
				printProcess(proces, state);
			}
			if (string_equals_ignore_case(state, "new")) {
				list_iterate(states->new, printElement);
				valid = true;
			}
			if (string_equals_ignore_case(state, "ready")) {
				list_iterate(states->ready->elements, printElement);
				valid = true;
			}
			if (string_equals_ignore_case(state, "execute")) {
				list_iterate(states->execute, printElement);
				valid = true;
			}
			if (string_equals_ignore_case(state, "block")) {
				list_iterate(states->block->elements, printElement);
				valid = true;
			}
			if (string_equals_ignore_case(state, "exit")) {
				list_iterate(states->exit->elements, printElement);
				valid = true;
			}
			if (!valid) {
				printInvalidArguments(state, commands[0]);
			}
		} else
			printInvalidArguments(commands[3], commands[0]);
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_info_by_pid_process(kernel_struct* kernelStruct,
		char** commands) {
	if (commands[2] != NULL) {
		if (commands[3] == NULL) {
			int pid = atoi(commands[2]);
			Process* process = getProcessByPID(pid);
			if (process != NULL) {
				printProcessFull(process);
			} else {
				if (pid == 0)
					printInvalidArguments(commands[2], commands[0]);
				else
					printPidNotFound(pid);
			}
		} else
			printInvalidArguments(commands[3], commands[0]);
	} else
		printInvalidArguments(commands[2], commands[0]);
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

void handleCommand_lock_unlock(kernel_struct* kernelStruct, char** commands) {
	if (commands[1] != NULL) {
		if (commands[2] == NULL) {
			char* planning = commands[1];
			//bool validar = false;

			if (string_equals_ignore_case(planning, "Bloqueado")) {
				shortTermScheduler_lock();
				//	validar = true;
			} else if (string_equals_ignore_case(planning, "Desbloqueado")) {
				shortTermScheduler_unlock();
				//	validar = true;
			} else
				printInvalidArguments(commands[1], commands[0]);

		} else
			printInvalidArguments(commands[2], commands[0]);

	} else
		printInvalidArguments("", commands[0]);
}

void handleCommand_end_all_program(kernel_struct* kernelStruct, char** commands) {
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

	char* patter = "%10s %-5s %-10s\t- %-60s\n";
	printf("\nForma de uso: «command» [«option»] [«args»]\n\n");
	puts("Lista de comandos permitidos:");

	printf(patter,
	COD_CONSOLE_INFO_PROGRAM, OPT_ALL, "",
			"Muestra informacion de todos los procesos");
	printf(patter,
	COD_CONSOLE_INFO_PROGRAM, OPT_STATE, "«state»",
			"Muestra informacion de los procesos correspondientes a «state»");
	printf(patter, "", "", "", "Estados: NEW, READY, EXEC, BLOCK, EXIT.");
	printf(patter,
	COD_CONSOLE_INFO_PROGRAM, OPT_PID, "«pid»",
			"Muestra informacion del programa con «pid»");

	printf(patter,
	COD_CONSOLE_STOP_PROGRAM, OPT_ALL, "",
			"Detiene la ejecución de todos los programas");
	printf(patter,
	COD_CONSOLE_STOP_PROGRAM, OPT_PID, "«pid»",
			"Detiene la ejecución del programa con «pid»");

	printf(patter,
	COD_CONSOLE_SET_NEW_MULTIPROGRAMMING_LEVEL, "", "«new level»",
			"Actualiza el nivel de multiprogramacion a «new level»");

	printf(patter,
	COD_CONSOLE_PLANING, "", "", "Bloquea o Desbloquea el planificador");

	printf(patter, COD_CONSOLE_CLEAR, "", "", "Limpia la pantalla");

	printf(patter, COD_CONSOLE_EXIT, "", "", "Finaliza el proceso");

	unlockPrinter();
}

