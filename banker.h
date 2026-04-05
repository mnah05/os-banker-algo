/*
 * banker.h
 * Header file for Banker's Algorithm simulation.
 * Defines data structures and function prototypes for
 * deadlock avoidance in a cloud computing environment.
 *
 * Uses dynamic sizes — the user inputs the number of
 * VMs (processes) and resource types at runtime.
 */

#ifndef BANKER_H
#define BANKER_H

#include <stdbool.h>

/* Maximum limits for array sizing */
#define MAX_VMS       10
#define MAX_RESOURCES 10

/*
 * SystemState holds all matrices and vectors for the Banker's Algorithm.
 *   n             - number of VMs (processes)
 *   m             - number of resource types
 *   allocation[i][j] - resources of type j currently allocated to VM i
 *   max[i][j]        - maximum demand of VM i for resource type j
 *   need[i][j]       - remaining need: max[i][j] - allocation[i][j]
 *   available[j]     - currently available instances of resource type j
 */
typedef struct {
    int n;  /* number of VMs */
    int m;  /* number of resource types */
    int allocation[MAX_VMS][MAX_RESOURCES];
    int max[MAX_VMS][MAX_RESOURCES];
    int need[MAX_VMS][MAX_RESOURCES];
    int available[MAX_RESOURCES];
} SystemState;

/*
 * Read all input from the user: number of VMs, resources,
 * Allocation matrix, Max matrix, and Available vector.
 * Computes the Need matrix automatically.
 */
void input_data(SystemState *state);

/*
 * Display the Allocation, Max, Need, and Available matrices
 * in a formatted table with clear labels and separators.
 */
void display_matrices(const SystemState *state);

/*
 * Run the Safety Algorithm (Banker's Algorithm safety check).
 * Returns true if the system is in a safe state.
 * If safe, the safe_sequence array is filled with the VM execution order.
 * Prints step-by-step progress to the console.
 */
bool safety_algorithm(const SystemState *state, int safe_sequence[]);

/*
 * Process a resource request from a VM.
 * vm_id: index of the requesting VM (0 to n-1)
 * request: array of requested resources
 *
 * Validates the request against Need and Available, tentatively
 * allocates, runs safety check, and either grants or denies.
 * Prints a step-by-step explanation of the decision.
 *
 * Returns true if the request was granted, false otherwise.
 */
bool request_resources(SystemState *state, int vm_id, int request[]);

/*
 * Print a horizontal separator line for the console UI.
 */
void print_separator(void);

#endif /* BANKER_H */
