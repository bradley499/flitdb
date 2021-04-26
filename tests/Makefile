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
	@$(GCC) $(ARGS) -c sequential_inserts.c
	@$(GCC) $(ARGS) -c deletions.c
	@$(GCC) $(ARGS) -c sequential_deletions.c
	@$(GCC) $(ARGS) -c permission_create.c
	@$(GCC) $(ARGS) -c permission_readonly.c
	@$(GCC) $(ARGS) -c flit.c
	@$(GCC) $(ARGS) inserts.o flit.o -o $(BUILD_DIR)/test_insert
	@$(GCC) $(ARGS) sequential_inserts.o flit.o -o $(BUILD_DIR)/test_sequential_inserts
	@$(GCC) $(ARGS) deletions.o flit.o -o $(BUILD_DIR)/test_delete
	@$(GCC) $(ARGS) sequential_deletions.o flit.o -o $(BUILD_DIR)/test_sequential_delete
	@$(GCC) $(ARGS) permission_create.o flit.o -o $(BUILD_DIR)/permission_create
	@$(GCC) $(ARGS) permission_readonly.o flit.o -o $(BUILD_DIR)/permission_readonly
	@echo "Done"

test:
	@mkdir -pv $(BUILD_DIR)
	@echo "Running tests:"
	@echo -n "Sporadic inserts "
	@cd $(BUILD_DIR);./test_insert > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Sporadic deletions "
	@cd $(BUILD_DIR);./test_delete > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Sequential inserts "
	@cd $(BUILD_DIR);./test_sequential_inserts > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Sequential deletions "
	@cd $(BUILD_DIR);./test_sequential_delete > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Permission create "
	@cd $(BUILD_DIR);./permission_create > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Permission readonly "
	@cd $(BUILD_DIR);./permission_readonly > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"

clean:
	@rm -rfv $(BUILD_DIR) *.o flit.*