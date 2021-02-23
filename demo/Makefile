GCC=gcc
SRCDIR=../flitdb
ARGS=-Wall -g -ggdb
LIB_FILES=flit.h
DEMO_FILES=main.c
FILES:=flit.c $(LIB_FILES) flit_handler.c flit_handler.h flit_misc.c flit_misc.h 
SRC:=flit.c flit_handler.c flit_misc.c $(DEMO_FILES)
OBJ=flit_handler.o flit.o flit_misc.o $(DEMO_FILES:.c=.o)

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
	@cd $(SRCDIR) && $(MAKE)
	@echo -n "\nCompiling with shared library... "
	@$(GCC) $(ARGS) -DFLITDB_LIB_DEMO -c $(DEMO_FILES)
	@$(GCC) $(ARGS) $(DEMO_FILES:.c=.o) -lflitdb -o "./build/demo"
	@echo "Done"

lib: setup_lib build_lib

run:
	@cd build; ./demo

clean:
	@rm -rfv build $(FILES)
	@rm -rfv build $(OBJ)