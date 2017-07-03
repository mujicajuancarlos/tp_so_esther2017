/*
 * program.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include "program.h"

bool pid_isEqual(Program* program, int pid) {
	return program->pid == pid;
}

Program* createNewProgram(console_struct* console, char* sourceCodePath) {
	Program *program = malloc(sizeof(Program));
	program->startDate = time(NULL);
	program->endDate = 0;
	program->fd_client = -1;
	program->pid = -1;
	program->printSize = 0;
	program->exit = false;
	program->sourceCodePath = string_duplicate(sourceCodePath);
	program->console = console;
	return program;
}

void destroyProgram(Program* program) {
	if (program != NULL) {
		if(program->sourceCodePath!=NULL){
			free(program->sourceCodePath);
		}
		free(program);
		program = NULL;
	}
}

void printProgramStatus(Program* program, char* statusMessage) {
	lockPrinter();
	printf("\n%s:\n\n", statusMessage);
	printHeaderProgram();
	printProgram(program);
	unlockPrinter();
	printNewLine();
}

void printHeaderProgram() {
	printf("\n%5s\t%15s\t%20s\t%20s\t%15s\t\t%-20s\n%s\n", "PID", "IMPRESIONES", "INICIO",
			"FIN", "T.EJECUTADO", "CODIGO FUENTE",
			"==============================================================================================================");
}

void printProgram(Program* program) {
	int size = 20;
	char* none = "-";
	char* dateFormat = "%Y/%m/%d %H:%M:%S";
	char* buffer = malloc(sizeof(char) * size);
//print pid
	if (program->pid != -1)
		printf("%5d\t", program->pid);
	else
		printf("%5s\t", none);
//print cant. imporesiones
	printf("%15d\t", program->printSize);
//print start date
	strftime(buffer, size, dateFormat, localtime(&(program->startDate)));
	printf("%20s\t", buffer);
//print end and internal date
	if (program->endDate != 0) {
		strftime(buffer, size, dateFormat, localtime(&(program->endDate)));
		printf("%20s\t", buffer);
		printIntervalTime(&(program->startDate), &(program->endDate));
	} else {
		printf("%20s\t", none);
		printf("%15s\t", none);
	}
//print file
	printf("\t%-20s\n", program->sourceCodePath);
	free(buffer);
}

void printIntervalTime(time_t* startTime, time_t* endTime) {
	const int ONE_HOUR = 60 * 60;
	const int ONE_MINUE = 60;
	double diff_t = difftime(*endTime, *startTime);
	int hour, min, sec;
	hour = diff_t / ONE_HOUR;
	diff_t -= hour * ONE_HOUR;
	min = diff_t / ONE_MINUE;
	diff_t -= min * ONE_HOUR;
	sec = diff_t;
	printf("%6s %2.2i:%2.2i:%2.2i\t", "", hour, min, sec);

}
