#include "neural_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --- FONCTIONS UTILITAIRES (MATHÉMATIQUES) ---

// Génère un poids aléatoire entre -0.5 et 0.5
static double random_weight() {
    return ((double)rand() / RAND_MAX) - 0.5;
}

// Fonction d'activation ReLU (pour la couche cachée)
static double relu(double x) {
    return x > 0 ? x : 0;
}

// Dérivée de ReLU
static double relu_derivative(double x) {
    return x > 0 ? 1.0 : 0.0;
}

// Applique la fonction Softmax (pour la couche de sortie)
static void softmax(double *input, double *output, int size) {
    // On cherche la valeur maximale pour éviter les débordements (overflow) dans exp()
    double max = input[0];
    for (int i = 1; i < size; i++) {
        if (input[i] > max) max = input[i];
    }
    
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        output[i] = exp(input[i] - max);
        sum += output[i];
    }
    
    // Normalisation pour obtenir des probabilités (somme = 1.0)
    for (int i = 0; i < size; i++) {
        output[i] /= sum;
    }
}

// --- INITIALISATION ET LIBÉRATION ---

NeuralNetwork *nn_init() {
    NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
    if (!nn) return NULL;

    nn->input_size = INPUT_SIZE;
    nn->hidden_size = HIDDEN_SIZE;
    nn->output_size = OUTPUT_SIZE;

    // Allocation Couche 1 (Entrée -> Cachée)
    nn->W1 = malloc(INPUT_SIZE * sizeof(double *));
    for (int i = 0; i < INPUT_SIZE; i++) {
        nn->W1[i] = malloc(HIDDEN_SIZE * sizeof(double));
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            nn->W1[i][j] = random_weight();
        }
    }
    nn->b1 = malloc(HIDDEN_SIZE * sizeof(double));
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        nn->b1[i] = 0.0;
    }

    // Allocation Couche 2 (Cachée -> Sortie)
    nn->W2 = malloc(HIDDEN_SIZE * sizeof(double *));
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        nn->W2[i] = malloc(OUTPUT_SIZE * sizeof(double));
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            nn->W2[i][j] = random_weight();
        }
    }
    nn->b2 = malloc(OUTPUT_SIZE * sizeof(double));
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        nn->b2[i] = 0.0;
    }

    return nn;
}

void nn_free(NeuralNetwork *nn) {
    if (!nn) return;

    // Libération de W1
    for (int i = 0; i < INPUT_SIZE; i++) free(nn->W1[i]);
    free(nn->W1);
    free(nn->b1);

    // Libération de W2
    for (int i = 0; i < HIDDEN_SIZE; i++) free(nn->W2[i]);
    free(nn->W2);
    free(nn->b2);

    // Libération de la structure
    free(nn);
}

// --- PROPAGATION AVANT (FORWARD) ---

static void forward(NeuralNetwork *nn, double *input, double *hidden, double *output) {
    // 1. Couche cachée : ReLU(Input * W1 + b1)
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        double sum = nn->b1[j];
        for (int i = 0; i < INPUT_SIZE; i++) {
            sum += input[i] * nn->W1[i][j];
        }
        hidden[j] = relu(sum);
    }

    // 2. Couche sortie : Softmax(Hidden * W2 + b2)
    double *raw_output = malloc(OUTPUT_SIZE * sizeof(double));
    for (int k = 0; k < OUTPUT_SIZE; k++) {
        double sum = nn->b2[k];
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            sum += hidden[j] * nn->W2[j][k];
        }
        raw_output[k] = sum;
    }
    
    softmax(raw_output, output, OUTPUT_SIZE);
    free(raw_output);
}

// --- ENTRAÎNEMENT (RÉTROPROPAGATION) ---

void nn_train(NeuralNetwork *nn, double *input, int target_label) {
    double hidden[HIDDEN_SIZE];
    double output[OUTPUT_SIZE];
    
    // 1. Forward pass
    forward(nn, input, hidden, output);

    // 2. Calcul du gradient de sortie (Cross-Entropy + Softmax = output - target)
    double delta_output[OUTPUT_SIZE];
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        double target = (i == target_label) ? 1.0 : 0.0;
        delta_output[i] = output[i] - target;
    }

    // 3. Rétropropagation vers la couche cachée
    double delta_hidden[HIDDEN_SIZE] = {0};
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        double error = 0.0;
        for (int k = 0; k < OUTPUT_SIZE; k++) {
            error += delta_output[k] * nn->W2[j][k];
            // Mise à jour des poids W2
            nn->W2[j][k] -= LEARNING_RATE * delta_output[k] * hidden[j]; 
        }
        delta_hidden[j] = error * relu_derivative(hidden[j]);
    }

    // 4. Mise à jour de la couche 1 (W1)
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            nn->W1[i][j] -= LEARNING_RATE * delta_hidden[j] * input[i]; 
        }
    }
    
    // 5. Mise à jour des biais
    for(int k = 0; k < OUTPUT_SIZE; k++) {
        nn->b2[k] -= LEARNING_RATE * delta_output[k];
    }
    for(int j = 0; j < HIDDEN_SIZE; j++) {
        nn->b1[j] -= LEARNING_RATE * delta_hidden[j];
    }
}

// --- INFÉRENCE ---

char nn_predict(NeuralNetwork *nn, double *input, double *confidence) {
    double hidden[HIDDEN_SIZE];
    double output[OUTPUT_SIZE];
    
    // Propagation avant pour obtenir les probabilités
    forward(nn, input, hidden, output);

    // Recherche de la probabilité la plus haute (Argmax)
    int best_index = 0;
    double max_prob = output[0];
    for (int i = 1; i < OUTPUT_SIZE; i++) {
        if (output[i] > max_prob) {
            max_prob = output[i];
            best_index = i;
        }
    }
    
    if (confidence != NULL) {
        *confidence = max_prob;
    }
    
    // Retourne le caractère ASCII correspondant ('A' + index)
    return (char)('A' + best_index); 
}

// --- SAUVEGARDE ET CHARGEMENT ---

int nn_save(NeuralNetwork *nn, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;

    // Sauvegarde des matrices de poids et des biais en format binaire
    for (int i = 0; i < INPUT_SIZE; i++) fwrite(nn->W1[i], sizeof(double), HIDDEN_SIZE, f);
    fwrite(nn->b1, sizeof(double), HIDDEN_SIZE, f);
    
    for (int i = 0; i < HIDDEN_SIZE; i++) fwrite(nn->W2[i], sizeof(double), OUTPUT_SIZE, f);
    fwrite(nn->b2, sizeof(double), OUTPUT_SIZE, f);

    fclose(f);
    return 1;
}

NeuralNetwork *nn_load(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    NeuralNetwork *nn = nn_init(); 

    // Lecture des données depuis le fichier
    for (int i = 0; i < INPUT_SIZE; i++) fread(nn->W1[i], sizeof(double), HIDDEN_SIZE, f);
    fread(nn->b1, sizeof(double), HIDDEN_SIZE, f);
    
    for (int i = 0; i < HIDDEN_SIZE; i++) fread(nn->W2[i], sizeof(double), OUTPUT_SIZE, f);
    fread(nn->b2, sizeof(double), OUTPUT_SIZE, f);

    fclose(f);
    return nn;
}

// --- INTERFACE POUR L'ÉQUIPE (LOUIS / MAXIME) ---

static NeuralNetwork *global_nn = NULL;

char ocr_image(const char *img_path) {
    // Chargement du modèle une seule fois (lazy loading)
    if (global_nn == NULL) {
        global_nn = nn_load("weights.bin");
        if (global_nn == NULL) {
            fprintf(stderr, "Erreur: Impossible de charger le modèle weights.bin\n");
            return '?';
        }
    }

    double input[INPUT_SIZE];
    
    // =====================================================================
    
    
    // =====================================================================
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = (double)(rand() % 2); 
    }

    double confidence;
    char letter = nn_predict(global_nn, input, &confidence);

    return letter;
}



#include "neural_net.h"


static NeuralNetwork *global_nn = NULL;

char ocr_image(/* Image *img */ const char *img_path) {
    if (global_nn == NULL) {
        global_nn = nn_load("weights.bin");
        if (global_nn == NULL) {
            printf("Erreur: Impossible de charger le modèle.\n");
            return '?';
        }
    }

    double input[INPUT_SIZE];
    
    
 

    double confidence;
    char letter = nn_predict(global_nn, input, &confidence);

    printf("Lettre prédite : %c (Confiance : %.2f%%)\n", letter, confidence * 100);
    
    return letter;
}