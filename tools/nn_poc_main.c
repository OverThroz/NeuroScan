#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --- NETWORK PARAMETERS ---
#define INPUT_SIZE 2
#define HIDDEN_SIZE 4
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.5
#define MAX_ITERATIONS 10000
#define ERROR_THRESHOLD 0.01

// --- NETWORK STRUCTURE ---
typedef struct {
    double weights_input_hidden[INPUT_SIZE][HIDDEN_SIZE];
    double bias_hidden[HIDDEN_SIZE];

    double weights_hidden_output[HIDDEN_SIZE][OUTPUT_SIZE];
    double bias_output[OUTPUT_SIZE];
} NeuralNetwork;

// --- MATH FUNCTIONS ---
// Sigmoid: squashes values between 0 and 1
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Sigmoid derivative (needed for backpropagation)
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// Generates a random number between -1 and 1
double random_weight() {
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

// --- INITIALIZATION ---
void init_network(NeuralNetwork *nn) {
    srand(time(NULL)); // Initialize random generator

    // Initialize weights and biases with random values
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

// --- FORWARD PROPAGATION ---
double forward(NeuralNetwork *nn,
	       	double inputs[INPUT_SIZE], double hidden_out[HIDDEN_SIZE]) {
    // 1. Compute hidden layer
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        double sum = nn->bias_hidden[j];
        for (int i = 0; i < INPUT_SIZE; i++) {
            sum += inputs[i] * nn->weights_input_hidden[i][j];
        }
        hidden_out[j] = sigmoid(sum);
    }

    // 2. Compute output layer
    double output = nn->bias_output[0];
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        output += hidden_out[j] * nn->weights_hidden_output[j][0];
    }
    return sigmoid(output);
}

// --- TRAINING (Backpropagation) ---
void train(NeuralNetwork *nn, double inputs[INPUT_SIZE], double target) {
    double hidden_out[HIDDEN_SIZE];

    // 1. Forward pass (get prediction)
    double predicted = forward(nn, inputs, hidden_out);

    // 2. Compute error (Squared error)
    double error = target - predicted;

    // 3. Backpropagation (Compute gradients)
    // Output layer delta
    double delta_output = error * sigmoid_derivative(predicted);

    // Hidden layer delta
    double delta_hidden[HIDDEN_SIZE];
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        double error_hidden = delta_output * nn->weights_hidden_output[j][0];
        delta_hidden[j] = error_hidden * sigmoid_derivative(hidden_out[j]);
    }

    // 4. Update weights and biases (Gradient descent)
    for (int j = 0; j < HIDDEN_SIZE; j++) {
        nn->weights_hidden_output[j][0]
		+= LEARNING_RATE * delta_output * hidden_out[j];
    }
    nn->bias_output[0] += LEARNING_RATE * delta_output;

    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            nn->weights_input_hidden[i][j]
		    += LEARNING_RATE * delta_hidden[j] * inputs[i];
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

    // XNOR truth table (outputs 1 if inputs are the same, 0 otherwise)
    double inputs[4][INPUT_SIZE] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double targets[4] = {1, 0, 0, 1};

    printf("Starting XNOR training...\n");

    int iteration = 0;
    double total_error = 1.0;

    // Training loop
    while (total_error > ERROR_THRESHOLD && iteration < MAX_ITERATIONS) {
        total_error = 0;
        for (int i = 0; i < 4; i++) {
            double hidden_out[HIDDEN_SIZE];
            double prediction = forward(&nn, inputs[i], hidden_out);
            total_error += 0.5 * pow(targets[i] - prediction, 2); // MSE error

            train(&nn, inputs[i], targets[i]);
        }
        iteration++;
    }

    printf("Training completed in %d iterations (Final error: %f).\n\n"
		    , iteration, total_error);
    printf("--- Truth table results ---\n");

    // Testing phase
    for (int i = 0; i < 4; i++) {
        double hidden_out[HIDDEN_SIZE];
        double prediction = forward(&nn, inputs[i], hidden_out);

        // Round result to 0 or 1
        int result = prediction >= 0.5 ? 1 : 0;

        printf("Input : %g XNOR %g | Expected : %g "
		"| Predicted : %d (raw value: %.4f) | %s\n",
		inputs[i][0], inputs[i][1], targets[i], result, prediction,
		(result == targets[i]) ? "OK" : "ERROR");
    }

    return 0;
}

