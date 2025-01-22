MAKE_PATH=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR=$(MAKE_PATH)/
SRC_DIR=$(MAKE_PATH)/../src/
INCLUDE_DIR=$(MAKE_PATH)/../include/

OUT_EXE=$(BUILD_DIR)output/
OUT_OBJ=$(BUILD_DIR)output/obj/

BUILD := dev

CC=gcc
CSTDFLAGS=-Wall -Wextra -Wno-unused-parameter -Wuninitialized -Werror -std=c89 -pedantic

CFLAGS_OPT.debug=-O0 -g -ggdb -fno-omit-frame-pointer
CFLAGS_OPT.dev=-O0 -g -ggdb -fno-omit-frame-pointer
CFLAGS_OPT.release=-O3 -fno-omit-frame-pointer -march=x86-64-v3
CFLAGS=${CFLAGS_OPT.${BUILD}} ${CSTDFLAGS}
CFLAGS_TEST=${CFLAGS_OPT.dev} ${CSTDFLAGS}

HEADERS=$(INCLUDE_DIR)cpu.h $(INCLUDE_DIR)emulate.h $(INCLUDE_DIR)memory.h 

OBJS=$(OUT_OBJ)cpu.o $(OUT_OBJ)emulate.o $(OUT_OBJ)memory.o

all: $(OUT_EXE)chip

$(OUT_EXE)chip: $(HEADERS) $(OBJS) $(SRC_DIR)main.c
	@mkdir -p $(OUT_EXE)
	$(CC) $(CFLAGS) -o $(OUT_EXE)chip $(OBJS) $(SRC_DIR)main.c

$(OUT_OBJ)cpu.o: $(HEADERS) $(SRC_DIR)cpu.c 
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)cpu.o -c $(SRC_DIR)cpu.c

$(OUT_OBJ)emulate.o: $(HEADERS) $(SRC_DIR)emulate.c 
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)emulate.o -c $(SRC_DIR)emulate.c

$(OUT_OBJ)memory.o: $(HEADERS) $(SRC_DIR)memory.c 
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)memory.o -c $(SRC_DIR)memory.c

clean:
	rm -rf $(OUT_EXE)* $(OUT_OBJ)*.o