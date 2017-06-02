/*
 * cpu-core.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#include "cpu-core.h"

PCB* pcb;
int pageSize;
int stackPageSize = 0;
int errorFlag;
pthread_mutex_t executionMutex;

AnSISOP_funciones ansisop_funtions = { .AnSISOP_definirVariable =
		ansisop_definirVariable, .AnSISOP_obtenerPosicionVariable =
		ansisop_obtenerPosicionVariable, .AnSISOP_dereferenciar =
		ansisop_dereferenciar, .AnSISOP_asignar = ansisop_asignar,
		.AnSISOP_obtenerValorCompartida = ansisop_obtenerValorCompartida,
		.AnSISOP_asignarValorCompartida = ansisop_asignarValorCompartida,
		.AnSISOP_irAlLabel = ansisop_irAlLabel, .AnSISOP_llamarSinRetorno =
				ansisop_llamarSinRetorno, .AnSISOP_llamarConRetorno =
				ansisop_llamarConRetorno,
		.AnSISOP_finalizar = ansisop_finalizar, .AnSISOP_retornar =
				ansisop_retornar, };

AnSISOP_kernel ansisop_kernelFunctions = { .AnSISOP_wait = ansisopKernel_wait,
		.AnSISOP_signal = ansisopKernel_signal, .AnSISOP_reservar =
				ansisopKernel_reservar,
		.AnSISOP_liberar = ansisopKernel_liberar, .AnSISOP_abrir =
				ansisopKernel_abrir, .AnSISOP_borrar = ansisopKernel_borrar,
		.AnSISOP_cerrar = ansisopKernel_cerrar, .AnSISOP_moverCursor =
				ansisopKernel_moverCursor, .AnSISOP_escribir =
				ansisopKernel_escribir, .AnSISOP_leer = ansisopKernel_leer, };

void initExecutionMutex() {
	pthread_mutex_init(&executionMutex, NULL);
}

void destroyExecutionMutex() {
	pthread_mutex_destroy(&executionMutex);
}

PCB* getPCB() {
	return pcb;
}

void loadPCB(PCB* newPCB) {
	pcb = newPCB;
	logTrace("Contexto de proceso cargado PID:%d...", pcb->pid);
}

void unloadPCB() {
	destroy_PBC(pcb);
	pcb = NULL;
	logTrace("Se descargo el pcb");
}

void setStackPageSize(int size) {
	stackPageSize = size;
}

void setErrorFlag(int error) {
	errorFlag = error;
}

int getErrorFlag() {
	return errorFlag;
}

void setPageSize(int value) {
	pageSize = value;
}

bool isFinishedProcess() {
	return pcb->programCounter > pcb->metadata->instrucciones_size;
}

void validateStackOverflow(size_t requiredSize) {
	int maxStackOffset = stackPageSize * pageSize;
	if (pcb->stackOffset + requiredSize >= maxStackOffset) {
		logError("StackOverflow para el pid: %d", pcb->pid);
		errorFlag = FLAG_STACKOVERFLOW;
	}
}

t_stack_index* getCurrentContext() {
	return (pcb->stackIndex != NULL) ? &(pcb->stackIndex[pcb->stackSize]) : NULL;
}

t_puntero logicalAddressToPointer(dir_memoria* dir) {
	t_puntero pointer = 0;
	pointer += (dir->pagina - pcb->stackFirstPage) * pageSize;
	pointer += dir->offset;
	return pointer;
}

dir_memoria* pointerToLogicalAddress(t_puntero pointer) {
	dir_memoria* address = malloc(sizeof(dir_memoria));
	int pageNumber = pointer / pageSize;
	int offset = pointer % pageSize;
	address->pagina = pcb->stackFirstPage + pageNumber;
	address->offset = offset;
	address->size = sizeof(uint32_t);
	return address;
}

/**
 * me pueden invocar en dos momentos
 * 	- cuando tengo un pcb -> tengo que realizar acciones para no perder los datos del pcb
 * 	- cuando no tengo pcb -> finalizo feliz
 */
void finalize_CPU_process() {
	pthread_mutex_lock(&executionMutex);
	logInfo("Finalizando el proceso porque se recibio la señal SIGUSR1");
	exit(0);
	pthread_mutex_unlock(&executionMutex);
}

void ansisopExecuteInstruccion(cpu_struct* cpuStruct) {
	pthread_mutex_lock(&executionMutex);
	errorFlag = FLAG_OK;
	char* instruccion = getNextInstruction(cpuStruct);
	if (errorFlag == FLAG_OK) {
		pcb->programCounter++;
		logTrace("Ansisop instruction - START");
		logTrace("Ejecutando '%s'", instruccion);
		analizadorLinea(instruccion, &ansisop_funtions,
				&ansisop_kernelFunctions);
		logTrace("Ansisop instruction - START");
		free(instruccion);
	}
	if (errorFlag == FLAG_OK) {
		if (isFinishedProcess()) {
			reportEndProcess(cpuStruct);
		} else {
			reportEndInstruction(cpuStruct);
		}
	} else {
		reportExcecutionError(cpuStruct, errorFlag);
	}
	pthread_mutex_unlock(&executionMutex);
}

char* getNextInstruction(cpu_struct* cpuStruct) {
	uint32_t currentPC = pcb->programCounter;
	t_intructions instruction =
			pcb->metadata->instrucciones_serializado[currentPC];
	t_puntero_instruccion offset = instruction.start;
	t_size length = instruction.offset;
	logInfo(
			"Leyendo de memoria la proxima ejecucion a ejecutar pid: %d offset: %d size: %d",
			pcb->pid, offset, length);
	return getDataFromMemory(cpuStruct, 0, offset, length); //0 porque el codigo se almacena en la pag 0 ;)
}

/**
 * Soy una funcion generica que sirve para devolver los datos de la memoria
 * SIN IMPORTAR QUE ESOS DATOS ESTEN PARTIDOS ENTRE DOS O MAS PAGINAS CONTIGUAS
 * ---importante -> hay que pasarle un numero de pagina de inicio -> startPage
 */
char* getDataFromMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset,
		t_size length) {
	char* buffer = malloc(length + 1);
	int bufferOffset = 0;
	char* tmpBuffer;
	int firstByte, lastByte, pageNumber, tmpBufferSize;
	int firstPage = startPage + (offset / pageSize);
	int lastPage = startPage + ((offset + length) / pageSize);
	int firstPageOffset = offset % pageSize;
	int lastPageOffset = (offset + length) % pageSize;
	for (pageNumber = firstPage; errorFlag == FLAG_OK && pageNumber <= lastPage;
			++pageNumber) {
		firstByte = (pageNumber == firstPage) ? firstPageOffset : 0;
		lastByte = (pageNumber == lastPage) ? lastPageOffset : pageSize;
		tmpBufferSize = lastByte - firstByte;
		tmpBuffer = getDataFromPage(cpuStruct, pageNumber, firstByte,
				tmpBufferSize);
		if (errorFlag == FLAG_OK) {
			memcpy(buffer + bufferOffset, tmpBuffer, tmpBufferSize);
			bufferOffset += tmpBufferSize;
			free(tmpBuffer);
		}
	}
	if (errorFlag == FLAG_OK) {
		buffer[length] = '\0'; //lo convierto en string
	} else {
		free(buffer);
		buffer = NULL;
	}
	return buffer;
}

/**
 * Soy una funcion generica que sirve para guardar datos en la memoria
 * SIN IMPORTAR QUE ESOS DATOS ESTEN PARTIDOS ENTRE DOS O MAS PAGINAS CONTIGUAS
 * ---importante -> hay que pasarle un numero de pagina de inicio -> startPage
 */
void saveDataOnMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset,
		t_size length, char* buffer) {
	int bufferOffset = 0;
	char* tmpBuffer;
	int firstByte, lastByte, pageNumber, tmpBufferSize;
	int firstPage = startPage + (offset / pageSize);
	int lastPage = startPage + ((offset + length) / pageSize);
	int firstPageOffset = offset % pageSize;
	int lastPageOffset = (offset + length) % pageSize;
	for (pageNumber = firstPage; errorFlag == FLAG_OK && pageNumber <= lastPage;
			++pageNumber) {
		firstByte = (pageNumber == firstPage) ? firstPageOffset : 0;
		lastByte = (pageNumber == lastPage) ? lastPageOffset : pageSize;
		tmpBufferSize = lastByte - firstByte;
		tmpBuffer = malloc(tmpBufferSize);
		memcpy(tmpBuffer, buffer + bufferOffset, tmpBufferSize);
		saveDataOnPage(cpuStruct, pageNumber, firstByte, tmpBufferSize,
				tmpBuffer);
		if (errorFlag == FLAG_OK) {
			bufferOffset += tmpBufferSize;
		}
		free(tmpBuffer);
	}
}

t_variable* createVariableForCurrentPCB(t_nombre_variable name) {
	t_stack_index* currentContext = getCurrentContext();
	currentContext->vars = realloc(currentContext->vars,
			sizeof(t_variable) * (currentContext->var_len + 1));
	currentContext->vars[currentContext->var_len].nombre = name;
	//TODO verificar la correcta asignacion
	currentContext->vars[currentContext->var_len].direccion.pagina =
			pcb->stackFirstPage + (pcb->stackOffset / pageSize);
	//TODO verificar la correcta asignacion
	currentContext->vars[currentContext->var_len].direccion.offset =
			pcb->stackOffset % pageSize;
	currentContext->vars[currentContext->var_len].direccion.size =
			sizeof(uint32_t);
	currentContext->var_len++;
	pcb->stackOffset += sizeof(uint32_t);
	return &(currentContext->vars[currentContext->var_len - 1]);
}

t_variable* createArgumentForCurrentPCB(t_nombre_variable name) {
	t_stack_index* currentContext = getCurrentContext();
	currentContext->args = realloc(currentContext->args,
			sizeof(t_variable) * (currentContext->arg_len + 1));
	currentContext->args[currentContext->arg_len].nombre = name;
	//TODO verificar la correcta asignacion
	currentContext->args[currentContext->arg_len].direccion.pagina =
			pcb->stackFirstPage + (pcb->stackOffset / pageSize);
	//TODO verificar la correcta asignacion
	currentContext->args[currentContext->arg_len].direccion.offset =
			pcb->stackOffset % pageSize;
	currentContext->args[currentContext->arg_len].direccion.size =
			sizeof(uint32_t);
	currentContext->arg_len++;
	pcb->stackOffset += sizeof(uint32_t);
	return &(currentContext->args[currentContext->arg_len - 1]);
}

t_variable* getArgument(t_nombre_variable name) {
	t_stack_index* currentContext = getCurrentContext();
	int i;
	for (i = 0; i < currentContext->arg_len; i++) {
		if (currentContext->args[i].nombre == name)
			return &(currentContext->args[i]);
	}
	return NULL;
}

t_variable* getVariable(t_nombre_variable name) {
	t_stack_index* currentContext = getCurrentContext();
	int i;
	for (i = 0; i < currentContext->var_len; i++) {
		if (currentContext->vars[i].nombre == name)
			return &(currentContext->vars[i]);
	}
	return NULL;
}

t_puntero_instruccion getProgramCounterByLabel(t_nombre_etiqueta label) {
	char* labels = pcb->metadata->etiquetas;
	size_t size = pcb->metadata->etiquetas_size;
	int offset = 0;
	t_puntero_instruccion newProgramCounter = NULL_VALUE;
	if (label[strlen(label) - 1] == '\n') {
		label[strlen(label) - 1] = '\0';
	}
	while (offset < size && strcmp(labels + offset, label) != 0) {
		offset += strlen(labels + offset) + 1 + sizeof(t_puntero_instruccion);
	}
	if (offset < size) {
		memcpy(&newProgramCounter,
				labels + offset + strlen(labels + offset) + 1,
				sizeof(t_puntero_instruccion));
	}
	return newProgramCounter;
}
