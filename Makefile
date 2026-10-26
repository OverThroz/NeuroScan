# Makefile for the OCR Word Search Solver project (EPITA SAE S3)
#
# Constraints from the spec:
# - C language, compiles without error with -Wall -Wextra
# - Mandatory rules: all, clean

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -Werror -pedantic -O2 -g -Iinclude
LDFLAGS = -lm

ifeq ($(ASAN),1)
CFLAGS  += -fsanitize=address -fno-omit-frame-pointer -O0
LDFLAGS += -fsanitize=address
endif

SRC_DIR = src
OBJ_DIR = obj

# Common objects used by both solver and test binaries
COMMON_SRC = $(SRC_DIR)/solver/grid.c \
             $(SRC_DIR)/solver/wordlist.c \
             $(SRC_DIR)/solver/solver.c \
             $(SRC_DIR)/solver/reconstruction.c \
             $(SRC_DIR)/solver/render.c
COMMON_OBJ = $(COMMON_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SOLVER_BIN = solver
SOLVER_OBJ = $(OBJ_DIR)/tools/solver_main.o

TEST_BIN   = test_solver
TEST_OBJ   = $(OBJ_DIR)/tests/test_solver.o

TEST_RECON_BIN = test_reconstruction
TEST_RECON_OBJ = $(OBJ_DIR)/tests/test_reconstruction.o

TEST_RENDER_BIN = test_render
TEST_RENDER_OBJ = $(OBJ_DIR)/tests/test_render.o

PIPELINE_BIN = pipeline_demo
PIPELINE_OBJ = $(OBJ_DIR)/tools/pipeline_demo.o

.PHONY: all clean test

all: $(SOLVER_BIN) $(PIPELINE_BIN)

$(SOLVER_BIN): $(COMMON_OBJ) $(SOLVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_BIN): $(COMMON_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_RECON_BIN): $(COMMON_OBJ) $(TEST_RECON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_RENDER_BIN): $(COMMON_OBJ) $(TEST_RENDER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PIPELINE_BIN): $(COMMON_OBJ) $(PIPELINE_OBJ)
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

test: $(TEST_BIN) $(TEST_RECON_BIN) $(TEST_RENDER_BIN)
	./$(TEST_BIN)
	./$(TEST_RECON_BIN)
	./$(TEST_RENDER_BIN)

clean:
	rm -rf $(OBJ_DIR) $(SOLVER_BIN) $(TEST_BIN) $(TEST_RECON_BIN) $(TEST_RENDER_BIN) $(PIPELINE_BIN)