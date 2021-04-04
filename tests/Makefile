GCC=gcc
LIB_LINK=-lflitdb
BUILD_DIR=./build
ARGS=-Wall -g -ggdb

SRCDIR=../flitdb
LIB_ARGS=-DFLITDB_LIB_DEMO
LIB_LINK=-lflitdb
TARGET=demo
LIB_FILES=flit.h
DEMO_FILES=main.c
FILES:=flit.c $(LIB_FILES)
SRC:=flit.c $(DEMO_FILES)
OBJ=$(SRC:.c=.o)

.PHONY: build
default: clean setup build

setup:
	@mkdir -pv $(BUILD_DIR)
	@cp -v ../flitdb/flit* .

build:
	@echo -n "Compiling tests... "
	@$(GCC) $(ARGS) -c inserts.c
	@$(GCC) $(ARGS) -c deletions.c
	@$(GCC) $(ARGS) -c flit.c
	@$(GCC) $(ARGS) inserts.o flit.o -o $(BUILD_DIR)/test_insert
	@$(GCC) $(ARGS) deletions.o flit.o -o $(BUILD_DIR)/test_delete
	@echo "Done"

test:
	@mkdir -pv $(BUILD_DIR)
	@echo "Running tests:"
	@echo -n "Sporadic inserts "
	@cd $(BUILD_DIR);./test_insert > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Sporadic deletions "
	@cd $(BUILD_DIR);./test_delete > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"

clean:
	@rm -rfv $(BUILD_DIR) *.o flit.*