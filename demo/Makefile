GCC=gcc
SRCDIR=../flitdb
ARGS=-Wall -g -ggdb
LIB_ARGS=-DFLITDB_LIB_DEMO
ifeq ($(OS),Windows_NT)
	LIB_ARGS += -DFLITDB_WIN_LIB
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
	@$(GCC) -o "./build/demo" $(OBJ)
	@echo "Done"

build_lib:
	echo $(OS)
	@cd $(SRCDIR) && $(MAKE)
	@echo -n "\nCompiling with shared library... "
	@$(GCC) $(ARGS) $(LIB_ARGS) -c $(DEMO_FILES)
	@$(GCC) $(ARGS) $(DEMO_FILES:.c=.o) -lflitdb -o "./build/demo"
	@echo "Done"

lib: setup_lib build_lib

run:
	@cd build; ./demo

clean:
	@rm -rfv build $(FILES)
	@rm -rfv build $(OBJ)