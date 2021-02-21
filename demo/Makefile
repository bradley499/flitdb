GCC=gcc
SRCDIR=../flitdb
ARGS=-Wall -g -ggdb
FILES:=flit.c flit.h flit_version.h flit_handler.c flit_handler.h flit_misc.c flit_misc.h 

.PHONY: build
default: setup build

setup:
	@mkdir -pv build
	@cp -rnv $(FILES:%=$(SRCDIR)/%) .

setup_lib:
	@mkdir -pv build

build:
	@echo -n "Compiling... "
	@$(GCC) $(ARGS) "./main.c" -o "./build/demo"
	@echo "Done"

build_lib:
	@cd $(SRCDIR) && $(MAKE)
	@echo -n "\nCompiling with shared library... "
	@$(GCC) $(ARGS) -DFLITDB_LIB_DEMO "./main.c" -lflitdb -o "./build/demo"
	@echo "Done"

lib: setup_lib build_lib

run:
	@cd build; ./demo

clean:
	@rm -rfv build $(FILES)