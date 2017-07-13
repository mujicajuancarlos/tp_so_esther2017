install:
	@echo "> START................................................................................ "
	@echo "Instalando commons en el sistema "
	-cd commons && sudo $(MAKE) install
	@echo "Compilando el parser"
	-cd parser && $(MAKE) all
	@echo "Instalando parser en el sistema "
	-cd parser && sudo $(MAKE) install
	@echo "> END................................................................................ "

uninstall:
	@echo "> START................................................................................ "
	@echo "Desinstalando commons en el sistema "
	-cd commons && sudo $(MAKE) uninstall
	@echo "Desinstalando parser en el sistema "
	-cd parser && sudo $(MAKE) uninstall
	@echo "> END................................................................................ "
