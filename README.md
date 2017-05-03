#tp-2017-1c-Los-5-Fant-sticos


## tp-2017-1c-Los-5-Fant-sticos  Library for C ##


## Guía de Instalación: ##


	- Clonar el proyecto tp-2017-1c-Los-5-Fant-sticos
	- Desde consola ir al directorio del proyecto /dc-commons
	- `sudo make install` -> instala la biblioteca en el sistema
	- `sudo make uninstall` -> desinstala la biblioteca

## Guía para el uso: ##

Linkear con `-ldc-commons`

Para usarla en un .c/.h debería incluirse de la siguiente forma: `dc-commons/<Nombre_TAD>`

Por ejemplo:

```c
#include <dc-commons/logger.h>

## Desde eclipse ##

1. Ir a las Properties del proyecto (en el Project Explorer - la columna de la izquierda - la opción aparece dándole click derecho al proyecto), y dentro de la categoría `C/C++ Build` entrar a `Settings`, y ahí a `Tool Settings`.
2. Buscar `GCC Linker` > `Libraries` > `Libraries`. Notar que entre paréntesis dice `-l`, el parámetro de `gcc` que estamos buscando.
3. Darle click en el botón de `+`, y poner el nombre de la biblioteca sin el `-l` (en este caso, `dc-commons`).
4. Aceptar y buildear el proyecto.
