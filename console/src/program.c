/*
 * program.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include "program.h"

void printHeaderProgram(){
	printf("\nPID\tC.IMPR.\tFECHA INICIO\tHORA INICIO\tFECHA FIN\tHORA FIN\tT.EJECUTADO\tCODIGO FUENTE\n");
	printf("======================================================================================================\n");
}

void printProgram(Program* program){
	int size = 20;
	char* none = "-";
	char* dateFormat = "%Y/%m/%d";
	char* timeFormat = "%H:%M:%S";
	char* buffer = malloc(sizeof(char) * size);
	//print pid
	if (program->pid != -1)
		printf("%d\t",program->pid);
	else
		printf ("%s\t", none);
	//print cant. imporesiones
	printf("%d\t",program->printSize);
	//print start date
	strftime (buffer, size, dateFormat, localtime(&(program->startDate)));
	printf ("%s\t", buffer);
	//print start time
	strftime (buffer, size, timeFormat, localtime(&(program->startDate)));
	printf ("%s\t", buffer);
	//print end and internal date
	if(program->endDate != 0){
		strftime (buffer, size, dateFormat, localtime(&(program->endDate)));
		printf ("%s\t", buffer);
		strftime (buffer, size, timeFormat, localtime(&(program->endDate)));
		printf ("%s\t", buffer);
		printIntervalTime(&(program->startDate), &(program->endDate));
	}else{
		printf ("%s\t", none);
		printf ("%s\t", none);
		printf ("%s\t", none);
	}
	//print file
	printf ("%s\n", program->sourceCodePath);
	printf("\n");
	free(buffer);
}

void printIntervalTime(time_t* startTime, time_t* endTime) {
	const int ONE_HOUR = 60 * 60;
	const int ONE_MINUE = 60;
	double diff_t = difftime(*endTime, *startTime);
	int hour, min, sec;
	hour=diff_t/ONE_HOUR;
	diff_t-=hour*ONE_HOUR;
	min=diff_t/ONE_MINUE;
	diff_t-=min*ONE_HOUR;
	sec=diff_t;
	printf("%.2i:%.2i:%.2i \t",hour,min,sec);
}

Program* createNewProgram(console_struct* console, char* sourceCodePath) {
	Program *program = malloc(sizeof(Program));
	program->startDate = time(NULL);
	program->endDate = 0;
	program->fd_client = -1;
	program->pid = -1;
	program->printSize = 0;
	program->sourceCodePath = string_duplicate(sourceCodePath);
	program->console = console;
	return program;
}

void destroyProgram(Program* program) {
	if (program != NULL) {
		free(program->sourceCodePath);
	}
	free(program);
}
