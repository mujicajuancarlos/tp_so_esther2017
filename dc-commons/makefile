NAME=dc-commons
install:
	@echo "Instalando la libreria <$(NAME)> en el Sistema "
	-cd src && sudo $(MAKE) install
	@echo "La libreria <$(NAME)> se instalo exitosamente "

uninstall:
	@echo "Desinstalando la libreria <$(NAME)> en el Sistema "
	-cd src && sudo $(MAKE) uninstall
	@echo "La libreria <$(NAME)> se desinstalo exitosamente "

release:  cleanRelease
	@echo "> START................................................................................ "
	@echo "Iniciando compilacion de <$(NAME)> en modo RELEASE "
	-cd Release && $(MAKE) all
	@echo "Finalizando compilacion de <$(NAME)> en modo RELEASE "
	@echo "> END................................................................................ "

cleanRelease:
	@echo "> START................................................................................ "
	@echo "Limpiando archivos de <$(NAME)> "
	-cd Release && $(MAKE) clean
	@echo "> END................................................................................ "

debug: cleanDebug
	@echo "> START................................................................................ "
	@echo "Iniciando compilacion de <$(NAME)> en modo DEBUG "
	-cd Debug && $(MAKE) all
	@echo "Finalizando compilacion de <$(NAME)> en modo DEBUG "
	@echo "> END................................................................................ "

cleanDebug:
	@echo "> START................................................................................ "
	@echo "Limpiando archivos de <$(NAME)> "
	-cd Debug && $(MAKE) clean
	@echo "> END................................................................................ "

