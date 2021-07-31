CC ?= gcc
BUILD_DIR=./build
ARGS=-Wall -g -ggdb
SRCDIR=../flitdb

.PHONY: build

default: clean setup build

setup:
	@mkdir -pv $(BUILD_DIR)
	@cp -v $(SRCDIR)/flit.* .

build:
	$(CC) $(ARGS) -c inserts.c
	$(CC) $(ARGS) -c sequential_inserts.c
	$(CC) $(ARGS) -c inverse_sequential_inserts.c
	$(CC) $(ARGS) -c deletions.c
	$(CC) $(ARGS) -c sequential_deletions.c
	$(CC) $(ARGS) -c inverse_sequential_deletions.c
	$(CC) $(ARGS) -c alterations.c
	$(CC) $(ARGS) -c permission_create.c
	$(CC) $(ARGS) -c permission_readonly.c
	$(CC) $(ARGS) -c flit.c
	$(CC) $(ARGS) inserts.o flit.o -o $(BUILD_DIR)/test_insert
	$(CC) $(ARGS) sequential_inserts.o flit.o -o $(BUILD_DIR)/test_sequential_inserts
	$(CC) $(ARGS) inverse_sequential_inserts.o flit.o -o $(BUILD_DIR)/test_inverse_sequential_inserts
	$(CC) $(ARGS) deletions.o flit.o -o $(BUILD_DIR)/test_delete
	$(CC) $(ARGS) sequential_deletions.o flit.o -o $(BUILD_DIR)/test_sequential_delete
	$(CC) $(ARGS) inverse_sequential_deletions.o flit.o -o $(BUILD_DIR)/test_inverse_sequential_delete
	$(CC) $(ARGS) alterations.o flit.o -o $(BUILD_DIR)/test_alterations
	$(CC) $(ARGS) permission_create.o flit.o -o $(BUILD_DIR)/test_permission_create
	$(CC) $(ARGS) permission_readonly.o flit.o -o $(BUILD_DIR)/test_permission_readonly
	sed "s/#define FLITDB_SIZING_MODE FLITDB_SIZING_MODE_BIG/#define FLITDB_SIZING_MODE FLITDB_SIZING_MODE_TINY/g" flit.h > flit2.h && mv flit2.h flit.h
	rm flit.o
	$(CC) $(ARGS) -c flit.c
	$(CC) $(ARGS) -c mmap.c
	$(CC) $(ARGS) -c range.c
	$(CC) $(ARGS) mmap.o flit.o -o $(BUILD_DIR)/test_mmap
	$(CC) $(ARGS) range.o flit.o -o $(BUILD_DIR)/test_range

test:
	@mkdir -pv $(BUILD_DIR)
	@echo "Running tests:"
	@echo -n "Sporadic inserts             "
	@cd $(BUILD_DIR);./test_insert > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Sporadic deletions           "
	@cd $(BUILD_DIR);./test_delete > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Sequential inserts           "
	@cd $(BUILD_DIR);./test_sequential_inserts > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Sequential deletions         "
	@cd $(BUILD_DIR);./test_sequential_delete > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Inverse sequential inserts   "
	@cd $(BUILD_DIR);./test_inverse_sequential_inserts > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Inverse sequential deletions "
	@cd $(BUILD_DIR);./test_inverse_sequential_delete > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Alternating values           "
	@cd $(BUILD_DIR);./test_alterations > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Memory mapped reading        "
	@cd $(BUILD_DIR);./test_mmap > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Permission create            "
	@cd $(BUILD_DIR);./test_permission_create > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"
	@echo -n "Permission readonly          "
	@cd $(BUILD_DIR);./test_permission_readonly > /dev/null 2>&1 && echo "~ Success" || echo "~ Failed"

test_:
	@mkdir -pv $(BUILD_DIR)
	cd $(BUILD_DIR);./test_insert
	cd $(BUILD_DIR);./test_delete
	cd $(BUILD_DIR);./test_sequential_inserts
	cd $(BUILD_DIR);./test_sequential_delete
	cd $(BUILD_DIR);./test_inverse_sequential_inserts
	cd $(BUILD_DIR);./test_inverse_sequential_delete
	cd $(BUILD_DIR);./test_alterations
	cd $(BUILD_DIR);./test_mmap
	cd $(BUILD_DIR);./test_permission_create
	cd $(BUILD_DIR);./test_permission_readonly
clean:
	@rm -rfv $(BUILD_DIR) *.o flit.*
