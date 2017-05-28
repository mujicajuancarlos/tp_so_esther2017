/*
 * primitiveKernelFuncions.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "primitiveKernelFunctions.h"

void ansisopKernel_wait(t_nombre_semaforo identificador_semaforo) {

}

void ansisopKernel_signal(t_nombre_semaforo identificador_semaforo) {

}

t_puntero ansisopKernel_reservar(t_valor_variable espacio) {
	return 0;
}

void ansisopKernel_liberar(t_puntero puntero) {

}

t_descriptor_archivo ansisopKernel_abrir(t_direccion_archivo direccion, t_banderas flags) {
	return 0;
}

void ansisopKernel_borrar(t_descriptor_archivo direccion) {

}

void ansisopKernel_cerrar(t_descriptor_archivo descriptor_archivo) {

}

void ansisopKernel_moverCursor(t_descriptor_archivo descriptor_archivo, t_valor_variable posicion) {

}

void ansisopKernel_escribir(t_descriptor_archivo descriptor_archivo, void* informacion, t_valor_variable tamanio) {

}

void ansisopKernel_leer(t_descriptor_archivo descriptor_archivo, t_puntero informacion, t_valor_variable tamanio) {

}
