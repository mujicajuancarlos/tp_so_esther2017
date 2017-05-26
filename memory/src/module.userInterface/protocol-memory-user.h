/*
 * protocol-console-user.h
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#ifndef PROTOCOL_KERNEL_USER_H_
#define PROTOCOL_KERNEL_USER_H_

#define COD_CONSOLE_HELP "-help"
#define COD_CONSOLE_EXIT "exit"
#define COD_CONSOLE_CLEAR "clear"
#define COD_CONSOLE_SLEEP "sleep"
#define COD_CONSOLE_DUMP "dump"
#define COD_CONSOLE_FLUSH "flush"
#define COD_CONSOLE_SIZE "size"

// opciones para el dump
#define OPT_CACHE "cache"
#define OPT_STRUCTURE "struc"
#define OPT_CONTENT "cont"

// opciones para el size
#define OPT_MEMORY "memo"
#define OPT_PID "pid"

// opcion para el sleep
#define OPT_MILISECONDS "-mil"

#define OPT_PAGE "-p"
#define OPT_ALL "-all"

#endif /* PROTOCOL_KERNEL_USER_H_ */
