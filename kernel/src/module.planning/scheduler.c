/*
 * scheduler.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#include "scheduler.h"


void initializeScheduler(kernel_struct* kernelStruct){
	initializeProcessLifeCycle();
	initializeMultiprogrammingLevel(kernelStruct);
	initializeshortTermScheduler();
}

void destroyScheduler(){
	destroyProcessLifeCycle();
	destroyMultiprogrammingLevel();
	destroyshortTermScheduler();
}
