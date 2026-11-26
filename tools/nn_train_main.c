#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neural_net.h"

// Fonction factice à remplacer par ta vraie lecture d'image
void load_image_to_input(const char *path, double *input) {
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = (double)(rand() % 2); 
    }
}

int main() {
    srand(time(NULL));
    NeuralNetwork *nn = nn_init();
    
    printf("Début de l'entraînement...\n");
    
    // BOUCLE D'ENTRAÎNEMENT
    int epochs = 10;
    int num_samples = 1000; 
    
    for (int epoch = 0; epoch < epochs; epoch++) {
        for (int i = 0; i < num_samples; i++) {
            double input[INPUT_SIZE];
            int target_label = i % OUTPUT_SIZE; 
            
            // load_image_to_input("dataset/img.bmp", input);
            
            nn_train(nn, input, target_label);
        }
        printf("Epoch %d terminée.\n", epoch + 1);
    }

    // Sauvegarde des poids (crée un fichier weights.bin)
    if (nn_save(nn, "weights.bin")) {
        printf("Modèle sauvegardé avec succès !\n");
    }

    nn_free(nn);
    return 0;
}