CC ?= gcc
LIB=libflitdb.so
LIB_PATH=/usr/lib
HEADER_PATH=/usr/include/flit.h
OBJ_ARGS=-fPIC -Os -Wno-format-truncation
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
	@$(CC) $(OBJ_ARGS) -c $(SRC)
	@echo " done"
	@echo -n "Generating shared library..."
	@$(CC) -fPIC -s -shared -o $(LIB) $(OBJ)
	@echo " done"

update:
	@sudo cp -v $(LIB) $(LIB_PATH)
	@sudo cp -v flit.h $(HEADER_PATH)

clean:
	@rm -rfv $(OBJ) $(LIB)

done:
	@echo "Created FlitDB shared library!"