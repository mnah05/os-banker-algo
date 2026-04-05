========================================================================
  BANKER'S ALGORITHM SIMULATION
  Deadlock Avoidance in Cloud Computing
========================================================================

OVERVIEW
--------
This program simulates the Banker's Algorithm for deadlock avoidance in
a cloud computing environment. Virtual machines (VMs) compete for shared
resources such as CPU cores, RAM, and Storage.

The user provides all input — number of VMs, number of resource types,
the Allocation matrix, Max matrix, and Available vector. The program
computes the Need matrix, checks for safe/unsafe state, and allows
interactive resource requests.


FILES
-----
  main.c    - Entry point. Contains the interactive menu loop, user input
              handling, and program flow control. Calls into banker.c for
              all algorithm logic.

  banker.h  - Header file. Defines max limits (MAX_VMS=10, MAX_RESOURCES=10),
              the SystemState struct (allocation, max, need, available
              matrices plus n and m sizes), and function prototypes.

  banker.c  - Core implementation. Contains the input collector, matrix
              display, safety algorithm, and resource request handler
              with step-by-step explanations.


HOW TO COMPILE
--------------
Using GCC (Linux/macOS):

    gcc -o banker main.c banker.c -Wall -Wextra

Using Clang (macOS):

    clang -o banker main.c banker.c -Wall -Wextra

On Windows with MinGW:

    gcc -o banker.exe main.c banker.c -Wall -Wextra


HOW TO RUN
----------
After compiling:

    ./banker          (Linux/macOS)
    banker.exe        (Windows)


USING THE PROGRAM
-----------------
On startup, the program asks for:
  1. Number of VMs (processes)
  2. Number of resource types
  3. Available vector (free instances of each resource)
  4. Max matrix (maximum demand of each VM, row by row)
  5. Allocation matrix (current allocation to each VM, row by row)

Then it automatically:
  - Computes the Need matrix (Need = Max - Allocation)
  - Displays Allocation, Max, Need, and Available in a formatted table
  - Runs the Safety Algorithm and reports SAFE or UNSAFE state
  - If safe, prints the full safe sequence

After that, an interactive menu appears:
  [1] Request resources for a VM
      - Enter a VM number and resource values
      - The program checks step-by-step and grants or denies
      - Updated matrices are shown after each request

  [2] Display current matrices
      - Re-prints the current system state

  [3] Run safety algorithm
      - Re-checks if the current state is safe

  [0] Exit


EXAMPLE INPUT
-------------
  Number of VMs: 5
  Number of resource types: 3
  Available: 3 3 2
  Max matrix:
    VM 0: 7 5 3
    VM 1: 3 2 2
    VM 2: 9 0 2
    VM 3: 2 2 2
    VM 4: 4 3 3
  Allocation matrix:
    VM 0: 0 1 0
    VM 1: 2 0 0
    VM 2: 3 0 2
    VM 3: 2 1 1
    VM 4: 0 0 2

  Result: SAFE state
  Safe sequence: VM 1 -> VM 3 -> VM 0 -> VM 2 -> VM 4


EXAMPLE RESOURCE REQUEST
------------------------
  Enter choice: 1
  Enter VM number: 1
  Enter requested resources: 1 0 2

  Check 1: Request <= Need? YES for all resources
  Check 2: Request <= Available? YES for all resources
  Check 3: Tentatively grant, run safety check -> SAFE
  Result: GRANTED
