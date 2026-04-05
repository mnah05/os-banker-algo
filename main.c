/*
 * main.c
 * Entry point for the Banker's Algorithm Cloud Computing Simulation.
 *
 * This program demonstrates deadlock avoidance using the Banker's Algorithm.
 * VMs (processes) compete for shared resources (CPU, RAM, Storage, etc.).
 *
 * Flow:
 *   1. Take all input from the user (VMs, resources, matrices)
 *   2. Compute and display Need matrix
 *   3. Display Allocation, Max, Need, and Available in a formatted table
 *   4. Run the Safety Algorithm — report safe/unsafe state with sequence
 *   5. Interactive loop: user can make resource requests, re-check safety
 */

#include <stdio.h>
#include <stdlib.h>
#include "banker.h"

/*
 * display_menu - Show the interactive menu options to the user.
 */
static void display_menu(void)
{
    printf("\n  OPTIONS:\n");
    printf("  [1] Request resources for a VM\n");
    printf("  [2] Display current matrices\n");
    printf("  [3] Run safety algorithm\n");
    printf("  [0] Exit\n");
    printf("  Enter choice: ");
}

/*
 * handle_request - Prompt user for a VM ID and resource request,
 * then process the request through the Banker's Algorithm.
 */
static void handle_request(SystemState *state)
{
    int vm_id;
    int request[MAX_RESOURCES];

    /* Get the VM number from the user */
    printf("\n  Enter VM number (0-%d): ", state->n - 1);
    if (scanf("%d", &vm_id) != 1) {
        printf("  Invalid input. Please enter a number.\n");
        while (getchar() != '\n');
        return;
    }

    /* Validate VM number is in range */
    if (vm_id < 0 || vm_id >= state->n) {
        printf("  Invalid VM number. Must be between 0 and %d.\n", state->n - 1);
        return;
    }

    /* Get the requested resources */
    printf("  Enter requested resources (%d values): ", state->m);
    for (int j = 0; j < state->m; j++) {
        if (scanf("%d", &request[j]) != 1) {
            printf("  Invalid input.\n");
            while (getchar() != '\n');
            return;
        }
        if (request[j] < 0) {
            printf("  Invalid request. Resource values cannot be negative.\n");
            return;
        }
    }

    /* Process the request through the Banker's Algorithm */
    request_resources(state, vm_id, request);

    /* Show updated system state after the request */
    display_matrices(state);
}

int main(void)
{
    SystemState state;
    int safe_sequence[MAX_VMS];
    int choice;

    /* Display welcome banner */
    print_separator();
    printf("   BANKER'S ALGORITHM SIMULATION\n");
    printf("   Deadlock Avoidance in Cloud Computing\n");
    printf("   VMs competing for shared resources\n");
    print_separator();

    /* Step 1: Take all input from the user */
    input_data(&state);

    /* Step 2: Display the complete system state (Allocation, Max, Need, Available) */
    display_matrices(&state);

    /* Step 3: Run initial safety check and report safe/unsafe + sequence */
    printf("  --- Initial Safety Check ---\n");
    safety_algorithm(&state, safe_sequence);

    /* Step 4: Interactive loop for resource requests */
    while (1) {
        display_menu();

        if (scanf("%d", &choice) != 1) {
            printf("  Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            handle_request(&state);
            break;
        case 2:
            display_matrices(&state);
            break;
        case 3:
            printf("\n  --- Running Safety Algorithm ---\n");
            safety_algorithm(&state, safe_sequence);
            break;
        case 0:
            printf("\n  Shutting down. Goodbye!\n\n");
            return 0;
        default:
            printf("  Invalid choice. Please try again.\n");
            break;
        }
    }

    return 0;
}
