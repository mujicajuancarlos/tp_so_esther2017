/*
 * ansisopProcess.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "process.h"

int currentPid = FIRST_PID;
pthread_mutex_t currentPidMutex;

Process* createProcess(int socket, kernel_struct* kernelStruct) {
	Process* newProcess = malloc(sizeof(Process));
	newProcess->fileDescriptor = socket;
	newProcess->pid = 0;
	newProcess->pcb = NULL;
	newProcess->forceQuit = false;
	newProcess->heapPages = list_create();
	//tablaProcesosFD = list_create();
	newProcess->kernelStruct = kernelStruct;
	return newProcess;
}

void destroyProcess(Process* process) {
	if (process != NULL) {
		if (process->pcb != NULL)
			destroy_PBC(process->pcb);
		if (process->heapPages != NULL) {
			list_destroy_and_destroy_elements(process->heapPages,
					(void*) destroy_heap_page);
			process->heapPages = NULL;
		}
		if (process->fileDescriptorList != NULL) {
			list_destroy_and_destroy_elements(process->fileDescriptorList,
					(void*) destroy_t_processFileDescriptor);
			process->fileDescriptorList = NULL;
		}
		free(process);
	}
}

void freeProcessResources(Process* process) {
	process->fileDescriptor = -1;
	destroy_stackArray(process->pcb->stackIndex, process->pcb->stackSize);
	metadata_destruir(process->pcb->metadata);
	process->pcb->stackIndex = NULL;
	process->pcb->metadata = NULL;
	//todo: enviar solicitud a la memoria para que libere las paginas -> considerar tmb heaps
	//todo: enviar solicitud al file system para que libere los archivos
}

void initializeCurrentPidMutex() {
	pthread_mutex_init(&currentPidMutex, NULL);
}

void destroyCurrentPidMutex() {
	pthread_mutex_destroy(&currentPidMutex);
}

int getNextPID() {
	int next;
	pthread_mutex_lock(&currentPidMutex);
	next = currentPid++;
	pthread_mutex_unlock(&currentPidMutex);
	return next;
}

void markForcedQuitProcess(Process* process) {
	process->forceQuit = true;
}

void createPcbForNewProcess(Process* process, Package* sourceCodePackage) {

	logInfo("Generando la metadata para el proceso %d", process->pid);
	t_metadata_program* metadata = metadata_desde_literal(
			sourceCodePackage->stream);

	//el va despues del codigo, por lo tanto con esta cuenta puedo saber cual es la primer pagina del stack
	uint32_t stackFirstPage = sourceCodePackage->size
			/ process->kernelStruct->pageSize;

	logInfo("Generando el pcb para el proceso %d", process->pid);
	PCB* pcb = create_new_PCB(process->pid, stackFirstPage, metadata);

	logInfo("Pcb asociado al proceso de pid %d", process->pid);
	process->pcb = pcb;
}

void replacePCB(Process* process, PCB* newPCB) {
	PCB* oldPcb = process->pcb;
	destroy_PBC(oldPcb);
	process->pcb = newPCB;
}

void printHeaderProcess() {
	printf("\n%5s\t%20s\t%20s\n%s\n", "PID", "STATE", "EXIT CODE",
			"===============================================================");
}

void printProcess(Process* proceso, int stateIndex) {
	char* exitCode =
			(proceso->pcb == NULL) ? "" : string_itoa(proceso->pcb->exit_code);
	char* state = stateIndexToString(stateIndex);
	printf("%5d\t%20s\t%20s\n", proceso->pid, state, exitCode);
}

void printProcessFull(Process* proceso, int stateIndex) {
	printf("\nInformación del proceso %d\n", proceso->pid);
	printf("\nEstado %s", stateIndexToString(stateIndex));
	printf("\tCantidad rafagas ejecutadas %d\n", 567);//TODO: completar rafagas ejecutadas
	printf("\tCantidad de operaciones privilegiadas %d\n", 142);//TODO: completar
	printf("\tTabla de archivos abiertos %d\n", 423);	//TODO: completar
	printf("\tCantidad de paginas heap utilizadas %d\n", 423);//TODO: completar
	printf("\tOtra cantidad %d\n", 432);	//TODO: completar
	printf("\tOtra cantidad %d\n", 234);	//TODO: completar
	printf("\tCantidad de syscall ejecutadas %d\n", 432);	//TODO: completar
}

t_processFileDescriptor* createNew_t_processFileDescriptor(char* permiso,
		t_fileDescriptor* fd) {

	t_processFileDescriptor* newPFD = malloc(sizeof(t_processFileDescriptor));

	newPFD->fileDescriptor = fd;
	newPFD->flag = habilitarPermisos(permiso);
	newPFD->offset = 0;

	return newPFD;

}

void destroy_t_processFileDescriptor(t_processFileDescriptor* pfd) {
	destroy_t_filedescriptor(pfd->fileDescriptor);
	free(pfd);
}

flags* habilitarPermisos(char* permiso) {
	flags* flag = malloc(sizeof(flags));
	int retorno = -1;
	retorno = strcmp(permiso, READ);
	if (retorno == 0) {
		flag->read = true;
		flag->write = false;
		//logInfo("Los permisos para el FileDescriptor han sido seteados a Read Only");
	} else {
		retorno = strcmp(permiso, WRITE);
		if (retorno == 0) {
			flag->read = false;
			flag->write = true;
			//logInfo("Los permisos para el FileDescriptor %s han sido seteados a Write Only", auxPFD->fileDescriptor->fd);
		} else {
			retorno = strcmp(permiso, RW);
			if (retorno == 0) {
				flag->read = true;
				flag->write = true;
				//logInfo("Los permisos para el FileDescriptor %s han sido seteados a Read & Write", auxPFD->fileDescriptor->fd);
			} else {
				flag->read = false;
				flag->write = false;
				//logInfo("No se han podido setear los permisos correspondientes para el FileDescriptor %s", auxPFD->fileDescriptor->fd);
			}
		}
	}
	return flag;
}

void agregarPFD_Alista(Process* process, t_processFileDescriptor* pfd) {
	list_add(process->fileDescriptorList, pfd);

}

void removerPFD_Lista(t_processFileDescriptor* pfd, Process* process) {
	bool condicion(void* element) {
		t_processFileDescriptor* pfd_aux = element;
		return pfd_aux == pfd;
	}

	list_remove_and_destroy_by_condition(process->fileDescriptorList, condicion,
			(void*) destroy_t_processFileDescriptor);

}

/*
 char* serialize_Flags(flags* flags){
 char* buffer = malloc(sizeof(flags));
 uint32_t offset = 0;

 serialize_and_copy_value(buffer,&(flags->read),sizeof(bool),&offset);
 serialize_and_copy_value(buffer,&(flags->write),sizeof(bool),&offset);

 return buffer;


 }

 flags* deserialize_Flags(char* buffer){
 flags* f = malloc(sizeof(flags)) ;
 uint32_t offset = 0;

 deserialize_and_copy_value(&(f->read),buffer,sizeof(bool),&offset);
 deserialize_and_copy_value(&(f->write),buffer,sizeof(bool),&offset);

 return f;

 }
 uint32_t sizeof_ProcessFileDescriptor(t_processFileDescriptor* pfd) {
 uint32_t longitud = 0;
 longitud += sizeof(flags);
 longitud += sizeof(t_fileDescriptor);

 return longitud;
 }


 char* serialize_ProcessFileDescriptor(t_processFileDescriptor* pfd){

 uint32_t total_size = sizeof_ProcessFileDescriptor(pfd);

 char *stream = malloc(sizeof(char) * total_size);

 char* tmpBuffer;

 uint32_t offset = 0;

 tmpBuffer = serialize_Flags(pfd->flag);

 serialize_and_copy_value(stream, tmpBuffer,
 sizeof(flags), &offset);

 free(tmpBuffer);

 tmpBuffer = serialize_FileDescriptor(pfd->fileDescriptor);

 serialize_and_copy_value(stream, tmpBuffer,
 sizeof(t_fileDescriptor), &offset);

 free(tmpBuffer);


 return stream;
 }

 t_processFileDescriptor* deserialize_ProcessFileDescritpro(char* buffer) {

 uint32_t offset = 0;

 t_processFileDescriptor* pfd = malloc(sizeof(t_processFileDescriptor));

 flags* f = malloc(sizeof(flags));
 t_size ft = sizeof(flags);

 deserialize_and_copy_value(&f,buffer, sizeof(flags),
 &offset);

 char* flagsBuffer = malloc(sizeof(flags));

 deserialize_and_copy_value(flagsBuffer, buffer, ft,
 &offset);
 pfd->flag = deserialize_Flags(flagsBuffer);

 t_processFileDescriptor* unPFD = malloc(sizeof(t_processFileDescriptor));

 deserialize_and_copy_value(unPFD,buffer,sizeof(t_processFileDescriptor),&offset);

 char* pfdBuffer = malloc(sizeof(t_processFileDescriptor));

 t_size pdfT = sizeof(t_processFileDescriptor);

 deserialize_and_copy_value(pfdBuffer,buffer,pdfT,&offset);

 pfd->fileDescriptor = deserialize_FileDescriptor(pfdBuffer);

 free(f);  //verificar

 free(pfdBuffer);

 free(unPFD);

 return pfd;
 }
 */
