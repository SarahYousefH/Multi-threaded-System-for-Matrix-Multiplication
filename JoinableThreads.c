/**
 * Sarah Hassouneh_1210068
 * Approach of joinable threads
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define ROWS 100
#define COLS 100
#define THREADS_NUM 20


// Functions Defenitions:
void generateMatrices(int A[ROWS][COLS], int B[ROWS][COLS]);
void printMatrix(int p[ROWS][COLS]);
void printMatrixPartial(int n, int p[ROWS][COLS]);



//Global Shared Data:

// Create Matrices :
int A[ROWS][COLS];
int B[ROWS][COLS];
int C[ROWS][COLS];

int partSize = ROWS / THREADS_NUM;

void *threadFunc(void *arg)
{

    int start = *(int *)arg;
    int end = start + partSize;


    /* excute matrix partial multiplication
        note: the the result matrix A,B,C is considered to be a global shared data between all threads */
    for (int k = start; k < end; k++) // outer rows

        for (int i = 0; i < ROWS; i++) // one row
        {
            C[k][i] = 0;
            for (int j = 0; j < COLS; j++) // with all columns
            {
                C[k][i] += A[k][j] * B[j][i];
            }
        }

    free(arg);// because it was manually allocated in main 
    return NULL;
}

int main(int argc, char *argv[])
{

    // Welcome Message
    printf("Starting the program! ---> Multiple Threads Num: %d\n", THREADS_NUM);
    fflush(stdout);

    // clock variables
    //clock_t start, end;

    



    // generate Matrices
    generateMatrices(A, B);

    // output results to file, to save for multiple runs and get average
    FILE *file;

    // Open the file in appeand mode mode ("a+")
    file = fopen("Joinever.txt", "a+");

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        return 2;
    }


    // variables to help in assigning the specified rows to each process
    int startRow = 0;

    // array to keep track of threads ids
    pthread_t threads[THREADS_NUM];

    //start = clock(); //-> start of clock

//measuring elapsed time
     struct timeval start_time, end_time;
    long elapsed_seconds, elapsed_microseconds;

    // Record the start time
    if (gettimeofday(&start_time, NULL) != 0) {
        perror("Error getting start time");
        return 1;
    } 


    for (int i = 0; i < THREADS_NUM; i++)
    {

        // this is used to prevent race condition 
        int *threadStart = malloc(sizeof(int));
        *threadStart = startRow;

        // create the thread
        if (pthread_create(&threads[i], NULL, &threadFunc, (void *)threadStart) != 0)
        {
            fprintf(stderr, "Thread Creation failed\n");
            return 3;
        }

        startRow += partSize;
    }

    // joinable threads joining 
    for (int i = 0; i < THREADS_NUM; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Thread Join failed\n");
            return 4;
        }
    }

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


    // print result
    //printMatrix(C);

    //output time 
    //double cpuTime = (end - start) / (double)(CLOCKS_PER_SEC);
   // printf(" CPU Time is %f\n", cpuTime);

    //write results to file
    //fprintf(file, "%f\n", cpuTime);
    fprintf(file, "%ld\n", elapsed_microseconds);


    // Close the file
    fclose(file);

    return 0;

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


/* printf("pthread ID - %lu\n", pthread_self());

    printf("address: %p\n", A);
    //printf("start address: %d\n", &start);
    printf("*******");*/