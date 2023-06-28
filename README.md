# JobShop

First includes the first solution for this problem, where each operation startTime are determined by adding the previous operation times to a global variable.
It reads the input.txt and writes to the output.txt file of its directory.
Execution:

gcc main.c -o main.c
./main.exe

Possibility includes the alternative solution where it was attepted to create a Branch & Bound solution for the Job-Shop problem

# Generate Input

This allows the creation of a Fisher and Thompson diagram
gcc generateInput.c -o generateInput
./generateInput.exe

During the execution of this programm the input.txt is populated.

# Run Program

- Sequential programm:
  gcc sequencial.c -o sequencial
  ./main.exe input.txt output.txt

- Parallel programm:
  gcc -pthread main.c -o main
  ./main.exe input.txt output.txt <num_threads>

# Operation Results

- **For this operation you need to updade you context path present in both sequencialResults.c and parallelResults.c**

- Sequential Results:
  gcc sequencialResults.c -o sequencialResults
  ./sequencialResults.exe

- Parallel Results:
  gcc parallelResults.c -o parallelResults
  ./parallelResults.exe <num_threads>

- Obtain operation results:
  Operation time is written in the results.txt file.
