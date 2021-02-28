GCC=gcc
SRCDIR=../flitdb
ARGS=-Wall -g -ggdb
LIB_ARGS=-DFLITDB_LIB_DEMO
LIB_LINK=-lflitdb
BUILD_DIR=./build
TARGET=demo
ifeq ($(OS),Windows_NT)
	LIB_ARGS += -DFLITDB_WIN_LIB
	LIB_LINK=-L. -lflitdb
	TARGET=demo.exe
endif
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
ifeq ($(OS),Windows_NT)
	@cp -v $(SRCDIR)/flitdb.dll .
endif
	@echo -n "\nCompiling with shared library... "
	@$(GCC) $(ARGS) $(LIB_ARGS) -c $(DEMO_FILES)
	@$(GCC) $(ARGS) $(DEMO_FILES:.c=.o) $(LIB_LINK) -o $(BUILD_DIR)/$(TARGET)
	@echo "Done"
ifeq ($(OS),Windows_NT)
	@mv -v flitdb.dll $(BUILD_DIR)
endif

lib: setup_lib build_lib

run:
	@cd $(BUILD_DIR); ./$(TARGET)

clean:
	@rm -rfv build $(FILES) $(OBJ)