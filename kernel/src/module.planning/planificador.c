


#include "planificador.h"



int planificar(int argc, char *argv[]) {
	//pthread_mutex_init(&readyListMutex, NULL);

	//LEVANTO EL ARCHIVO DE CONFIG
	//Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	listaDeBloqueado = malloc(50000);
	listaDeBloqueado = list_create();
	listaDeEjecutado = malloc(50000);
	listaDeEjecutado = list_create();
	listaDeListo = malloc(50000);
	listaDeListo = list_create();

	pthread_t thread1;
	pthread_t thread2;

	char *m1 = "consola";
	char *m2 = "despachador";

	int r1;
	int r2;

//	r1 = pthread_create( &thread1, NULL, &consola, (void*) m1); FALTA CREAR O LINKEAR CON LA CONSOLA
	r2 = pthread_create( &thread2, NULL, &enviarPCBaMemoria, (void*) m2);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);

	return 1;

}



void* enviarPCBaMemoria() {

	sem_wait(&mutexMemoria);
	while(1) {
		sem_wait(&semProgramas);

		if (listaDeListo->elements_count > 0) {
			//sem_wait(sem_Memoria_conectada);
			t_pcb * nodoPCB =  list_get(listaDeListo, 0);

			int err = enviarMensajeDePCBaMemoria(socketMemoria, nodoPCB); //Guardo el retorno de la funcion en una var para saber si dio error

			if (err <= 0){ //Error en el envío
				printf("No se pudo enviar el PCB %d",nodoPCB->PID);
			} else { //Enviado correctamente

				list_remove(listaDeListo, 0); //< Saca de lista ready

				list_add(listaDeEjecutado,nodoPCB ); //< Se agrega a la lista de ejecucion
/*
				t_resp_Memoria_plan * nodoRespuesta;
				int nbytes;
				nodoRespuesta = malloc(sizeof(t_resp_Memoria_plan));
				while ((nbytes = socketRecibirMensaje(socketMemoria, nodoRespuesta,sizeof(t_nodo_mem)) > 0)){
					interpretarLinea(nodoRespuesta);
				}//fin while recibir rta
*/
			}//Cierra el err

		}//cierra if cantidad de elementos

	}//cierra while



//	int programaValido(char * programa); < Elimino Funcion programa Valido. se Puede utilizar file_exists de dc-commons


void agregarALista(char * programa) {

	t_pcb * pcb = malloc(sizeof(t_pcb));

	pcb->PID = contadorPID; //Asigno valor de Process ID (0 Inicial declarado en .h)
	contadorPID++; //Aumento para prox AnSISOP
	strcat(programa, "\0"); //Agrega /0 para indicar que es el ultimo de la lista, necesario para tratarlo como string.
	pcb->contextoEjecucion = programa;

//	pcb->pc=0;
//	pcb->estado=LISTO;
//	pcb->quantum=quantumcfg;


	int antesDeAgregar = listaDeListo->elements_count; //Guardo Cuantos hay en la Lista Antes de agregar

	//MUTEX  PARA PRIORIZAR BLOQUEADOS Y RR SOBRE NUEVOS
	/*pthread_mutex_lock(&readyListMutex);
	list_add(listaDeListo, pcb);
	pthread_mutex_unlock(&readyListMutex);
	*/
	list_add(listaDeListo, pcb);

	int despuesDeAgregar = listaDeListo->elements_count; //Guardo cuantos hay despues..

	if (despuesDeAgregar > antesDeAgregar) { //Verifico

		sem_post(&semProgramas); //NO SE Q HACE ESTE SEMAFORO

	} else {

		perror("Lista no agregada."); //Si despues de agregar no es mayor a antes, significa que no se pudo agregar a la lista por lo que nos da error.

	}
	pthread_exit(NULL);
}

/*
static  t_hilos *hilo_create(pthread_t thread, char * m, int  r) {

	t_hilos *nuevo = malloc(sizeof(t_hilos));
	nuevo->thread = thread;
	nuevo->r = r;
	strncpy(nuevo->m, m, sizeof(nuevo->m)-1);
	nuevo->m[sizeof(nuevo->m)-1] = "\0";
    return nuevo;

}

*/
/*
static  t_pcb *pcb_create(int PID, char * contextoDeEjecucion) {

	t_pcb *nuevo = malloc(sizeof(t_pcb)); //Reservo tamaño para lag estructura PCB

	nuevo->PID = PID;

	//nuevo->estado = LISTO;
	//nuevo->pc = 0;
	//nuevo->quantum = quantumcfg;

	strcpy(nuevo->contextoEjecucion, contextoDeEjecucion);

    return nuevo;

}
*/

int enviarMensajeDePCBaMemoria(int socketMemoria, t_pcb * nodoPCB) {

	int nbytes;
	unsigned char buffer[1024];
	empaquetarPCB(buffer,nodoPCB);
	nbytes = send(socketMemoria, buffer, sizeof(buffer) , 0);
	if (nbytes < 0) {
		printf("Planificador: Socket Memoria %d desconectado.\n", socketMemoria);
	}

	return nbytes;

}


void empaquetarPCB(unsigned char *buffer,t_pcb * nodoPCB)

{
	unsigned int tamanioBuffer;

	/*
	t_pcb * pcb1 = malloc(sizeof(t_pcb));
	pcb1->PID = 0;
	pcb1->contextoEjecucion = malloc(strlen("programa3.cod"));
	strcpy(pcb1->contextoEjecucion,"programa3.cod");
	pcb1->pc=3;
	pcb1->estado=LISTO;
	pcb1->quantum=5;
	*/

	printf("PCB a enviar: PID: %d,  Estado: %d, PC: %d, Quantum: %d, Archivo: %s\n",
	nodoPCB->PID,nodoPCB->contextoEjecucion);

	//nodoPCB->PID,nodoPCB->estado,nodoPCB->pc,nodoPCB->quantum,nodoPCB->contextoEjecucion);


	//tamanioBuffer = pack(buffer,SECUENCIA_PCB,nodoPCB->PID,nodoPCB->estado,nodoPCB->pc,nodoPCB->quantum,nodoPCB->contextoEjecucion);
	tamanioBuffer = pack(buffer,SECUENCIA_PCB,&nodoPCB->PID,nodoPCB->contextoEjecucion);

	packi16(buffer+1, tamanioBuffer); // store packet size in packet for kicks

	printf("Tamaño del PCB serializado es %u bytes\n", tamanioBuffer);

}



void desempaquetarPCB(unsigned char *buffer,t_pcb * nodoPCB){

	//t_pcb * pcb = malloc(sizeof(t_pcb));

	char programa[50];

//	unpack(buffer,SECUENCIA_PCB,&nodoPCB->PID,&nodoPCB->estado,&nodoPCB->pc,&nodoPCB->quantum,programa);
	unpack(buffer,SECUENCIA_PCB,&nodoPCB->PID,programa);

	nodoPCB->contextoEjecucion = programa;

	printf("PCB recibido: PID: %d,  Archivo: %s\n",nodoPCB->PID,nodoPCB->contextoEjecucion);

			//nodoPCB->PID,nodoPCB->estado,nodoPCB->pc,nodoPCB->quantum,nodoPCB->contextoEjecucion);

}










	/* EJEMPLO DE ENVIO / RECEPCION PARA PROBAR EN MAIN

	 t_pcb * pcb1 = malloc(sizeof(t_pcb)); //Crea la estructura del PCB

	pcb1->PID = 0; //Carga el campo PID de la estructura

	pcb1->contextoEjecucion = malloc(strlen("programa3.cod")); //Reserva memoria para el contexto de la estructura segun tamaño

	strcpy(pcb1->contextoEjecucion,"programa3.cod"); //Carga el Contexto del PCB

	pcb1->pc=3; // Carga el Program Counter.

	pcb1->estado=LISTO;

	pcb1->quantum=5;

	unsigned char buffer[1024];

	empaquetarPCB(buffer,pcb1);

	puts("pcb enviado.");



	t_pcb * pcb = malloc(sizeof(t_pcb));

	desempaquetarPCB(buffer,pcb);

	 */

}

