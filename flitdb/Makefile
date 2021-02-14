GCC=g++
LIB=libflitdb.so
OBJ=flitdb.o

default: build update clean done

build:
	@echo -n "Generating object..."
	@$(GCC) -fPIC -c flit_handler.cpp -o $(OBJ)
	@echo " done"
	@echo -n "Generating shared library..."
	@$(GCC) -s -shared -o $(LIB) $(OBJ)
	@echo " done"

update:
	@sudo cp -v $(LIB) /usr/lib

clean:
	@rm -rfv $(OBJ) $(LIB)

done:
	@echo "Created FlitDB shared library!"