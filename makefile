install:
	mkdir -p build
	-cd dc-commons && $(MAKE) install
	-cd console && $(MAKE) install
	-cd cpu && $(MAKE) install
	-cd fileSystem && $(MAKE) install
	-cd kernel && $(MAKE) install
	-cd memory && $(MAKE) install

uninstall:
	rm -rf build
	-cd dc-commons && $(MAKE) uninstall
	-cd console && $(MAKE) uninstall
	-cd cpu && $(MAKE) uninstall
	-cd fileSystem && $(MAKE) uninstall
	-cd kernel && $(MAKE) uninstall
	-cd memory && $(MAKE) uninstall
