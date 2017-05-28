/*
 * cpu-core.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#include "cpu-core.h"

PCB* pcb;
int pageSize;
int errorFlag;
AnSISOP_funciones ansisop_funtions;
AnSISOP_kernel ansisop_kernelFunctions;

PCB* getPCB(){
	return pcb;
}

void setErrorFlag(int error){
	errorFlag = error;
}

int getErrorFlag(){
	return errorFlag;
}

void setPageSize(int value){
	pageSize = value;
}

void loadPCB(PCB* newPCB) {
	pcb = newPCB;
	logTrace("Contexto de proceso cargado PID:%d...", pcb->pid);
}

void ansisopExecuteInstruccion(cpu_struct* cpuStruct) {
	errorFlag = FLAG_OK;
	char* instruccion = getNextInstruction(cpuStruct);
	pcb->programCounter++;

	logTrace("Ansisop instruction - START");
	logTrace("Ejecutando '%s'", instruccion);
	analizadorLinea(instruccion, &ansisop_funtions, &ansisop_kernelFunctions);
	logTrace("Ansisop instruction - START");

	if (instruccion != NULL)
		free(instruccion);
}

void unloadPCB(cpu_struct* cpuStruct) {
	char* buffer = serialize_PCB(pcb);
	uint32_t size = sizeOf_PCB(pcb);
	Package* package = createAndSendPackage(cpuStruct->socketClientKernel,
	COD_CONTEXT_SWITCH_RESPONSE, size, buffer);
	logTrace("Se envio en PCB del pid: %d actualizado al kernel fd:%d",
			pcb->pid, cpuStruct->socketClientKernel);
	destroyPackage(package);
	destroy_PBC(pcb);
	logTrace("Se descargo el pcb");
}

char* getNextInstruction(cpu_struct* cpuStruct) {
	uint32_t currentPC = pcb->programCounter;
	t_intructions instruction =
			pcb->metadata->instrucciones_serializado[currentPC];
	t_puntero_instruccion offset = instruction.start;
	t_size length = instruction.offset;
	return getDataFromMemory(cpuStruct, 0, offset, length);//0 porque el codigo se almacena en la pag 0 ;)
}

/**
 * Soy una funcion generica que sirve para devolver los datos de la memoria
 * SIN IMPORTAR QUE ESOS DATOS ESTEN PARTIDOS ENTRE DOS O MAS PAGINAS CONTIGUAS
 * ---importante -> hay que pasarle un numero de pagina de inicio -> startPage
 */
char* getDataFromMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset, t_size length) {
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
		tmpBuffer = getDataFromPage(cpuStruct, pageNumber, firstByte, tmpBufferSize);
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
void saveDataOnMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset, t_size length, char* buffer) {
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
		saveDataOnPage(cpuStruct, pageNumber, firstByte, tmpBufferSize, tmpBuffer);
		if (errorFlag == FLAG_OK) {
			bufferOffset += tmpBufferSize;
		}
		free(tmpBuffer);
	}
}
