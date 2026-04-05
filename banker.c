/*
 * banker.c
 * Implementation of Banker's Algorithm for deadlock avoidance.
 * Simulates a cloud computing environment where VMs compete
 * for resources like CPU cores, RAM (GB), and Storage (GB).
 *
 * All sizes are dynamic — determined by user input at runtime.
 */

#include <stdio.h>
#include <string.h>
#include "banker.h"

/*
 * print_separator - Prints a visual divider line for clean console output.
 */
void print_separator(void)
{
    printf("============================================================\n");
}

/*
 * input_data - Collect all input from the user.
 *
 * Prompts for:
 *   1. Number of VMs (processes)
 *   2. Number of resource types
 *   3. Available vector (currently available instances of each resource)
 *   4. Max matrix (maximum demand of each VM)
 *   5. Allocation matrix (currently allocated to each VM)
 *
 * Then computes:
 *   Need matrix: need[i][j] = max[i][j] - allocation[i][j]
 */
void input_data(SystemState *state)
{
    /* Step 1: Get the number of VMs */
    printf("\n  Enter the number of VMs (processes): ");
    scanf("%d", &state->n);

    /* Step 2: Get the number of resource types */
    printf("  Enter the number of resource types: ");
    scanf("%d", &state->m);

    /*
     * Step 3: Get the Available vector.
     * These are the resources currently free in the system.
     */
    printf("\n  Enter the Available vector (%d values):\n", state->m);
    printf("  (e.g., available instances of each resource type)\n");
    printf("  >> ");
    for (int j = 0; j < state->m; j++) {
        scanf("%d", &state->available[j]);
    }

    /*
     * Step 4: Get the Max matrix.
     * Each row represents the maximum demand of a VM for each resource.
     */
    printf("\n  Enter the Max matrix (%d x %d):\n", state->n, state->m);
    printf("  (maximum resources each VM may ever need)\n");
    for (int i = 0; i < state->n; i++) {
        printf("  VM %d >> ", i);
        for (int j = 0; j < state->m; j++) {
            scanf("%d", &state->max[i][j]);
        }
    }

    /*
     * Step 5: Get the Allocation matrix.
     * Each row represents resources currently allocated to a VM.
     */
    printf("\n  Enter the Allocation matrix (%d x %d):\n", state->n, state->m);
    printf("  (resources currently allocated to each VM)\n");
    for (int i = 0; i < state->n; i++) {
        printf("  VM %d >> ", i);
        for (int j = 0; j < state->m; j++) {
            scanf("%d", &state->allocation[i][j]);
        }
    }

    /*
     * Compute the Need matrix automatically.
     * Need[i][j] = Max[i][j] - Allocation[i][j]
     * This tells us how many more resources each VM might still request.
     */
    printf("\n  Computing Need matrix (Need = Max - Allocation)...\n");
    for (int i = 0; i < state->n; i++) {
        for (int j = 0; j < state->m; j++) {
            state->need[i][j] = state->max[i][j] - state->allocation[i][j];
        }
    }
}

/*
 * display_matrices - Print all matrices in a clean formatted table.
 * Shows Allocation, Max, Need side by side, plus Available vector.
 * Works for any number of VMs and resources.
 */
void display_matrices(const SystemState *state)
{
    int m = state->m;

    print_separator();
    printf("       CURRENT SYSTEM STATE (Cloud Resource Allocation)\n");
    print_separator();

    /* Print header row: Allocation | Max | Need with R0, R1, R2... labels */
    printf("       | Allocation");
    /* Pad to align columns: each resource takes 4 chars */
    for (int j = 0; j < m; j++) printf("    ");
    printf("| Max");
    for (int j = 0; j < m; j++) printf("    ");
    printf("| Need\n");

    /* Resource labels row */
    printf("  VM   |");
    for (int k = 0; k < 3; k++) { /* 3 sections: Alloc, Max, Need */
        printf(" ");
        for (int j = 0; j < m; j++) {
            printf(" R%-2d", j);
        }
        printf(" |");
    }
    printf("\n");

    /* Separator line */
    printf("-------|");
    for (int k = 0; k < 3; k++) {
        for (int j = 0; j < m * 4 + 1; j++) printf("-");
        printf("|");
    }
    printf("\n");

    /* One row per VM showing all three matrices side by side */
    for (int i = 0; i < state->n; i++) {
        printf("  VM %d |", i);

        /* Allocation */
        printf(" ");
        for (int j = 0; j < m; j++) printf(" %2d ", state->allocation[i][j]);
        printf(" |");

        /* Max */
        printf(" ");
        for (int j = 0; j < m; j++) printf(" %2d ", state->max[i][j]);
        printf(" |");

        /* Need */
        printf(" ");
        for (int j = 0; j < m; j++) printf(" %2d ", state->need[i][j]);
        printf("\n");
    }

    /* Available vector displayed below the table */
    printf("\n  Available: ");
    for (int j = 0; j < m; j++) {
        printf("R%d=%d  ", j, state->available[j]);
    }
    printf("\n");
    print_separator();
}

/*
 * safety_algorithm - Determine if the system is in a safe state.
 *
 * The algorithm simulates resource allocation to see if all VMs
 * can eventually finish. It works by:
 *   1. Initialize Work = Available, Finish[i] = false for all VMs
 *   2. Find a VM i where Finish[i] == false and Need[i] <= Work
 *   3. If found: Work = Work + Allocation[i], Finish[i] = true
 *      (VM finishes and releases all its allocated resources)
 *   4. Repeat step 2. If no such VM exists, check if all are finished.
 *
 * Returns true if a safe sequence exists (all VMs can finish).
 */
bool safety_algorithm(const SystemState *state, int safe_sequence[])
{
    int n = state->n;
    int m = state->m;
    int work[MAX_RESOURCES];
    bool finish[MAX_VMS];
    int seq_index = 0;

    /* Step 1: Initialize work vector and finish flags */
    for (int j = 0; j < m; j++) {
        work[j] = state->available[j];
    }
    for (int i = 0; i < n; i++) {
        finish[i] = false;
    }

    printf("\n  [Safety Algorithm] Starting safety check...\n");
    printf("  Initial Work = [ ");
    for (int j = 0; j < m; j++) printf("%d ", work[j]);
    printf("]\n");

    /*
     * Step 2-4: Repeatedly find a VM whose Need can be satisfied
     * by the current Work vector. Each pass finds at most one VM.
     * We need n passes if a safe sequence exists.
     */
    for (int count = 0; count < n; count++) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (finish[i]) continue; /* Skip already-finished VMs */

            /* Check if Need[i] <= Work for all resource types */
            bool can_finish = true;
            for (int j = 0; j < m; j++) {
                if (state->need[i][j] > work[j]) {
                    can_finish = false;
                    break;
                }
            }

            if (can_finish) {
                /*
                 * This VM can finish: simulate it running to completion.
                 * It releases all its allocated resources back to the pool.
                 */
                printf("  Step %d: VM %d can finish  (Need [ ", count + 1, i);
                for (int j = 0; j < m; j++) printf("%d ", state->need[i][j]);
                printf("] <= Work [ ");
                for (int j = 0; j < m; j++) printf("%d ", work[j]);
                printf("])\n");

                /* Work = Work + Allocation[i] (VM releases its resources) */
                for (int j = 0; j < m; j++) {
                    work[j] += state->allocation[i][j];
                }

                printf("         VM %d releases resources -> Work = [ ", i);
                for (int j = 0; j < m; j++) printf("%d ", work[j]);
                printf("]\n");

                finish[i] = true;
                safe_sequence[seq_index++] = i;
                found = true;
                break; /* Restart search from VM 0 with updated Work */
            }
        }

        if (!found) {
            /* No unfinished VM can proceed — deadlock risk */
            break;
        }
    }

    /* Check if all VMs were able to finish */
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            printf("\n  ****************************************************\n");
            printf("  *  RESULT: System is in an UNSAFE state!           *\n");
            printf("  *  VM %d cannot finish — potential DEADLOCK!        *\n", i);
            printf("  ****************************************************\n\n");
            return false;
        }
    }

    /* Print the safe sequence */
    printf("\n  ****************************************************\n");
    printf("  *  RESULT: System is in a SAFE state!              *\n");
    printf("  ****************************************************\n");
    printf("  Safe sequence: < ");
    for (int i = 0; i < n; i++) {
        printf("VM %d", safe_sequence[i]);
        if (i < n - 1) printf(" -> ");
    }
    printf(" >\n\n");

    return true;
}

/*
 * request_resources - Handle a resource request from a VM.
 *
 * Three checks are performed in order:
 *   1. Request <= Need   (VM isn't asking more than it declared)
 *   2. Request <= Available (resources are actually free)
 *   3. Tentatively grant and run safety algorithm
 *
 * If all three pass, the request is granted permanently.
 * If any fail, the request is denied and state is rolled back.
 */
bool request_resources(SystemState *state, int vm_id, int request[])
{
    int m = state->m;

    print_separator();
    printf("  RESOURCE REQUEST from VM %d: [ ", vm_id);
    for (int j = 0; j < m; j++) printf("%d ", request[j]);
    printf("]\n");
    print_separator();

    /*
     * Check 1: Request must not exceed the VM's remaining Need.
     * A VM should never request more than its declared maximum.
     */
    printf("\n  [Check 1] Request <= Need?\n");
    for (int j = 0; j < m; j++) {
        printf("    R%d: Request(%d) <= Need(%d) ? %s\n",
               j, request[j], state->need[vm_id][j],
               request[j] <= state->need[vm_id][j] ? "YES" : "NO");
        if (request[j] > state->need[vm_id][j]) {
            printf("\n  ** DENIED: VM %d requested more R%d than its declared max need. **\n",
                   vm_id, j);
            printf("  ** Error: process has exceeded its maximum claim. **\n\n");
            return false;
        }
    }
    printf("  -> Check 1 PASSED\n");

    /*
     * Check 2: Request must not exceed Available resources.
     * If not enough resources are free, the VM must wait.
     */
    printf("\n  [Check 2] Request <= Available?\n");
    for (int j = 0; j < m; j++) {
        printf("    R%d: Request(%d) <= Available(%d) ? %s\n",
               j, request[j], state->available[j],
               request[j] <= state->available[j] ? "YES" : "NO");
        if (request[j] > state->available[j]) {
            printf("\n  ** DENIED: Not enough R%d available. VM %d must wait. **\n\n",
                   j, vm_id);
            return false;
        }
    }
    printf("  -> Check 2 PASSED\n");

    /*
     * Check 3: Tentatively allocate and check safety.
     * Pretend we granted the request and see if the system stays safe.
     */
    printf("\n  [Check 3] Tentatively granting and checking safety...\n");

    /* Tentatively allocate the requested resources */
    for (int j = 0; j < m; j++) {
        state->available[j]          -= request[j];
        state->allocation[vm_id][j]  += request[j];
        state->need[vm_id][j]        -= request[j];
    }

    /* Run safety algorithm on the tentative state */
    int safe_sequence[MAX_VMS];
    bool safe = safety_algorithm(state, safe_sequence);

    if (safe) {
        printf("  ** GRANTED: Request from VM %d has been approved! **\n\n", vm_id);
        return true;
    } else {
        /*
         * Unsafe state detected — roll back the tentative allocation.
         * The VM must wait for resources to become available later.
         */
        printf("  ** DENIED: Granting would lead to an UNSAFE state. **\n");
        printf("  ** Rolling back. VM %d must wait. **\n\n", vm_id);

        for (int j = 0; j < m; j++) {
            state->available[j]          += request[j];
            state->allocation[vm_id][j]  -= request[j];
            state->need[vm_id][j]        += request[j];
        }
        return false;
    }
}
