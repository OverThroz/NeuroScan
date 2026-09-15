# Makefile pour le projet OCR Word Search Solver (EPITA SAÉ S3)
#
# Contraintes du Cahier des Charges :
# - Langage C avec compilation sans erreur sous -Wall -Wextra
# - Règles obligatoires : all, clean

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj

# Exécutable CLI pour la première soutenance
SOLVER\_BIN = solver
SOLVER\_SRC = $(SRC\_DIR)/solver.c
SOLVER\_OBJ = $(OBJ\_DIR)/solver.o

.PHONY: all clean

all: $(SOLVER\_BIN)

# Règle de compilation de l'exécutable solver
$(SOLVER\_BIN): $(SOLVER\_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
# Règle générique pour les fichiers objets (.o)
$(OBJ\_DIR)/%.o: $(SRC\_DIR)/%.c
	@mkdir -p $(OBJ\_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des objets et de l'exécutable
clean:
	rm -rf $(OBJ\_DIR) $(SOLVER\_BIN)