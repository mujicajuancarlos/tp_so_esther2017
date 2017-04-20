/*
 * handler-user.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(console_struct* args) {

	int exit = 0;
	bool shouldCompareCommand;
	while (exit == 0) {
		printf(" > ");
		char** commands = get_user_commands(200);
		shouldCompareCommand = true;

		if (equal_user_command(commands[0], COD_HELP, &shouldCompareCommand)) {
			printCommandsHelp();
		}

		if (equal_user_command(commands[0], COD_RESET, &shouldCompareCommand)) {
			puts("reseteo");
		}

		if (equal_user_command(commands[0], COD_EXIT, &shouldCompareCommand)) {
			exit = 1;
		}

		if (equal_user_command(commands[0], COD_CLEAR, &shouldCompareCommand)) {
			clearConsoleScreen();
		}

		if (equal_user_command(commands[0], COD_START_PROGRAM,
				&shouldCompareCommand)) {
			logInfo("Creando el hilo para ejecutar un programa");
			//pthread_t hiloPrograma;
			//pthread_create(&hiloPrograma, NULL, (void*) handleProgram, &args);
		}

		if (equal_user_command(commands[0], COD_INFO_PROGRAM,
				&shouldCompareCommand)) {
			puts("info");
		}

		if (equal_user_command(commands[0], COD_STOP_PROGRAM,
				&shouldCompareCommand)) {
			puts("stop");
		}

		if (shouldCompareCommand) {
			printf("«%s» no es un comando válido. Si necesita ayuda use: «%s»\n",commands[0],COD_HELP);
		}

	}
}

void clearConsoleScreen() {
	system("clear");
}

void printCommandsHelp() {
	printf("\nForma de uso: «command» [«option»] [«args»]\n\n");
	puts("Lista de comandos permitidos:");
	printf("\t%s\t\t\tRestablece el proceso por completo\n", COD_RESET);
	printf("\t%s\t\t\tFinaliza el proceso\n", COD_EXIT);
	printf("\t%s\t\t\tLimpia la pantalla\n", COD_CLEAR);
	printf("\t%s «path»\t\tInicia el programa ansisop descrito en «path»\n", COD_START_PROGRAM);

	printf("\t%s %s\t\tMuestra informacion de todos los programas en ejecución\n", COD_INFO_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tMuestra informacion del programa con «pid»\n", COD_INFO_PROGRAM, OPT_PID);

	printf("\t%s %s\t\tDetiene la ejecución de todos los programas\n", COD_STOP_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tDetiene la ejecución del programa con «pid»\n", COD_STOP_PROGRAM, OPT_PID);
}

void printWelcome() {
	puts("_____________________________________________________");
	puts("_________ Bienvenido a Esther - Death Coders ________");
	puts("________________ UTN-FRBA SO 1C-2017 ________________");
	puts("_____________________________________________________");
	puts("");
}
