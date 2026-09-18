# Makefile for the OCR Word Search Solver project (EPITA SAE S3)
#
# Constraints from the spec:
# - C language, compiles without error with -Wall -Wextra
# - Mandatory rules: all, clean

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -Werror -pedantic -O2 -g -Iinclude
LDFLAGS =

ifeq ($(ASAN),1)
CFLAGS  += -fsanitize=address -fno-omit-frame-pointer -O0
LDFLAGS += -fsanitize=address
endif

SRC_DIR = src
OBJ_DIR = obj

# Common objects used by both solver and test_solver
COMMON_SRC = $(SRC_DIR)/solver/grid.c \
             $(SRC_DIR)/solver/wordlist.c \
             $(SRC_DIR)/solver/solver.c
COMMON_OBJ = $(COMMON_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOLVER_BIN = solver
SOLVER_OBJ = $(OBJ_DIR)/tools/solver_main.o

TEST_BIN   = test_solver
TEST_OBJ   = $(OBJ_DIR)/tests/test_solver.o

.PHONY: all clean test

all: $(SOLVER_BIN)

$(SOLVER_BIN): $(COMMON_OBJ) $(SOLVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_BIN): $(COMMON_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Generic rule for objects under src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Objects under tools/
$(OBJ_DIR)/tools/%.o: tools/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Objects under tests/
$(OBJ_DIR)/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(OBJ_DIR) $(SOLVER_BIN) $(TEST_BIN)