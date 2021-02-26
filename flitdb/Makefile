GCC=gcc
LIB=libflitdb.so
SRC:=flit.c
OBJ=flit.o

default: build update clean done

build:
	@echo -n "Generating object..."
	@$(GCC) -fPIC -Dflit_lib_compile -c $(SRC)
	@echo " done"
	@echo -n "Generating shared library..."
	@$(GCC) -s -shared -o $(LIB) $(OBJ)
	@echo " done"

update:
	@sudo cp -v $(LIB) /usr/lib
	@sudo cp -v flit.h /usr/include/flit.h

clean:
	@rm -rfv $(OBJ) $(LIB)

done:
	@echo "Created FlitDB shared library!"