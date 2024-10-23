/**
 * Sarah Hassouneh_1210068
 * Creatinng Multiple Processes Approach.
 * The IPC that was used is pipes to comminicate between the child and parent.
 * The child was programmed to return zero in order to prevent exponential growth in the
 * processes for this assignment
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>



#define ROWS 100
#define COLS 100
#define PROCESSES_NUM  50

#define READ_END 0
#define WRITE_END 1

// Functions Defenitions:
void printMatrix(int p[ROWS][COLS]);
void generateMatrices(int A[ROWS][COLS], int B[ROWS][COLS]);
void printMatrixPartial(int n, int p[ROWS][COLS]);
void simpleMatrixMultiplication(int A[ROWS][COLS], int B[ROWS][COLS], int C[ROWS][COLS]);
void childProcess(int start, int end, int A[ROWS][COLS], int B[ROWS][COLS], int n, int C[n][COLS]);

// Global Shared Data:

// Create Matrices :
int A[ROWS][COLS];
int B[ROWS][COLS];
int C[ROWS][COLS];

int partSize = ROWS / PROCESSES_NUM;

int main(int argc, char *argv[])
{

    // Welcome Message
    printf("Starting the program! ---> Multiple Processes Num: %d\n", PROCESSES_NUM);
    fflush(stdout);

    // clock variables
    clock_t start, end;

    // generate Matrices
    generateMatrices(A, B);

    // output results to file, to save for multiple runs and get average
    FILE *file;

    // Open the file in appeand mode mode ("a+")
    file = fopen("MPO.txt", "a+");

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        return 2;
    }

    // variables to help in assigning the specified rows to each process
    int startRow = 0, endRow = partSize;

    // array to keep track of process ids
    pid_t pids[PROCESSES_NUM];

    // array to keep tracks of pipe, to send data from childs to parent
    // the number of pipes is equal to number of processes
    int pipes[PROCESSES_NUM][2];

    // this matrix will hold the partial result calculated by each process
    int partialResult[partSize][COLS];

    // create the pipes, the pipes will be used to transfer data from child to parent
    for (int i = 0; i < PROCESSES_NUM; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            fprintf(stderr, "Pipe failed\n");
            return 3;
        }
    }

    //start = clock(); //-> start of clock

    //measuring elapsed time
    struct timeval start_time, end_time;
    long elapsed_seconds, elapsed_microseconds;

    // Record the start time
    if (gettimeofday(&start_time, NULL) != 0) {
        perror("Error getting start time");
        return 1;
    } 



    /* create the processes */
    for (int i = 0; i < PROCESSES_NUM; i++)
    {

        // the use of fork to create processes
        pids[i] = fork();

        if (pids[i] == -1)
        {
            fprintf(stderr, "Process Creation failed\n");
            return 4;
        }

        if (pids[i] == 0)
        { // Child process

            // pipe management
            for (int j = 0; j < PROCESSES_NUM; j++)
            {
                // close all reads
                close(pipes[j][READ_END]);

                /* close the write  end of pipes that is not specific to this child
                note that every child process kind of inherits those pipes or get a copy that it manages on its own
                that is why this step is neccessarry */
                if (j != i)
                {
                    close(pipes[j][WRITE_END]);
                }
            }

            childProcess(startRow, endRow, A, B, partSize, partialResult);
            //printMatrixPartial(partSize, partialResult);

            // write results to pipe matrix:
            if (write(pipes[i][WRITE_END], &partialResult, sizeof(partialResult)) == -1)
            {
                fprintf(stderr, "Writing failed\n");
                return 5;
            }

            close(pipes[i][WRITE_END]);

            // this prevents exxponential growth of processes
            // return 0;
            exit(0);
        }

        startRow += partSize;
        endRow += partSize;
    }

    //  Main process

    // wait for all children
    for (int i = 0; i < PROCESSES_NUM; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    // Join all partial Results:
    startRow = 0;
    endRow = partSize;

    for (int i = 0; i < PROCESSES_NUM; i++)
    {

        int k[partSize][COLS];

        // read partial sum (partial matrix) through pipe
        if (read(pipes[i][READ_END], k, sizeof(k)) == -1)
        {
            fprintf(stderr, "Reading failed\n");
            return 6;
        }

        int l = 0;
        // printMatrixPartial(partSize, k);

        for (int i = startRow; i < endRow; i++, l++)
        {
            for (int j = 0; j < COLS; j++) // one row
                C[i][j] = k[l][j];
        }

        startRow += partSize;
        endRow += partSize;

        close(pipes[i][READ_END]);
    }

    // print result
    //printMatrix(C);


    //end = clock(); //-> end of clock

     if (gettimeofday(&end_time, NULL) != 0) {
        perror("Error getting end time");
        return 1;
    }

    // Calculate elapsed time
    elapsed_seconds = end_time.tv_sec - start_time.tv_sec;
    elapsed_microseconds = end_time.tv_usec - start_time.tv_usec;

    // Print the elapsed time
    printf("Elapsed time: %ld seconds and %ld microseconds\n", elapsed_seconds, elapsed_microseconds);



    // output time
    //double cpuTime = (end - start) / (double)(CLOCKS_PER_SEC);
    //printf(" CPU Time is %f\n", cpuTime);

    // write results to file
    //fprintf(file, "%f\n", cpuTime);
    fprintf(file, "%ld\n", elapsed_microseconds);


    // Close the file
    fclose(file);
}

/* Matrix multiplication Function (AxB) Given:
    - start: int that represent the number of row where to start multiplication in Rows of A
    - end: int that represent the number of row where to end multiplication in Rows of A
    - Matrix A : The first matrix
    - Matrix B : The Second matrix
    - n : int that represent number of rows in results matrix C
    - C : The result matrix
*/
void childProcess(int start, int end, int A[ROWS][COLS], int B[ROWS][COLS], int n, int C[n][COLS])
{

    int l = 0;

    for (int k = start; k < end; k++, l++) // outer rows

        for (int i = 0; i < ROWS; i++) // one row
        {
            C[l][i] = 0;
            for (int j = 0; j < COLS; j++) // with all columns
            {
                C[l][i] += A[k][j] * B[j][i];
            }
        }
}

/**
 * function to generate and fill matrix A and B
 */
void generateMatrices(int A[ROWS][COLS], int B[ROWS][COLS])
{

    // student Id is m1
    int m1[] = {1, 2, 1, 0, 0, 6, 8}; // m1 array size= 7

    // student* birth year is m2
    // m2 = 1210068 *2003 = 2423766204
    int m2[] = {2, 4, 2, 3, 7, 6, 6, 2, 0, 4}; // m2 array size= 10

    int k = 0;

    // generateMatrices
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
        {
            A[i][j] = m1[(k) % 7];
            B[i][j] = m2[(k++) % 10];
        }

    // printMatrix(A);
    // printMatrix(B);
}

void printMatrix(int p[ROWS][COLS])
{
    printMatrixPartial(ROWS, p);
}

/**
 * Simple function to print partial matrix
 */
void printMatrixPartial(int partSize, int p[partSize][COLS])
{

    for (int i = 0; i < partSize; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%4d", p[i][j]);
        }
        printf("\n");
    }

    printf("printing done \n");
}


