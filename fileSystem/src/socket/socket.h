// Librerías generales

#include <stdio.h> //Para usar perror() - quizás
#include <stdlib.h> //Para usar malloc() y calloc() - no necesito
#include <unistd.h> //Para importar el close()
#include <errno.h> // Para usar la variable errno
#include <string.h> // Para importar memset()
#include <sys/types.h> // Importa tipos de datos requeridos por el socket
#include <sys/socket.h> // Importa las funciones necesarias del socket más algunas estructuras
#include <sys/epoll.h> // Creo que facilita el manejo de errores
#include <netinet/in.h> // Para poder usar in_port_t, in_addr y in_addr_in
#include <arpa/inet.h> // Para operaciones de flujo en internet
#include <sys/wait.h> // Para hacer uso de todas las funciones que aparecen en signal.h
#include <signal.h> //Para manejar señales del proceso en ejecución o definir macros adicionales con SIG

// dc-commoms

#include <dc-commons/socket-server.h>
#include <dc-commons/package.h>

int crearSocketServer(int puerto);
int aceptarConexionCliente(int socketServerFileDescriptor);



