# Makefile for the OCR Word Search Solver project (EPITA SAE S3)
#
# Constraints from the spec:
# - C language, compiles without error with -Wall -Wextra
# - Mandatory rules: all, clean

CC         = cc
CFLAGS     = -std=c11 -Wall -Wextra -Werror -pedantic -O2 -g -Iinclude
SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image)
SDL_LIBS   = $(shell pkg-config --libs sdl2 SDL2_image)
LDFLAGS    = -lm $(SDL_LIBS)

ifeq ($(ASAN),1)
CFLAGS  += -fsanitize=address -fno-omit-frame-pointer -O0
LDFLAGS += -fsanitize=address
endif

SRC_DIR = src
OBJ_DIR = obj

# --- OBJECTS ---

# Solver & Data Structures objects
SOLVER_OBJ = $(OBJ_DIR)/solver/grid.o \
             $(OBJ_DIR)/solver/wordlist.o \
             $(OBJ_DIR)/solver/solver.o \
             $(OBJ_DIR)/solver/reconstruction.o \
             $(OBJ_DIR)/solver/render.o

# Image & Segmentation objects
IMAGE_OBJ  = $(OBJ_DIR)/utils/image.o
PREPRO_OBJ = $(OBJ_DIR)/preprocessing/preprocessing.o
SEG_OBJ    = $(OBJ_DIR)/segmentation/segmentation.o

# Main tools objects
SOLVER_MAIN_OBJ   = $(OBJ_DIR)/tools/solver_main.o
PIPELINE_MAIN_OBJ = $(OBJ_DIR)/tools/pipeline_demo.o
SEGMENT_MAIN_OBJ  = $(OBJ_DIR)/tools/segment_main.o
PREPRO_MAIN_OBJ = $(OBJ_DIR)/tools/preprocess_main.o
NN_POC_MAIN_OBJ = $(OBJ_DIR)/tools/nn_poc_main.o

# Tests objects
TEST_SOLVER_OBJ = $(OBJ_DIR)/tests/test_solver.o
TEST_RECON_OBJ  = $(OBJ_DIR)/tests/test_reconstruction.o
TEST_RENDER_OBJ = $(OBJ_DIR)/tests/test_render.o
TEST_SEG_OBJ    = $(OBJ_DIR)/tests/test_segmentation.o

# --- BINARIES ---

SOLVER_BIN   = solver
PIPELINE_BIN = pipeline_demo
SEGMENT_BIN  = segment
PREPRO_BIN = preprocess
NN_POC_BIN = nn_poc

TEST_SOLVER_BIN = test_solver
TEST_RECON_BIN  = test_reconstruction
TEST_RENDER_BIN = test_render
TEST_SEG_BIN    = test_segmentation

.PHONY: all clean test

# By default, build the main executables
all: $(SOLVER_BIN) $(PIPELINE_BIN) \
	$(SEGMENT_BIN) $(PREPRO_BIN) $(NN_POC_BIN)

# --- LINKING RULES ---

$(SOLVER_BIN): $(SOLVER_OBJ) $(SOLVER_MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PIPELINE_BIN): $(SOLVER_OBJ) $(PIPELINE_MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SEGMENT_BIN): $(SEG_OBJ) $(PREPRO_OBJ) $(IMAGE_OBJ) $(SEGMENT_MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_SOLVER_BIN): $(SOLVER_OBJ) $(TEST_SOLVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_RECON_BIN): $(SOLVER_OBJ) $(TEST_RECON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_RENDER_BIN): $(SOLVER_OBJ) $(TEST_RENDER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PREPRO_BIN): $(PREPRO_OBJ) $(IMAGE_OBJ) $(PREPRO_MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(NN_POC_BIN): $(NN_POC_MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_SEG_BIN): $(SEG_OBJ) $(PREPRO_OBJ) $(IMAGE_OBJ) $(TEST_SEG_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# --- COMPILATION RULES ---

# Generic rule for src/ (solver, utils, preprocessing, segmentation)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

# Rule for tools/
$(OBJ_DIR)/tools/%.o: tools/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

# Rule for tests/
$(OBJ_DIR)/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

# --- ACTIONS ---

test: $(TEST_SOLVER_BIN) $(TEST_RECON_BIN) $(TEST_RENDER_BIN) $(TEST_SEG_BIN)
	./$(TEST_SOLVER_BIN)
	./$(TEST_RECON_BIN)
	./$(TEST_RENDER_BIN)
	./$(TEST_SEG_BIN)

clean:
	rm -rf $(OBJ_DIR) $(SOLVER_BIN) $(PIPELINE_BIN) $(SEGMENT_BIN) \
	       $(TEST_SOLVER_BIN) $(TEST_RECON_BIN) $(TEST_RENDER_BIN) \
	       $(TEST_SEG_BIN) $(PREPRO_BIN) $(NN_POC_BIN) \
	       out out_gray.png out_bw.png out_rotated.png
