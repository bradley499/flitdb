GCC=g++
SRCDIR=../flitdb
ARGS=-Wall
BASE_HEADER=flit.h flit_version.h
FILES:=flit.cpp $(BASE_HEADER) flit_handler.cpp flit_handler.h misc.cpp misc.h 


.PHONY: build
default: setup build

setup:
	@mkdir -pv build
	@cp -rnv $(FILES:%=$(SRCDIR)/%) .

setup_lib:
	@mkdir -pv build
	@cp -rnv $(BASE_HEADER:%=$(SRCDIR)/%) .

build:
	@echo -n "Compiling... "
	@$(GCC) $(ARGS) "./main.cpp" -o "./build/demo"
	@echo "Done"

build_lib:
	@cd $(SRCDIR) && $(MAKE)
	@echo -n "\nCompiling with shared library... "
	@$(GCC) $(ARGS) -DFLITDB_LIB_DEMO "./main.cpp" -lflitdb -o "./build/demo"
	@echo "Done"

lib: setup_lib build_lib

run:
	@cd build; ./demo

clean:
	@rm -rfv build/demo build/demo.db build $(FILES)