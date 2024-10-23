# Multi-Threaded-System-for-Matrix-Multiplication

%# Matrix Multiplication Using Processes and Threads 💻🔢 % commented for now

### Overview

This project demonstrates matrix multiplication using both multi-process and multi-threaded approaches, implemented in C. It explores process and thread management through the POSIX API and Pthreads library. The project was developed to analyze the performance trade-offs between the two approaches in terms of execution time and efficiency.

### Features

- **Naive Approach:** A basic matrix multiplication method using triple nested loops with a time complexity of O(n³).
- **Multi-Process Approach:**
  - Processes are created using `fork()`, with each child process handling specific rows of the matrix.
  - Inter-process communication (IPC) is implemented using pipes.
  - Results are gathered in the parent process for the final output.
- **Multi-Threaded Approach:**
  - Threads are created using Pthreads, with each thread responsible for specific rows of the matrix.
  - Threads share access to common data and variables, ensuring efficient coordination during matrix multiplication.
  - Thread management techniques that were explored included both joinable and detachable threads.

### Procedure

1. **Matrix Generation:** Matrices are generated as per the task guidelines.
2. **Naive Approach:** Basic matrix multiplication without any parallelism.
3. **Process-Based Solution:** Utilizes `fork()` to create child processes for matrix multiplication.
4. **Threaded Solution:** Pthreads are used to perform matrix multiplication in parallel.
5. **Performance Measurement:** Execution time is measured for all approaches across multiple runs to ensure accuracy.

### Performance Comparison 📊

These key average values were collected during testing :

| Approach                               | CPU Time (seconds) | Elapsed Time (microseconds) |
|----------------------------------------|--------------------|-----------------------------|
| Naive Approach                         | 0.00543818         | 183.885                     |
| Multi-Process (4 processes)            | 0.00085626         | 1893.79                     |
| Multi-Thread Joinable (25 threads)     | 0.00614801         | 751.5                       |
| Multi-Thread Detachable (25 threads)   | 2.2838E-3          | 401.76                      |


### Conclusion

This project provides a comparative analysis of multi-process and multi-threaded approaches in matrix multiplication. The threaded solution, particularly with 25 threads, was found to offer the most efficient performance in terms of elapsed time. It was noted that increasing number of threads and processes doesn't neccessarily decrease the time as there is a new overhead and time spent i manging these threads and processes.

### Requirements 🛠️

- **Operating System:** POSIX-compliant (e.g., macOS, Linux).
- **Tools:** Pthreads library, POSIX API.
- **Compiler:** GCC or any POSIX-compliant C compiler.


