NAME=memory
release:  cleanRelease
	@echo "> START................................................................................ "
	@echo "Iniciando compilacion de <$(NAME)> en modo RELEASE "
	-cd Release && $(MAKE) all
	@echo "Finalizando compilacion de <$(NAME)> en modo RELEASE "
	@echo "Copiando ejecutable en directorio principal de <$(NAME)> "
	-cp ./Release/$(NAME) ./$(NAME)
	@echo "> END................................................................................ "

cleanRelease:
	@echo "> START................................................................................ "
	@echo "Limpiando archivos de <$(NAME)> "
	-cd Release && $(MAKE) clean
	-rm ./$(NAME)
	@echo "> END................................................................................ "

debug: cleanDebug
	@echo "> START................................................................................ "
	@echo "Iniciando compilacion de <$(NAME)> en modo DEBUG "
	-cd Debug && $(MAKE) all
	@echo "Finalizando compilacion de <$(NAME)> en modo DEBUG "
	@echo "Copiando ejecutable en directorio principal de <$(NAME)> "
	-cp ./Debug/$(NAME) ./$(NAME)
	@echo "> END................................................................................ "

cleanDebug:
	@echo "> START................................................................................ "
	@echo "Limpiando archivos de <$(NAME)> "
	-cd Debug && $(MAKE) clean
	-rm ./$(NAME)
	@echo "> END................................................................................ "