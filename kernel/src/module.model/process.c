/*
 * ansisopProcess.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "process.h"

int currentPid = FIRST_PID;
pthread_mutex_t currentPidMutex;
pthread_mutex_t tablaProcesosFD_mutex;

Process* createProcess(int socket, kernel_struct* kernelStruct) {
	Process* newProcess = malloc(sizeof(Process));
	newProcess->fileDescriptor = socket;
	newProcess->pid = 0;
	newProcess->kernelStruct = kernelStruct;
	newProcess->pcb = NULL;
	tablaProcesosFD = list_create();
	return newProcess;
}

void destroyProcess(Process* process) {
	if (process != NULL) {
		if (process->pcb != NULL)
			destroy_PBC(process->pcb);
		free(process);
		list_destroy_and_destroy_elements(tablaProcesosFD, //TODO: MOVER A PROCESS (Farid)
			(void*) destroy_t_processFileDescriptor);
	}
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

void printHeaderProcess() {
	printf("\n%5s\t%20s\t%20s\n%s\n", "PID", "STATE", "EXIT CODE",
			"===============================================================");
}

void printProcess(Process* proceso, char* state) {
	char* exitCode = (proceso->pcb==NULL)? "" : string_itoa(proceso->pcb->exit_code);
	printf("%5d\t%20s\t%20s\n", proceso->pid, state, exitCode);
}

void printProcessFull(Process* proceso){
	printf("\nInformación del proceso %d\n", proceso->pid);
	printf("\tCantidad rafagas ejecutadas %d\n", 567);//TODO: completar rafagas ejecutadas
	printf("\tCantidad de operaciones privilegiadas %d\n", 142);//TODO: completar
	printf("\tTabla de archivos abiertos %d\n", 423);//TODO: completar
	printf("\tCantidad de paginas heap utilizadas %d\n", 423);//TODO: completar
	printf("\tOtra cantidad %d\n", 432);//TODO: completar
	printf("\tOtra cantidad %d\n", 234);//TODO: completar
	printf("\tCantidad de syscall ejecutadas %d\n", 432);//TODO: completar
}

t_processFileDescriptor* createNew_t_processFileDescriptor(char* permiso,
		t_fileDescriptor* fd) {

	t_processFileDescriptor* newPFD = malloc(sizeof(t_processFileDescriptor));

	newPFD->fileDescriptor = fd;
	newPFD->flag = habilitarPermisos(permiso);

	return newPFD;

}

void destroy_t_processFileDescriptor(t_processFileDescriptor* pfd) {
	destroy_t_filedescriptor(pfd->fileDescriptor);
	free(pfd);
}


flags habilitarPermisos(char* permiso) {
	flags flag;
	int retorno=-1;
	retorno = strcmp(permiso, READ);
	if (retorno == 0) {
		flag.read = true;
		flag.write = false;
		//logInfo("Los permisos para el FileDescriptor han sido seteados a Read Only");
	} else {
		retorno = strcmp(permiso, WRITE);
		if (retorno == 0) {
			flag.read = false;
			flag.write = true;
			//logInfo("Los permisos para el FileDescriptor %s han sido seteados a Write Only", auxPFD->fileDescriptor->fd);
		} else {
			retorno = strcmp(permiso, RW);
			if (retorno == 0) {
				flag.read = true;
				flag.write = true;
				//logInfo("Los permisos para el FileDescriptor %s han sido seteados a Read & Write", auxPFD->fileDescriptor->fd);
			} else {
				flag.read = false;
				flag.write = false;
				//logInfo("No se han podido setear los permisos correspondientes para el FileDescriptor %s", auxPFD->fileDescriptor->fd);
			}
		}
	}
	return flag;
}



void agregarPFD_Alista(t_processFileDescriptor* pfd) {		//TODO: MOVER A PROCESS (Farid)

	list_add(fileDescriptorGlobalList, pfd);

}

void removerPFD_Lista(t_processFileDescriptor* pfd) {			//TODO: MOVER A PROCESS (Farid)
	bool condicion(void* element) {
		t_processFileDescriptor* pfd_aux = element;
		return pfd_aux == pfd;
	}

	list_remove_and_destroy_by_condition(tablaProcesosFD, condicion,
			(void*) destroy_t_processFileDescriptor);

}

