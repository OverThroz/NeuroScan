#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include <stddef.h>

#define INPUT_SIZE 784   // 28x28 pixels
#define HIDDEN_SIZE 128  // Couche cachée
#define OUTPUT_SIZE 26   // A-Z
#define LEARNING_RATE 0.01

typedef struct {
    int input_size;
    int hidden_size;
    int output_size;
    
    double **W1; 
    double *b1;
    
    double **W2;
    double *b2;
} NeuralNetwork; // J'ai renommé OCRNetwork en NeuralNetwork pour faire plus standard

// Initialisation et libération
NeuralNetwork *nn_init();
void nn_free(NeuralNetwork *nn);

// Entraînement et Prédiction
void nn_train(NeuralNetwork *nn, double *input, int target_label);
char nn_predict(NeuralNetwork *nn, double *input, double *confidence);

// Sauvegarde et chargement
int nn_save(NeuralNetwork *nn, const char *filename);
NeuralNetwork *nn_load(const char *filename);

// La fonction demandée par ton camarade Louis
char ocr_image(/* Image *img */ const char *img_path); 

#endif // NEURAL_NET_H
