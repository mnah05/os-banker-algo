================================================================================
  BANKER'S ALGORITHM - SIMPLIFIED VERSION
  Deadlock Avoidance Safety Check Only
================================================================================

OVERVIEW
--------
This program implements the Banker's Algorithm safety check for deadlock
avoidance. Virtual machines (VMs) compete for shared resources.

The program focuses only on the core safety algorithm: given a system state,
determine if it's safe (all VMs can finish) and find a safe sequence.

Simplified features:
- No interactive resource requests
- No formatted matrix displays
- Just input → compute need → safety check → result


FILES
-----
  main.c          - Entry point. Prompts for manual input or loads test case.
  banker.h        - Header file with SystemState struct and function prototypes.
  banker.c        - Core safety algorithm implementation.
  test_cases.txt  - Pre-defined test cases (safe and unsafe).
  Makefile        - Build configuration.


HOW TO COMPILE
--------------
Using GCC:

    make

Or manually:

    gcc -o banker main.c banker.c -Wall -Wextra


HOW TO RUN
----------
    ./banker

You will be prompted to:
  1. Enter data manually, or
  2. Load a test case

If choosing test cases, select from:
  1. Safe state (5 VMs, 3 resources) - classic example
  2. Unsafe state (5 VMs, 3 resources)
  3. Small example (3 VMs, 2 resources)


MANUAL INPUT FORMAT
-------------------
The program will ask for:
  1. Number of VMs (1-10)
  2. Number of resource types (1-10)
  3. Available vector (space-separated values)
  4. Max matrix (one row per VM)
  5. Allocation matrix (one row per VM)

Example input for the classic safe case:
  VMs: 5
  Resources: 3
  Available: 3 3 2
  Max:
    VM 0: 7 5 3
    VM 1: 3 2 2
    VM 2: 9 0 2
    VM 3: 2 2 2
    VM 4: 4 3 3
  Allocation:
    VM 0: 0 1 0
    VM 1: 2 0 0
    VM 2: 3 0 2
    VM 3: 2 1 1
    VM 4: 0 0 2

Result: SAFE with sequence 1 -> 3 -> 0 -> 2 -> 4


TEST CASES
----------
The file test_cases.txt contains numbered test cases in this format:
  TEST_CASE <number>
  <n>
  <m>
  <available values>
  <max row 0>
  ...
  <max row n-1>
  <allocation row 0>
  ...
  <allocation row n-1>
  END

You can add your own test cases following this format.