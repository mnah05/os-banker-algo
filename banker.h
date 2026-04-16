/*
 * banker.h
 * Header file for Banker's Algorithm simulation.
 * Simplified version focusing only on safety algorithm.
 */

#ifndef BANKER_H
#define BANKER_H

#include <stdbool.h>

#define MAX_VMS       10
#define MAX_RESOURCES 10

/*
 * SystemState holds all matrices and vectors for the Banker's Algorithm.
 */
typedef struct {
    int n;
    int m;
    int allocation[MAX_VMS][MAX_RESOURCES];
    int max[MAX_VMS][MAX_RESOURCES];
    int need[MAX_VMS][MAX_RESOURCES];
    int available[MAX_RESOURCES];
} SystemState;

/*
 * Compute Need matrix: need[i][j] = max[i][j] - allocation[i][j]
 */
void compute_need(SystemState *state);

/*
 * Run the Safety Algorithm (Banker's Algorithm safety check).
 * Returns true if the system is in a safe state.
 * If safe, the safe_sequence array is filled with the VM execution order.
 */
bool safety_algorithm(const SystemState *state, int safe_sequence[]);

#endif /* BANKER_H */
