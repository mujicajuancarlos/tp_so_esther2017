/*
 * primitiveKernelFuncions.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "primitiveKernelFunctions.h"

int kernelSocket() {
	return getCPUStruct()->socketClientKernel;
}

void ansisopKernel_wait(t_nombre_semaforo identificador_semaforo) {
	if (identificador_semaforo[strlen(identificador_semaforo) - 1] == '\n') {
		identificador_semaforo[strlen(identificador_semaforo) - 1] = '\0';
	}
	logTrace("Ejecutando ansisopKernel_wait(%s)", identificador_semaforo);
	if (getErrorFlag() == FLAG_OK) {
		Package* package = createAndSendPackage(kernelSocket(), COD_SEM_WAIT,
				strlen(identificador_semaforo), identificador_semaforo);
		if (package != NULL) {
			logTrace("Se solicito al kernel ejecutar wait en el semaforo %s",
					identificador_semaforo);
			destroyPackage(package);
			package = createAndReceivePackage(kernelSocket());
			if (package != NULL) {
				if (package->msgCode == COD_SYSCALL_SUCCESS) {
					logTrace("Se ejecuto wait sobre el semaforo %s",
							identificador_semaforo);
					bool shouldLock = deserialize_bool(package->stream);//el package contiene un booleando que determina si el proceso va a bloquearse
					if (shouldLock) {
						logTrace("El kernel indico que el proceso debe bloquearse, envio el pcb al kernel");
						reportContextSwich(getCPUStruct());
						setErrorFlag(FLAG_PROCESS_BLOCK);
					}
				} else {
					setErrorFlag(FLAG_SYSCALL_FAILURE);
				}
				destroyPackage(package);
			} else {
				setErrorFlag(FLAG_DISCONNECTED_KERNEL);
			}
		} else {
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	}
	logTrace("Ejecutado ansisopKernel_wait(%s)", identificador_semaforo);
}

void ansisopKernel_signal(t_nombre_semaforo identificador_semaforo) {
	if (identificador_semaforo[strlen(identificador_semaforo) - 1] == '\n') {
		identificador_semaforo[strlen(identificador_semaforo) - 1] = '\0';
	}
	logTrace("Ejecutando ansisopKernel_signal(%s)", identificador_semaforo);
	if (getErrorFlag() == FLAG_OK) {
		Package* package = createAndSendPackage(kernelSocket(), COD_SEM_SIGNAL,
				strlen(identificador_semaforo), identificador_semaforo);
		if (package != NULL) {
			logTrace("Se solicito al kernel ejecutar signal en el semaforo %s",
					identificador_semaforo);
			destroyPackage(package);
			package = createAndReceivePackage(kernelSocket());
			if (package != NULL) {
				if (package->msgCode == COD_SYSCALL_SUCCESS) {
					logTrace("Se ejecuto signal sobre el semaforo %s",
							identificador_semaforo);
				} else {
					setErrorFlag(FLAG_SYSCALL_FAILURE);
				}
				destroyPackage(package);
			} else {
				setErrorFlag(FLAG_DISCONNECTED_KERNEL);
			}
		} else {
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	}
	logTrace("Ejecutado ansisopKernel_signal(%s)", identificador_semaforo);
}

t_puntero ansisopKernel_reservar(t_valor_variable espacio) {
	return 0;
}

void ansisopKernel_liberar(t_puntero puntero) {

}

t_descriptor_archivo ansisopKernel_abrir(t_direccion_archivo direccion,
		t_banderas flags) {
	return 0;
}

void ansisopKernel_borrar(t_descriptor_archivo direccion) {

}

void ansisopKernel_cerrar(t_descriptor_archivo descriptor_archivo) {

}

void ansisopKernel_moverCursor(t_descriptor_archivo descriptor_archivo,
		t_valor_variable posicion) {

}

void ansisopKernel_escribir(t_descriptor_archivo descriptor_archivo,
		void* informacion, t_valor_variable tamanio) {

}

void ansisopKernel_leer(t_descriptor_archivo descriptor_archivo,
		t_puntero informacion, t_valor_variable tamanio) {

}
