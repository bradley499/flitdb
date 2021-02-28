GCC=gcc
LIB=libflitdb.so
LIB_PATH=/usr/lib
HEADER_PATH=/usr/include/flit.h
OBJ_ARGS=-fPIC -Dflit_lib_compile
LIB_ARG=-shared
LIB_OBJ_ARG=
ifeq ($(OS),Windows_NT)
	LIB=flitdb.dll
	LIB_OBJ_ARG=-Wl,--subsystem,windows
else ifeq ($(shell uname -s),Darwin)
	LIB=libflitdb.dylib
	LIB_PATH=/usr/local/lib
	LIB_ARG=-dynamiclib
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
	@$(GCC) -s $(LIB_ARG) -o $(LIB) $(OBJ)
	@echo " done"

update:
	$(if !$(filter $(OS),Windows_NT),@sudo cp -v $(LIB) $(LIB_PATH))
	$(if !$(filter $(OS),Windows_NT),@sudo cp -v flit.h $(HEADER_PATH))

clean:
	@rm -rfv $(OBJ) $(if !$(filter $(OS),Windows_NT),$(LIB))

done:
	@echo "Created FlitDB shared library!"