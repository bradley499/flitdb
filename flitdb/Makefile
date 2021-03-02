GCC=gcc
LIB=libflitdb.so
LIB_PATH=/usr/lib
HEADER_PATH=/usr/include/flit.h
OBJ_ARGS=-fPIC -Dflit_lib_compile
LIB_OBJ_ARG=
ifeq ($(shell uname -s),Darwin)
	LIB=libflitdb.dylib
	LIB_PATH=/usr/local/lib
	HEADER_PATH=/usr/local/include
endif
SRC:=flit.c
OBJ=flit.o

default: build update clean done

build:
	@echo -n "Generating object..."
	@$(GCC) $(OBJ_ARGS) -c $(SRC)
	@echo " done"
	@echo -n "Generating shared library..."
	@$(GCC) -s -shared -o $(LIB) $(OBJ)
	@echo " done"

update:
	@sudo cp -v $(LIB) $(LIB_PATH)
	@sudo cp -v flit.h $(HEADER_PATH)

clean:
	@rm -rfv $(OBJ) $(LIB)

done:
	@echo "Created FlitDB shared library!"