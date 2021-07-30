CC ?= gcc
SRCDIR=../flitdb
ARGS=-Wall -g -ggdb
LIB_LINK=-lflitdb
BUILD_DIR=./build
TARGET=demo
LIB_FILES=flit.h
DEMO_FILES=main.c
FILES:=flit.c $(LIB_FILES)
SRC:=flit.c $(DEMO_FILES)
OBJ=$(SRC:.c=.o)

.PHONY: build
default: setup build

setup:
	@mkdir -pv build
	@cp -rnv $(FILES:%=$(SRCDIR)/%) .

setup_lib:
	@mkdir -pv build

build:
	@echo -n "Compiling... "
	@$(CC) $(ARGS) -c $(SRC)
	@$(CC) -o $(BUILD_DIR)/$(TARGET) $(OBJ)
	@echo "Done"

build_lib:
	@cd $(SRCDIR) && $(MAKE)
	@echo -n "Compiling with shared library... "
	@$(CC) $(ARGS) -c $(DEMO_FILES)
	@$(CC) $(ARGS) $(DEMO_FILES:.c=.o) $(LIB_LINK) -o $(BUILD_DIR)/$(TARGET)
	@echo "Done"

lib: setup_lib build_lib

run:
	@cd $(BUILD_DIR); ./$(TARGET)

clean:
	@rm -rfv build $(FILES) $(OBJ)