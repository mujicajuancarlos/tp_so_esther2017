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
		printHeaderProcess();
		void wapperPrintAllProcess(int stateIndex, t_list* processList) {
			void printElement(void* element) {
				Process* proces = element;
				printProcess(proces, stateIndex);
			}
			list_iterate(processList, printElement);
		}
		wapperPrintAllProcess(STATE_CODE_NEW, states->new);
		wapperPrintAllProcess(STATE_CODE_READY, states->ready->elements);
		wapperPrintAllProcess(STATE_CODE_EXECUTE, states->execute);
		wapperPrintAllProcess(STATE_CODE_BLOCK, states->block);
		wapperPrintAllProcess(STATE_CODE_EXIT, states->exit->elements);
	} else
		printInvalidArguments(commands[2], commands[0]);
}

void handleCommand_info_by_state_process(kernel_struct* kernelStruct,
		char** commands) {
	if (commands[2] != NULL) {
		if (commands[3] == NULL) {
			char* state = commands[2];
			int stateIndex = stringToStateIndex(state);
			t_planningStates* states = getStates();
			printHeaderProcess();
			void printElement(void* element) {
				Process* proces = element;
				printProcess(proces, stateIndex);
			}
			switch (stateIndex) {
			case STATE_CODE_NEW:
				list_iterate(states->new, printElement);
				break;
			case STATE_CODE_READY:
				list_iterate(states->ready->elements, printElement);
				break;
			case STATE_CODE_EXECUTE:
				list_iterate(states->execute, printElement);
				break;
			case STATE_CODE_BLOCK:
				list_iterate(states->block, printElement);
				break;
			case STATE_CODE_EXIT:
				list_iterate(states->exit->elements, printElement);
				break;
			default:
				printInvalidArguments(state, commands[0]);
				break;
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
			int stateIndex;
			Process* process = getProcessAndStateIndexByPID(pid, &stateIndex);
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
	if (commands[2] == NULL) {

		void wrapperToForceQuitAllProcess(int stateIndex, t_list* processList) {
			void moveExit(void* element) {
				Process* proces = element;
				basicForceQuitProcess(proces, stateIndex);
			}
			list_iterate(processList, moveExit);
		}

		wrapperToForceQuitAllProcess(STATE_CODE_NEW, getStates()->new);
		wrapperToForceQuitAllProcess(STATE_CODE_READY,
				getStates()->ready->elements);
		wrapperToForceQuitAllProcess(STATE_CODE_BLOCK, getStates()->block);
		wrapperToForceQuitAllProcess(STATE_CODE_EXECUTE, getStates()->execute);
		wrapperToForceQuitAllProcess(STATE_CODE_EXIT,
				getStates()->exit->elements);

		/*pthread_mutex_lock(&(kernelStruct->stdoutMutex));
		 state = list_map(state, basicForceQuitProcess(process,state));
		 pthread_mutex_unlock(&(kernelStruct->stdoutMutex));*/
	} else
		printInvalidArguments(commands[2], commands[0]);
} //recorro toda lista (new, ready..) y por cada lista ejecuto Funcion generica

void handleCommand_end_by_pid_program(kernel_struct* kernelStruct,
		char** commands) {
	if (commands[2] != NULL && commands[3] == NULL) {
		int pid = atoi(commands[2]);
		int stateIndex;
		Process* process = getProcessAndStateIndexByPID(pid, &stateIndex);
		if (process != NULL) {
			basicForceQuitProcess(process, stateIndex);
		} else {
			printPidNotFound(pid);
		}
		/*
		 pthread_mutex_lock(&(kernelStruct->programsListMutex));
		 Program* program = list_find(kernelStruct->listaProgramas, condicion);
		 pthread_mutex_unlock(&(kernelStruct->programsListMutex));
		 */
	} else {
		printInvalidArguments("", commands[0]);
	}
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

