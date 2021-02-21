GCC=gcc
LIB=libflitdb.so
OBJ=flitdb.o

default: build update clean done

build:
	@echo -n "Generating object..."
	@$(GCC) -fPIC -Dflit_lib_compile -c flit_handler.c -o $(OBJ)
	@echo " done"
	@echo -n "Generating shared library..."
	@$(GCC) -s -shared -o $(LIB) $(OBJ)
	@echo " done"

update:
	@sudo cp -v $(LIB) /usr/lib
	@sudo cp -v flit.h /usr/include/flit.h
	@sudo cp -v flit_version_lib.h /usr/include/flit_version.h

clean:
	@rm -rfv $(OBJ) $(LIB)

done:
	@echo "Created FlitDB shared library!"