/*
 * main.c
 * Entry point for simplified Banker's Algorithm.
 * Focuses on safety algorithm only.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banker.h"

void read_manual_input(SystemState *state)
{
    printf("Enter number of VMs (1-%d): ", MAX_VMS);
    scanf("%d", &state->n);
    
    printf("Enter number of resource types (1-%d): ", MAX_RESOURCES);
    scanf("%d", &state->m);
    
    printf("Enter Available vector (%d values): ", state->m);
    for (int j = 0; j < state->m; j++) {
        scanf("%d", &state->available[j]);
    }
    
    printf("Enter Max matrix (%d x %d):\n", state->n, state->m);
    for (int i = 0; i < state->n; i++) {
        printf("  VM %d: ", i);
        for (int j = 0; j < state->m; j++) {
            scanf("%d", &state->max[i][j]);
        }
    }
    
    printf("Enter Allocation matrix (%d x %d):\n", state->n, state->m);
    for (int i = 0; i < state->n; i++) {
        printf("  VM %d: ", i);
        for (int j = 0; j < state->m; j++) {
            scanf("%d", &state->allocation[i][j]);
        }
    }
}

bool load_test_case(SystemState *state, int case_number)
{
    FILE *file = fopen("test_cases.txt", "r");
    if (!file) {
        printf("Error: Could not open test_cases.txt\n");
        return false;
    }
    
    char line[256];
    bool found = false;
    int lines_read = 0;
    
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;
        
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Check for TEST_CASE marker
        if (strncmp(line, "TEST_CASE", 9) == 0) {
            int current_case;
            if (sscanf(line + 9, "%d", &current_case) == 1) {
                if (current_case == case_number) {
                    found = true;
                    lines_read = 0;
                } else if (found) {
                    // We've passed the target test case
                    break;
                }
            }
            continue;
        }
        
        if (!found) continue;
        
        // Check for END marker
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        // Read data based on line number
        if (lines_read == 0) {
            // n
            state->n = atoi(line);
        } else if (lines_read == 1) {
            // m
            state->m = atoi(line);
        } else if (lines_read == 2) {
            // available vector
            char *token = strtok(line, " ");
            for (int j = 0; j < state->m && token; j++) {
                state->available[j] = atoi(token);
                token = strtok(NULL, " ");
            }
        } else if (lines_read <= 2 + state->n) {
            // max matrix rows
            int row = lines_read - 3;
            char *token = strtok(line, " ");
            for (int j = 0; j < state->m && token; j++) {
                state->max[row][j] = atoi(token);
                token = strtok(NULL, " ");
            }
        } else if (lines_read <= 2 + 2 * state->n) {
            // allocation matrix rows
            int row = lines_read - 3 - state->n;
            char *token = strtok(line, " ");
            for (int j = 0; j < state->m && token; j++) {
                state->allocation[row][j] = atoi(token);
                token = strtok(NULL, " ");
            }
        }
        
        lines_read++;
    }
    
    fclose(file);
    
    if (!found) {
        printf("Error: Test case %d not found\n", case_number);
        return false;
    }
    
    return true;
}

void print_result(bool safe, int safe_sequence[], int n)
{
    if (safe) {
        printf("\nSystem is SAFE\n");
        printf("Safe sequence: ");
        for (int i = 0; i < n; i++) {
            printf("%d", safe_sequence[i]);
            if (i < n - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("\nSystem is UNSAFE\n");
        printf("No safe sequence exists\n");
    }
}

int main()
{
    SystemState state;
    int safe_sequence[MAX_VMS];
    int choice;
    
    printf("=== Banker's Algorithm Safety Check ===\n");
    printf("1. Enter data manually\n");
    printf("2. Load test case\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        read_manual_input(&state);
    } else if (choice == 2) {
        printf("Available test cases:\n");
        printf("  1. Safe state (5 VMs, 3 resources)\n");
        printf("  2. Unsafe state (2 VMs, 2 resources)\n");
        printf("  3. Safe small example (2 VMs, 2 resources)\n");
        printf("Enter test case number (1-3): ");
        int case_num;
        scanf("%d", &case_num);
        
        if (!load_test_case(&state, case_num)) {
            printf("Falling back to manual input.\n");
            read_manual_input(&state);
        }
    } else {
        printf("Invalid choice. Using manual input.\n");
        read_manual_input(&state);
    }
    
    // Compute Need matrix
    compute_need(&state);
    
    // Run safety algorithm
    bool safe = safety_algorithm(&state, safe_sequence);
    
    // Print result
    print_result(safe, safe_sequence, state.n);
    
    return 0;
}