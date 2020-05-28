#Version 1.1.0 - 09-05-2020
#Makefile for Windows and Linux developed by Cristobal Medrano Alvarado.
#Makefile para Windows y Linux desarrollado por Cristobal Medrano Alvardo.

#The operating system is checked.
#Se verifica el sistema operativo.
ifeq ($(OS),Windows_NT)
    OS_detected := Windows
else
    OS_detected := $(shell uname -s)
endif

#Executable name.
#Nombre del ejecutable.
EXECUTABLE_NAME = laboratorio_1

#Select the variables to change depending on the OS.
#Se seleccionan las variables a cambiar dependiendo del SO. 

ifeq ($(OS_detected), Windows)
	EXTENSION = .exe
	EXECUTABLE = $(EXECUTABLE_NAME)$(EXTENSION)
	EXECUTE = $(EXECUTABLE)
	DEBUGEXECUTE = debug_$(EXECUTABLE)
    REMOVE = -del
	FILES = $(BUILD)\*.o 

endif
ifeq ($(OS_detected), Linux)
	EXTENSION = .out
	EXECUTABLE = $(EXECUTABLE_NAME)$(EXTENSION)
	EXECUTE = ./$(EXECUTABLE)
	DEBUGEXECUTE = ./debug_$(EXECUTABLE)
    REMOVE = -rm -f
	FILES = $(BUILD)/*.o 
endif

#Modules, headers and build folders.
#Carpeta de modulos, cabeceras y compilados.
HEADERS = include
SOURCES = src
BUILD = build

#Variables
CC = gcc
HDRS = -I $(HEADERS)/
BLDS = $(BUILD)/
FLAGS = -Wall -g3
DEBUG = -D DEBUG

SRCS = $(wildcard $(SOURCES)/*.c)
OBJS = $(subst $(SOURCES)/, $(BUILD)/,$(patsubst %.c,%.o,$(SRCS)))
DOBJS = $(subst $(SOURCES)/, $(BUILD)/,$(patsubst %.c,%_debug.o,$(SRCS)))
MAIN = main
DMAIN = $(MAIN)_debug

.SILENT: all debug clean $(EXECUTABLE) debug_$(EXECUTABLE)
all: $(EXECUTABLE)

# Normal compilation
$(EXECUTABLE): $(OBJS) $(BUILD)/$(MAIN).o
	$(CC) -o $@ $^ -lm
	echo Compilation done. Executable: $(EXECUTE)

$(BUILD)/%.o: $(SOURCES)/%.c | $(BUILD)
	$(CC) $(FLAGS) $(HDRS) -c -o $@ $<

$(BUILD)/$(MAIN).o: $(MAIN).c
	$(CC) $(FLAGS) $(HDRS) -c -o $(BUILD)/$(MAIN).o $(MAIN).c
# End normal compilation

# Debug compilation
debug: debug_$(EXECUTABLE)

debug_$(EXECUTABLE): $(DOBJS) $(BUILD)/$(DMAIN).o
	$(CC) -o $@ $^ -lm
	echo Compilation done. Executable: $(DEBUGEXECUTE)

$(BUILD)/%_debug.o: $(SOURCES)/%.c | $(BUILD)
	$(CC) $(FLAGS) $(HDRS) $(DEBUG) -c -o $@ $<

$(BUILD)/$(DMAIN).o: $(MAIN).c
	$(CC) $(FLAGS) $(HDRS) $(DEBUG) -c -o $(BUILD)/$(DMAIN).o $(MAIN).c
# End debug compilation.

$(BUILD):
	mkdir $@
.PHONY: clean
clean:
	$(REMOVE) $(FILES)
	$(REMOVE) $(EXECUTABLE) debug_$(EXECUTABLE)
	echo Full wipe.