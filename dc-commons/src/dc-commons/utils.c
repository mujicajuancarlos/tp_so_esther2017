/*
 * utils.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "utils.h"


char** get_user_commands(int max_buffer){
	char* buffer = malloc(sizeof(char) * max_buffer);
	fgets(buffer,max_buffer,stdin);
	int length = string_length(buffer);
	if(length > 0 && buffer[--length] == '\n')
		buffer[length] = '\0';
	char** commands = string_split(buffer," ");
	free(buffer);
	return commands;
}

bool equal_user_command(char* userCommand, char* expectedCommand, bool *shouldCompareCommand){
	if(*shouldCompareCommand){
		if(string_equals_ignore_case(userCommand, expectedCommand)){
			*shouldCompareCommand = false;
			return true;
		}
	}
	return false;
}
