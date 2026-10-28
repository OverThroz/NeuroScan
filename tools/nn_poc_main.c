#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --- PARAMÈTRES DU RÉSEAU ---
#define INPUT_SIZE 2
#define HIDDEN_SIZE 4
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.5
#define MAX_ITERATIONS 10000
#define ERROR_THRESHOLD 0.01

// --- STRUCTURE DU RÉSEAU ---
typedef struct {
    double weights_input_hidden[INPUT_SIZE][HIDDEN_SIZE];
    double bias_hidden[HIDDEN_SIZE];
    
    double weights_hidden_output[HIDDEN_SIZE][OUTPUT_SIZE];
    double bias_output[OUTPUT_SIZE];
} NeuralNetwork;

// --- FONCTIONS MATHÉMATIQUES ---
// Sigmoïde : écrase les valeurs entre 0 et 1
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Dérivée de la sigmoïde (nécessaire pour la rétropropagation)
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// Génère un nombre aléatoire entre -1 et 1
double random_weight() {
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

// --- INITIALISATION ---
void init_network(NeuralNetwork *nn) {
    srand(time(NULL)); // Initialise le générateur aléatoire
    
    // Initialisation des poids et biais avec des valeurs aléatoires
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            nn->weights_input_hidden[i][j] = random_weight();
        }
    }
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        nn->bias_hidden[j] = random_weight();
    }
    
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        for (int k = 0; k < OUTPUT_SIZE; k++) {
            nn->weights_hidden_output[j][k] = random_weight();
        }
    }
    for (int k = 0; k < OUTPUT_SIZE; k++) {
        nn->bias_output[k] = random_weight();
    }
}

// --- PROPAGATION AVANT (Forward) ---
double forward(NeuralNetwork *nn, double inputs[INPUT_SIZE], double hidden_out[HIDDEN_SIZE]) {
    // 1. Calcul de la couche cachée
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        double sum = nn->bias_hidden[j];
        for (int i = 0; i < INPUT_SIZE; i++) {
            sum += inputs[i] * nn->weights_input_hidden[i][j];
        }
        hidden_out[j] = sigmoid(sum);
    }
    
    // 2. Calcul de la sortie
    double output = nn->bias_output[0];
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        output += hidden_out[j] * nn->weights_hidden_output[j][0];
    }
    return sigmoid(output);
}

// --- ENTRAÎNEMENT (Rétropropagation) ---
void train(NeuralNetwork *nn, double inputs[INPUT_SIZE], double target) {
    double hidden_out[HIDDEN_SIZE];
    
    // 1. Forward (obtenir la prédiction)
    double predicted = forward(nn, inputs, hidden_out);
    
    // 2. Calcul de l'erreur (Erreur quadratique)
    double error = target - predicted;
    
    // 3. Rétropropagation (Calcul des gradients)
    // Delta de la couche de sortie
    double delta_output = error * sigmoid_derivative(predicted);
    
    // Delta de la couche cachée
    double delta_hidden[HIDDEN_SIZE];
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        double error_hidden = delta_output * nn->weights_hidden_output[j][0];
        delta_hidden[j] = error_hidden * sigmoid_derivative(hidden_out[j]);
    }
    
    // 4. Mise à jour des poids et biais (Descente de gradient)
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        nn->weights_hidden_output[j][0] += LEARNING_RATE * delta_output * hidden_out[j];
    }
    nn->bias_output[0] += LEARNING_RATE * delta_output;
    
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            nn->weights_input_hidden[i][j] += LEARNING_RATE * delta_hidden[j] * inputs[i];
        }
    }
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        nn->bias_hidden[j] += LEARNING_RATE * delta_hidden[j];
    }
}

// --- MAIN ---
int main() {
    NeuralNetwork nn;
    init_network(&nn);
    
    // Table de vérité XOR
    double inputs[4][INPUT_SIZE] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double targets[4] = {0, 1, 1, 0};
    
    printf("Début de l'apprentissage du XOR...\n");
    
    int iteration = 0;
    double total_error = 1.0;
    
    // Boucle d'apprentissage
    while (total_error > ERROR_THRESHOLD && iteration < MAX_ITERATIONS) {
        total_error = 0;
        for (int i = 0; i < 4; i++) {
            double hidden_out[HIDDEN_SIZE];
            double prediction = forward(&nn, inputs[i], hidden_out);
            total_error += 0.5 * pow(targets[i] - prediction, 2); // Erreur MSE
            
            train(&nn, inputs[i], targets[i]);
        }
        iteration++;
    }
    
    printf("Apprentissage terminé en %d itérations (Erreur finale : %f).\n\n", iteration, total_error);
    printf("--- Résultats sur la table de vérité ---\n");
    
    // Phase de test
    for (int i = 0; i < 4; i++) {
        double hidden_out[HIDDEN_SIZE];
        double prediction = forward(&nn, inputs[i], hidden_out);
        // On arrondit le résultat pour avoir 0 ou 1
        int result = prediction >= 0.5 ? 1 : 0;
        
        printf("Entrée : %g XOR %g | Attendu : %g | Prédit : %d (valeur brute : %.4f) | %s\n", 
               inputs[i][0], inputs[i][1], targets[i], result, prediction, 
               (result == targets[i]) ? "OK" : "ERREUR");
    }
    
    return 0;
}