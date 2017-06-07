/*
 * processLifeCycle.c
 *
 *  Created on: 5/5/2017
 *      Author: utnso
 */

#include "processLifeCycle.h"

pthread_mutex_t executeListMutex;
pthread_mutex_t exitListMutex;
pthread_mutex_t newListMutex;
pthread_mutex_t readyListMutex;
pthread_mutex_t blockListMutex;
pthread_mutex_t blockQueuesMutex;

t_planningStates* states;
t_log* logger;
t_dictionary* blockQueues;

void moveFromNewToReady(Process* process) {
	removeFromNEW(process);
	sendToREADY(process);
}

void moveFromExcecToReady(Process* process) {
	removeFromEXEC(process);
	sendToREADY(process);
}

void moveFromExcecToExit(Process* process) {
	removeFromEXEC(process);
	sendToEXEC(process);
	process->exit_code = SC_SUCCESS;
	_incrementMultiprogrammingLevel();
}

void moveFromExcecToBlock(Process* process) {
	removeFromEXEC(process);
	sendToBLOCK(process);
}

void moveFromBlockToReady(Process* process){
	removeFromBLOCK(process);
	sendToREADY(process);
}

void sendToREADY(Process* process) {
	pthread_mutex_lock(&readyListMutex);
	queue_push(states->ready, process);
	processInReady_signal();
	pthread_mutex_unlock(&readyListMutex);
	logTrace("El proceso %d ingresó a la lista de READY", process->pid);
}

Process* popToREADY() {
	Process* process;
	processInReady_wait();
	pthread_mutex_lock(&readyListMutex);
	process = queue_pop(states->ready);
	pthread_mutex_unlock(&readyListMutex);
	logTrace("El proceso %d salio a la lista de READY", process->pid);
	return process;
}

void sendToEXEC(Process* process) {
	pthread_mutex_lock(&executeListMutex);
	list_add(states->execute, process);
	pthread_mutex_unlock(&executeListMutex);
	logTrace("El proceso %d ingresó a la lista de EXECUTE", process->pid);
}

void removeFromEXEC(Process* process) {
	bool condition(void* element) {
		Process* anyProcess = element;
		return anyProcess->pid == process->pid;
	}
	pthread_mutex_lock(&executeListMutex);
	list_remove_by_condition(states->execute, condition);
	pthread_mutex_unlock(&executeListMutex);
	logTrace("El proceso %d salio de la lista de EXECUTE", process->pid);
}

void sendToBLOCK(Process* process) {
	pthread_mutex_lock(&blockListMutex);
	list_add(states->block, process);
	pthread_mutex_unlock(&blockListMutex);
	logTrace("El proceso %d ingresó a la lista de BLOCK", process->pid);
}

void removeFromBLOCK(Process* process) {
	bool condition(void* element) {
		Process* anyProcess = element;
		return anyProcess->pid == process->pid;
	}
	pthread_mutex_lock(&blockListMutex);
	list_remove_by_condition(states->block, condition);
	pthread_mutex_unlock(&blockListMutex);
	logTrace("El proceso %d salio de la lista de BLOCK", process->pid);
}

void initializeQueueBySemaphore(char** semKeys) {
	blockQueues = dictionary_create();
	int index = 0;
	while (semKeys[index] != NULL) {
		t_queue* value = queue_create();
		dictionary_put(blockQueues, semKeys[index], value);
		index++;
	}
}

void notifyUnlockedProcessFor(char* semKey) {
	t_queue* value;
	Process* process;
	pthread_mutex_lock(&blockQueuesMutex);
	if (dictionary_has_key(blockQueues, semKey)) {
		value = dictionary_get(blockQueues, semKey);
		process = queue_pop(value);
		if(process != NULL){
			moveFromBlockToReady(process);
		}
	} else {
		logError(
				"No se encontro la cola de bloqueados para: %s",
				semKey);
		exit(-1);
	}
	pthread_mutex_unlock(&blockQueuesMutex);
}

void notifyLockedProcessFor(char* semKey, Process* process) {
	t_queue* value;
	pthread_mutex_lock(&blockQueuesMutex);
	if (dictionary_has_key(blockQueues, semKey)) {
		value = dictionary_get(blockQueues, semKey);
		queue_push(value,process);
		moveFromExcecToBlock(process);
	} else {
		logError(
				"No se encontro la cola de bloqueados para: %s",
				semKey);
		exit(-1);
	}
	pthread_mutex_unlock(&blockQueuesMutex);
}

void sendToEXIT(Process* process) {
	pthread_mutex_lock(&exitListMutex);
	queue_push(states->exit, process);
	pthread_mutex_unlock(&exitListMutex);
	logTrace("El proceso %d ingresó a la lista de EXIT", process->pid);
}

Process* popToEXIT() {
	Process* process;
	pthread_mutex_lock(&exitListMutex);
	process = queue_pop(states->exit);
	pthread_mutex_unlock(&exitListMutex);
	logTrace("El proceso %d salio a la lista de EXIT", process->pid);
	return process;
}

void sendToNEW(Process* process) {
	pthread_mutex_lock(&newListMutex);
	list_add(states->new, process);
	pthread_mutex_unlock(&newListMutex);
	logTrace("El proceso %d ingresó a la lista de NEW", process->pid);
}

void removeFromNEW(Process* process) {
	bool condition(void* element) {
		Process* anyProcess = element;
		return anyProcess->pid == process->pid;
	}
	pthread_mutex_lock(&newListMutex);
	list_remove_by_condition(states->new, condition);
	pthread_mutex_unlock(&newListMutex);
	logTrace("El proceso %d salio de la lista de EXECUTE", process->pid);
}

t_planningStates* getStates() {
	return states;
}

void initializeProcessLifeCycle() {

	logTrace("Inicializando t_planningStates del Planificador");
	states = malloc(sizeof(t_planningStates));

	states->execute = list_create();
	states->exit = queue_create();
	states->new = list_create();
	states->ready = queue_create();
	states->block = queue_create();

	pthread_mutex_init(&executeListMutex, NULL);
	pthread_mutex_init(&exitListMutex, NULL);
	pthread_mutex_init(&newListMutex, NULL);
	pthread_mutex_init(&readyListMutex, NULL);
	pthread_mutex_init(&blockListMutex, NULL);
	pthread_mutex_init(&blockQueuesMutex,NULL);
}

void destroyProcessLifeCycle() {

	list_destroy_and_destroy_elements(states->execute, (void*) destroyProcess);
	queue_destroy_and_destroy_elements(states->exit, (void*) destroyProcess);
	list_destroy_and_destroy_elements(states->new, (void*) destroyProcess);
	queue_destroy_and_destroy_elements(states->ready, (void*) destroyProcess);
	queue_destroy_and_destroy_elements(states->block, (void*) destroyProcess);

	pthread_mutex_destroy(&executeListMutex);
	pthread_mutex_destroy(&exitListMutex);
	pthread_mutex_destroy(&newListMutex);
	pthread_mutex_destroy(&readyListMutex);
	pthread_mutex_destroy(&blockListMutex);
	pthread_mutex_destroy(&blockQueuesMutex);
}

/*  dependiendo el estado se puede realizar diferentes acciones
 new - como no ingreso en el sistema solo hay q moverlo de new a exit
 ready - ya tiene asignado recursos por lo tanto hay que eliminarlos y luego moverlo
 block - lo mismo que ready
 exec - lo mismo que ready (tener en cuenta que si esta ejecutando
 hay que esperar que la cpu termine de ejecutar la instruccion)
 exit - loguear el error porque no seria una inconsitencia enviar a finalizar un proceso que ya finalizo    */

void endProcessGeneric(Process* process) {
	/*char* state = getProcessState(process);
	bool shouldCompareState = true;

	if (shouldCompareState && string_equals_ignore_case(state, "new")) {
		shouldCompareState = false;
		Package* package;
		package = createAndSendPackage(process->fileDescriptor,
				COD_FORCE_QUIT_PROGRAM, 0, NULL);
		destroyPackage(package);
		sendToEXIT(process);
	}

	if (shouldCompareState && string_equals_ignore_case(state, "ready")) {
		shouldCompareState = false;
		_incrementMultiprogrammingLevel();
		processInReady_wait();
		Package* package;
		package = createAndSendPackage(process->fileDescriptor,
				COD_FORCE_QUIT_PROGRAM, 0, NULL);
		destroyPackage(package);
		sendToEXIT(process);
		close(process->fileDescriptor);
		destroyProcess(process);
		//liberar memoria
		//liberar filesystem

		mostrar mensaje :
		 ->si viene desde consola -> finalizo con exito
		 ->si cerro por la consola kernel -> el programa finalizado por el administrador del sistema
		 ->si cerro por error -> se podria informar el tipo de error


	}
	if (shouldCompareState && string_equals_ignore_case(state, "execute")) {

		shouldCompareState = false;
		_incrementMultiprogrammingLevel();
		Package* package;
		package = createAndSendPackage(process->fileDescriptor,
				COD_FORCE_QUIT_PROGRAM, 0, NULL);
		destroyPackage(package);
		sendToEXIT(process);
		close(process->fileDescriptor);
		destroyProcess(process);
		//liberar memoria
		//liberar filesystem

	}
	if (shouldCompareState && string_equals_ignore_case(state, "block")) {
		shouldCompareState = false;
		_incrementMultiprogrammingLevel();
		Package* package;
		package = createAndSendPackage(process->fileDescriptor,
				COD_FORCE_QUIT_PROGRAM, 0, NULL);
		destroyPackage(package);
		sendToEXIT(process);
		close(process->fileDescriptor);
		destroyProcess(process);
		//liberar memoria
		//liberar filesystem
	}
	if (shouldCompareState && string_equals_ignore_case(state, "exit")) {
		shouldCompareState = false;
		log_error(logger,
				"Error, no puedo enviar un proceso a finalizar el cual ya finalizo");
		//printf("Error, no puedo enviar un proceso a finalizar el cual ya finalizo");
	}*/

}

