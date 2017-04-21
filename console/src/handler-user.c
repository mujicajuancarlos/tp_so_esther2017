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
			puts("exit");
			exit = 1;
		}

		if (equal_user_command(commands[0], COD_CLEAR, &shouldCompareCommand)) {
			system("clear");
		}

		if (equal_user_command(commands[0], COD_START_PROGRAM, &shouldCompareCommand)) {
			handleCommand_start_program(args, commands, &shouldCompareCommand);
		}

		if (equal_user_command(commands[0], COD_INFO_PROGRAM,
				&shouldCompareCommand)) {
			puts("info");
		}

		if (equal_user_command(commands[0], COD_STOP_PROGRAM, &shouldCompareCommand)) {
			puts("stop");
		}

		if (shouldCompareCommand) {
			printf("«%s» no es un comando válido. Si necesita ayuda use: «%s»\n",commands[0],COD_HELP);
		}

	}
}

void handleCommand_start_program(console_struct* console, char** commands, bool *shouldCompareCommand){
	if(commands[1]!=NULL && commands[2]==NULL){
		if(file_exists(commands[1],"r")){
			logInfo("Creando el hilo para ejecutar un programa");
			pthread_t hiloPrograma;
			logInfo("Creando el nuevo programa");
			Program* program = createNewProgram(console, &hiloPrograma, commands[1]);
			pthread_create(&hiloPrograma, NULL, (void*)handleNewProgram, &program);
		} else
			printFileNotFound(commands[1]);
	}else
		printInvalidArguments(commands[0]);
}

void printFileNotFound(char* filePath){
	printf("No se pudo acceder al archivo «%s».\n",filePath);
}

void printInvalidArguments(char* command){
	printf("Los argumentos ingresados no son validos para el comando «%s». Si necesita ayuda use: «%s»\n", command, COD_HELP);
}

void printInvalidOptions(char* command){
	printf("La opcion ingresada no es valida para el comando «%s». Si necesita ayuda use: «%s»\n", command, COD_HELP);
}

void printCommandsHelp() {
	printf("\nForma de uso: «command» [«option»] [«args»]\n\n");
	puts("Lista de comandos permitidos:");

	printf("\t%s «path»\t\tInicia el programa ansisop descrito en «path»\n", COD_START_PROGRAM);

	printf("\t%s %s\t\tMuestra informacion de todos los programas en ejecución\n", COD_INFO_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tMuestra informacion del programa con «pid»\n", COD_INFO_PROGRAM, OPT_PID);

	printf("\t%s %s\t\tDetiene la ejecución de todos los programas\n", COD_STOP_PROGRAM, OPT_ALL);
	printf("\t%s %s «pid»\t\tDetiene la ejecución del programa con «pid»\n", COD_STOP_PROGRAM, OPT_PID);

	printf("\t%s\t\t\tLimpia la pantalla\n", COD_CLEAR);
	printf("\t%s\t\t\tRestablece el proceso por completo\n", COD_RESET);
	printf("\t%s\t\t\tFinaliza el proceso\n", COD_EXIT);
}

void printWelcome() {
	puts("_____________________________________________________");
	puts("_________ Bienvenido a Esther - Death Coders ________");
	puts("________________ UTN-FRBA SO 1C-2017 ________________");
	puts("_____________________________________________________");
	puts("");
}
