GCC=g++
ARGS=-Wall
SRCDIR=../flitdb
FILES := flit.cpp flit.h flit_handler.cpp flit_handler.h misc.cpp misc.h 

default: setup compile run

setup:
	@mkdir -pv build
	@cp -rnv $(FILES:%=$(SRCDIR)/%) .

compile:
	@echo -n "\nCompiling... "
	@$(GCC) $(ARGS) "./main.cpp" -o "./build/demo"
	@echo "Done"
	@echo "=================\n"
run:
	@cd build; ./demo

clean:
	@rm -rfv build/demo build/demo.db build $(FILES)