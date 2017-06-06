/*
 * primitiveKernelFuncions.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "primitiveKernelFunctions.h"

int kernelSocket() {
	return getCPUStruct().socketClientKernel;
}

void ansisopKernel_wait(t_nombre_semaforo identificador_semaforo) {
	logTrace("Ejecutando ansisopKernel_wait(%c)", identificador_semaforo);
	if (getErrorFlag() == FLAG_OK) {
		Package* package = createAndSendPackage(kernelSocket(), COD_SEM_WAIT, 1,
				identificador_semaforo);
		if (package != NULL) {
			logInfo("Se solicito al kernel ejecutar wait en el semaforo %c",
					identificador_semaforo);
			destroyPackage(package);
		}else{
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	}
	logTrace("Ejecutado ansisopKernel_wait(%c)", identificador_semaforo);
}

void ansisopKernel_signal(t_nombre_semaforo identificador_semaforo) {
	logTrace("Ejecutando ansisopKernel_signal(%c)", identificador_semaforo);
	if (getErrorFlag() == FLAG_OK) {
		Package* package = createAndSendPackage(kernelSocket(), COD_SEM_SIGNAL, 1,
				identificador_semaforo);
		if (package != NULL) {
			logInfo("Se solicito al kernel ejecutar signal en el semaforo %c",
					identificador_semaforo);
			destroyPackage(package);
		}else{
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	}
	logTrace("Ejecutado ansisopKernel_signal(%c)", identificador_semaforo);
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
