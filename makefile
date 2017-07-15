install:
	@echo "> START: INSTALACION LIBRERIAS ESTHER - 2017 - LOS 3 FANTASTICOS........................................................................... "
	@echo "Instalando  librerias externas del sistema  "
	-cd others && $(MAKE) install
	@echo "Instalando librerias internas del sistema "
	-cd dc-commons && $(MAKE) install
	@echo "> END: INSTALACION LIBRERIAS ESTHER - 2017 - LOS 3 FANTASTICOS............................................................................. "

uninstall:
	@echo "> START: DESINSTALACION LIBRERIAS ESTHER - 2017 - LOS 3 FANTASTICOS...................................................................... "
	@echo "Desinstalando librerias externas del sistema "
	-cd others && $(MAKE) uninstall
	@echo "Desinstalando librerias internas del sistema "
	-cd dc-commons && $(MAKE) uninstall
	@echo "> END: DESINSTALACION LIBRERIAS ESTHER - 2017 - LOS 3 FANTASTICOS........................................................................ "

uninstallSadica:
	-cd /home/utnso/ && rm -rf FS_SADICA
	-cd /home/utnso/ && rm -f FS_SADICA.tar

installSadica: uninstallSadica
	-cp FS_SADICA.tar /home/utnso/.
	-cd /home/utnso/ && tar -xvf FS_SADICA.tar

build: install installSadica
	@echo "> START: BUILD - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS...................................................................................... "
	@echo "Construyendo DC-COMMONS................................................................................................................................................. "
	-cd dc-commons && $(MAKE) release
	@echo "Construyendo CONSOLA.......................................................................................................................................................... "
	-cd console && $(MAKE) release
	@echo "Construyendo CPU ...................................................................................................................................................................."
	-cd cpu && $(MAKE) release
	@echo "Construyendo FILE SYSTEM ......................................................................................................................................................"
	-cd fileSystem && $(MAKE) release
	@echo "Construyendo KERNEL.............................................................................................................................................................. "
	-cd kernel && $(MAKE) release
	@echo "Construyendo MEMORY ..........................................................................................................................................................."
	-cd memory && $(MAKE) release
	@echo "> END: BUILD - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS........................................................................................... "

clean: uninstall uninstallSadica
	@echo "> START: CLEAN - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS...................................................................................... "
	@echo "Removiendo DC-COMMONS................................................................................................................................................. "
	-cd dc-commons && $(MAKE) cleanRelease
	@echo "Removiendo CONSOLA.......................................................................................................................................................... "
	-cd console && $(MAKE) cleanRelease
	@echo "Removiendo CPU ...................................................................................................................................................................."
	-cd cpu && $(MAKE) cleanRelease
	@echo "Removiendo FILE SYSTEM ......................................................................................................................................................"
	-cd fileSystem && $(MAKE) cleanRelease
	@echo "Removiendo KERNEL.............................................................................................................................................................. "
	-cd kernel && $(MAKE) cleanRelease
	@echo "Removiendo MEMORY ..........................................................................................................................................................."
	-cd memory && $(MAKE) cleanRelease
	@echo "> END: CLEAN - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS........................................................................................... "

buildDebug: install installSadica
	@echo "> START: BUILD DEBUG - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS............................................................................... "
	@echo "Construyendo DEBUG DC-COMMONS...................................................................................................................................... "
	-cd dc-commons && $(MAKE) debug
	@echo "Construyendo DEBUG CONSOLA............................................................................................................................................. "
	-cd console && $(MAKE) debug
	@echo "Construyendo DEBUG CPU ......................................................................................................................................................."
	-cd cpu && $(MAKE) debug
	@echo "Construyendo DEBUG FILE SYSTEM .........................................................................................................................................."
	-cd fileSystem && $(MAKE) debug
	@echo "Construyendo DEBUG KERNEL.................................................................................................................................................. "
	-cd kernel && $(MAKE) debug
	@echo "Construyendo DEBUG MEMORY ..............................................................................................................................................."
	-cd memory && $(MAKE) debug
	@echo "> END: BUILD DEBUG - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS.................................................................................. "

cleanDebug: uninstall uninstallSadica
	@echo "> START: CLEAN DEBUG - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS.............................................................................. "
	@echo "Removiendo DEBUG DC-COMMONS......................................................................................................................................... "
	-cd dc-commons && $(MAKE) cleanDebug
	@echo "Removiendo DEBUG CONSOLA................................................................................................................................................. "
	-cd console && $(MAKE) cleanDebug
	@echo "Removiendo DEBUG CPU ..........................................................................................................................................................."
	-cd cpu && $(MAKE) cleanDebug
	@echo "Removiendo DEBUG FILE SYSTEM ............................................................................................................................................"
	-cd fileSystem && $(MAKE) cleanDebug
	@echo "Removiendo DEBUG KERNEL.................................................................................................................................................... "
	-cd kernel && $(MAKE) cleanDebug
	@echo "Removiendo DEBUG MEMORY ................................................................................................................................................."
	-cd memory && $(MAKE) cleanDebug
	@echo "> END: CLEAN DEBUG - RELEASE - ESTHER - 2017 - LOS 3 FANTASTICOS................................................................................. "