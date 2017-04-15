CC=gcc
TARGET_PATH_FINAL="$(shell pwd)/../../"
TARGET_FINAL="$(TARGET_PATH_FINAL)build/"
NAME="console"
HEADERS=$(wildcard *.h)
OBJECTS=$(patsubst %.c, build/%.o, $(wildcard *.c))
LIBS=-lcommons -ldc-commons

all: $(TARGET) move-executable

create-dirs:
	mkdir -p build
	
move-executable:
	mv build/$(NAME) $(TARGET_FINAL)

build/%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@

$(TARGET): create-dirs $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $(NAME)


install: all

clean:
	rm -rf build
	
uninstall: clean

.PHONY: all clean install uninstall
