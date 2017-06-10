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
					bool shouldLock = deserialize_bool(package->stream); //el package contiene un booleando que determina si el proceso va a bloquearse
					if (shouldLock) {
						logTrace(
								"El kernel indico que el proceso debe bloquearse, envio el pcb al kernel");
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

t_descriptor_archivo ansisopKernel_abrir(t_direccion_archivo path,
		t_banderas flags) {
	if (path[strlen(path) - 1] == '\n') {
		path[strlen(path) - 1] = '\0';
	}
	uint32_t newFD = NULL_VALUE;
	logTrace(
			"Ejecutando ansisopKernel_abrir(%s,flags(leer:%d, escribir:%d, crear:%d))",
			path, flags.lectura, flags.escritura, flags.creacion);
	if (getErrorFlag() == FLAG_OK) {
		t_new_FD_request* data = create_t_new_FD_request(path, flags);
		char* buffer = serialize_t_new_FD_request(data);
		size_t size = sizeof_t_new_FD_request(data);
		Package* package = createAndSendPackage(kernelSocket(), COD_OPEN_FD,
				size, buffer);
		destroy_t_new_FD_request(data);
		free(buffer);
		if (package != NULL) {
			logTrace("Se solicito al kernel abrir el archivo %s", path);
			destroyPackage(package);
			package = createAndReceivePackage(kernelSocket());
			if (package != NULL) {
				if (package->msgCode == COD_SYSCALL_SUCCESS) {
					newFD = deserialize_int(package->stream);
					logTrace("El kernel asigno el fd: %d para el archivo %s",
							newFD, path);
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
	logTrace(
			"Ejecutado ansisopKernel_abrir(%s,flags(leer:%d, escribir:%d, crear:%d))",
			path, flags.lectura, flags.escritura, flags.creacion);
	return newFD;
}

void ansisopKernel_borrar(t_descriptor_archivo fd) {
	logTrace("Ejecutando ansisopKernel_borrar(%d)", fd);
	if (getErrorFlag() == FLAG_OK) {
		char* buffer = serialize_int(fd);
		Package* package = createAndSendPackage(kernelSocket(), COD_DELETE_FD,
				sizeof(uint32_t), buffer);
		free(buffer);
		if (package != NULL) {
			logTrace("Se solicito al kernel borrar el file descriptor %d", fd);
			destroyPackage(package);
			package = createAndReceivePackage(kernelSocket());
			if (package != NULL) {
				if (package->msgCode == COD_SYSCALL_SUCCESS) {
					logTrace("El kernel borro el file descriptor %d", fd);
				} else {
					logTrace(
							"El kernel rechazo el borrado del file descriptor %d",
							fd);
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
	logTrace("Ejecutado ansisopKernel_borrar(%d)", fd);
}

void ansisopKernel_cerrar(t_descriptor_archivo fd) {
	logTrace("Ejecutando ansisopKernel_cerrar(%d)", fd);
	if (getErrorFlag() == FLAG_OK) {
		char* buffer = serialize_int(fd);
		Package* package = createAndSendPackage(kernelSocket(), COD_CLOSE_FD,
				sizeof(uint32_t), buffer);
		free(buffer);
		if (package != NULL) {
			logTrace("Se solicito al kernel cerrar el file descriptor %d", fd);
			destroyPackage(package);
			package = createAndReceivePackage(kernelSocket());
			if (package != NULL) {
				if (package->msgCode == COD_SYSCALL_SUCCESS) {
					logTrace("El kernel cerro el file descriptor %d", fd);
				} else {
					logTrace(
							"El kernel rechazo el cierre del file descriptor %d",
							fd);
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
	logTrace("Ejecutado ansisopKernel_cerrar(%d)", fd);
}

void ansisopKernel_moverCursor(t_descriptor_archivo fd, t_valor_variable offset) {
	logTrace("Ejecutando ansisopKernel_moverCursor(%d,%d)", fd, offset);
	if (getErrorFlag() == FLAG_OK) {
		t_seed_FD_request* data = create_t_seed_FD_request(fd, offset);
		char* buffer = serialize_t_seed_FD_request(data);
		size_t size = sizeof_t_seed_FD_request(data);
		Package* package = createAndSendPackage(kernelSocket(), COD_SEED_FD,
				size, buffer);
		destroy_t_seed_FD_request(data);
		free(buffer);
		if (package != NULL) {
			logTrace(
					"Se solicito al kernel mover cursor a: %d del file descriptor %d",
					offset, fd);
			destroyPackage(package);
			package = createAndReceivePackage(kernelSocket());
			if (package != NULL) {
				if (package->msgCode == COD_SYSCALL_SUCCESS) {
					logTrace("El kernel movio el cursor del file descriptor %d",
							fd);
				} else {
					logTrace(
							"El kernel rechazo el movimiento del cursor del file descriptor %d",
							fd);
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
	logTrace("Ejecutado ansisopKernel_moverCursor(%d,%d)", fd, offset);
}

void ansisopKernel_escribir(t_descriptor_archivo fd, void* info,
		t_valor_variable sizeInfo) {
	logTrace("Ejecutando ansisopKernel_escribir(%d,>>data<<,%d)", fd, sizeInfo);
	if (getErrorFlag() == FLAG_OK) {
		t_seed_FD_request* data = create_t_data_FD_request(fd, sizeInfo, info);
		char* buffer = serialize_t_data_FD_request(data);
		size_t size = sizeof_t_data_FD_request(data);
		Package* package = createAndSendPackage(kernelSocket(), COD_WRITE_FD,
				size, buffer);
		destroy_t_data_FD_request(data);
		free(buffer);
		if (package != NULL) {
			logTrace("Se solicito al kernel escribir en el file descriptor %d",
					fd);
			destroyPackage(package);
			package = createAndReceivePackage(kernelSocket());
			if (package != NULL) {
				if (package->msgCode == COD_SYSCALL_SUCCESS) {
					logTrace("El kernel escribio en el file descriptor %d", fd);
				} else {
					logTrace(
							"El kernel rechazo la escritura en el file descriptor %d",
							fd);
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
	logTrace("Ejecutado ansisopKernel_escribir(%d,>>data<<,%d)", fd, sizeInfo);
}

void ansisopKernel_leer(t_descriptor_archivo descriptor_archivo,
		t_puntero informacion, t_valor_variable tamanio) {

}
