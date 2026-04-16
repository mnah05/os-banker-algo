/*
 * banker.c
 * Implementation of Banker's Algorithm for deadlock avoidance.
 * Simplified version focusing only on safety algorithm.
 */

#include <stdio.h>
#include "banker.h"

/*
 * compute_need - Compute Need matrix: need[i][j] = max[i][j] - allocation[i][j]
 */
void compute_need(SystemState *state)
{
    for (int i = 0; i < state->n; i++) {
        for (int j = 0; j < state->m; j++) {
            state->need[i][j] = state->max[i][j] - state->allocation[i][j];
        }
    }
}

/*
 * safety_algorithm - Determine if the system is in a safe state.
 *
 * Algorithm steps:
 * 1. Initialize Work = Available, Finish[i] = false for all VMs
 * 2. Find a VM i where Finish[i] == false and Need[i] <= Work
 * 3. If found: Work = Work + Allocation[i], Finish[i] = true
 * 4. Repeat step 2. If no such VM exists, check if all are finished.
 *
 * Returns true if a safe sequence exists.
 */
bool safety_algorithm(const SystemState *state, int safe_sequence[])
{
    int n = state->n;
    int m = state->m;
    int work[MAX_RESOURCES];
    bool finish[MAX_VMS];
    int seq_index = 0;

    // Initialize work vector and finish flags
    for (int j = 0; j < m; j++) {
        work[j] = state->available[j];
    }
    for (int i = 0; i < n; i++) {
        finish[i] = false;
    }

    // Repeatedly find a VM whose Need can be satisfied by current Work
    for (int count = 0; count < n; count++) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (finish[i]) continue;

            // Check if Need[i] <= Work for all resource types
            bool can_finish = true;
            for (int j = 0; j < m; j++) {
                if (state->need[i][j] > work[j]) {
                    can_finish = false;
                    break;
                }
            }

            if (can_finish) {
                // This VM can finish: simulate it running to completion
                for (int j = 0; j < m; j++) {
                    work[j] += state->allocation[i][j];
                }

                finish[i] = true;
                safe_sequence[seq_index++] = i;
                found = true;
                break; // Restart search from VM 0 with updated Work
            }
        }

        if (!found) {
            // No unfinished VM can proceed
            break;
        }
    }

    // Check if all VMs were able to finish
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            return false; // Unsafe state
        }
    }

    return true; // Safe state
}
