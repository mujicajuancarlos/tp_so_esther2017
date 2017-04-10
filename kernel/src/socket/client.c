/*
 * client.c
 *
 *  Created on: 20/4/2016
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include "client.h"

Package* createPackage(){
	Package* tmp;

	tmp=malloc(sizeof(Package));
	tmp->message=NULL;

	return tmp;
}

void destroyPackage(Package* package){
	if(package!=NULL){
		free(package->message);
	}
	free(package);
}
