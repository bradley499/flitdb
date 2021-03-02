GCC=gcc
SRCDIR=../flitdb
ARGS=-Wall -g -ggdb
LIB_ARGS=-DFLITDB_LIB_DEMO
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
	@$(GCC) $(ARGS) -c $(SRC)
	@$(GCC) -o $(BUILD_DIR)/$(TARGET) $(OBJ)
	@echo "Done"

build_lib:
	@cd $(SRCDIR) && $(MAKE)
	@echo -n "Compiling with shared library... "
	@$(GCC) $(ARGS) $(LIB_ARGS) -c $(DEMO_FILES)
	@$(GCC) $(ARGS) $(DEMO_FILES:.c=.o) $(LIB_LINK) -o $(BUILD_DIR)/$(TARGET)
	@echo "Done"

lib: setup_lib build_lib

run:
	@cd $(BUILD_DIR); ./$(TARGET)

clean:
	@rm -rfv build $(FILES) $(OBJ)