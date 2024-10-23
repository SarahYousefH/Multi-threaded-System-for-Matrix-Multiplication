/**
 * Sarah Hassouneh_1210068
 * Native approach in C. Solve a matrix multiplication of AxB = C.
 * each will be assumed to hava a size 100X100.
*/

#include <stdio.h>
#include <time.h>

#define ROWS 100
#define COLS 100

// Functions Defenitions:
void printMatrix(int p[ROWS][COLS]);
void simpleMatrixMultiplication(int A[ROWS][COLS], int B[ROWS][COLS], int C[ROWS][COLS]);
void generateMatrices(int A[ROWS][COLS], int B[ROWS][COLS]);

//Global Shared Data:

// Create Matrices :
int A[ROWS][COLS];
int B[ROWS][COLS];
int C[ROWS][COLS];

int main()
{
    // welcome message
    printf("Starting the program! --> Naive Approch \n");
    fflush(stdout);

    // clock variables
    clock_t start, end;

    // generate Matrices
    generateMatrices(A, B);

    // output results to file, to save for multiple runs and get average 
    FILE *file;

    // Open the file in appeand mode mode ("a+") 
    file = fopen("NaiveResults.txt", "a+");

    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return 2;
    }


    start = clock(); //-> start of clock

    // multiply matrices O(n^3) -> Naive Approach
    simpleMatrixMultiplication(A, B, C);

    //printMatrix(C);

    end = clock(); //-> end of clock

    //output time 
    double cpuTime = (end - start) / (double)(CLOCKS_PER_SEC);
    printf(" CPU Time is %f\n", cpuTime);

    //write results to file
    fprintf(file, "%f\n", cpuTime);

    // Close the file
    fclose(file);


    return 0;
}

/**
 * Simple matrix multiplication for square matrices
 */
void simpleMatrixMultiplication(int A[ROWS][COLS], int B[ROWS][COLS], int C[ROWS][COLS])
{

    for (int k = 0; k < ROWS; k++) // outer rows

        for (int i = 0; i < ROWS; i++) // one row
        {
            C[k][i] = 0;
            for (int j = 0; j < COLS; j++) // with all columns
            {
                C[k][i] += A[k][j] * B[j][i];
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

    // filling matrix
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
        {
            A[i][j] = m1[(k) % 7];
            B[i][j] = m2[(k++) % 10];
        }

    // printMatrix(A);
    // printMatrix(B);
}

/**
 * Simple function to print square matrix
 */
void printMatrix(int p[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%4d", p[i][j]);
        }
        printf("\n");
    }

    printf("printing done \n");
}
