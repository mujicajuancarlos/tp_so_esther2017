/*
 * protocol-fileSystem-kernel.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_FILESYSTEM_KERNEL_H_
#define SRC_DC_COMMONS_PROTOCOL_FILESYSTEM_KERNEL_H_

#define COD_VALIDAR_ARCHIVO 30 // Validar que exista el archivo
#define COD_CREAR_ARCHIVO 34 // Crea archivo en el path solicitado
#define COD_BORRAR_ARCHIVO 32 // Borra archivo del path (borra metadata y libera bloques del bitmap)
#define COD_OBTENER_DATOS 38 // Devuelve el path enviado de parámetro, requiere que el archivo este en modo lectura)
#define COD_GUARDAR_DATOS 42 // Almacena en el path enviado, requiere archivo en modo escritura)
#define COD_SALUDO 100

#endif /* SRC_DC_COMMONS_PROTOCOL_FILESYSTEM_KERNEL_H_ */
