Funciones para File System
/* EL FS ATIENDE PETICIONES SECUENCIALMENTE, NO EN PARALELO*/

*ValidarArchivo(Path)
	Validar que le archivo exista cuando el kernel reciba peticion de abrir un archivo

*CrearArchivo(Path)
	Cuando el kernel reciba la peticion de abrir uno, en caso de que el archivo no exista, y haya sido abierto en modo creacion, se llama a esta funcion que creara el archivo dentro del path pasado. (A todo archivoi creado se le debe asignar minimo 1 bloque de datos.

*Borrar(Path)
	Borrara el archivo que indique el path, eliminando sua rchivo de metadata y marcando los bloques como libres dentro del bitmap.

*ObtenerDatos(Path,Offset,size)
	Ante este pedido, se devolvera de ese path, la cantidad de bytes definidos por Size a partir de el offset solicitado. (requiere que el archivo se encuentre en modo lectura).

*GuardarDatos(path,Offset,Size,Buffer)
	Ante un pedido de escritura, se almacenara en dicho path, los bytes del buffer definidos por Size y a partir del offset señalado. El archivo se debe encontrar en modo escritura.

Estas son las funcionalidades fisicas, la interfaz que maneja, la cual dice el tp que no puede ser ampliada. Seguramente surjan mini funciones dentro de ellas, pero las centrales son las de arriba.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
El File System por lo que entendi deberia tener la siguiente estructura

MNT ----> FS_SADICA 	------>Archivos
						------>Bloques
						------>Metadata 	------> Metadata.bin
											------> Bitmap.bin

Falta analizar bien todo lo del Sadica para ver como se hace, pero no creo que sea nada complicado. Y para el bitmap estan las funciones de las commons.