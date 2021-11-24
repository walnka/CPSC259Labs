/*
 File:          multmatrix.c
 Purpose:       Exercise to learn how to calculate eigenvalues using MATLAB from a Visual Studio project
 Author:		Liam Foster and Willem Van Dam
 Student #s:	40199382 and 33500646
 CWLs:		    lifost00 and wvandam
 Date:			November 23rd, 2021
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

#define BUFSIZE 256

int main(void) {
    /* Variables */
    Engine* ep = NULL; // A pointer to a MATLAB engine object
    mxArray* mxMatrixOne = NULL, * mxMatrixTwo = NULL, * result = NULL, * mxMatrixOneT, * mxMatrixTwoT; // mxArray is the fundamental type underlying MATLAB data
    double matrixOne[3][3] = { { 1.0, 2.0, 3.0 }, {4.0, 5.0, 6.0 }, {7.0, 8.0, 9.0 } }; // Our test 'matrix', a 2-D array
    double matrixTwo[3][3] = { { 2.0, 2.0, 2.0 }, {3.0, 3.0, 3.0 }, {4.0, 4.0, 4.0 } };
    char buffer[BUFSIZE + 1];

    /* Starts a MATLAB process */
    if (!(ep = engOpen(NULL))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        system("pause");
        return 1;
    }
    // Creates a 2-D double-precision floating-point array initialized to 0, or returns NULL on failure. 
    mxMatrixOne = mxCreateDoubleMatrix(3, 3, mxREAL);
    mxMatrixTwo = mxCreateDoubleMatrix(3, 3, mxREAL);

    // Copies the array from the local 2-D array "time" to the MATLAB array "testArray"
    memcpy((void*)mxGetPr(mxMatrixOne), (void*)matrixOne, 9 * sizeof(double));
    memcpy((void*)mxGetPr(mxMatrixTwo), (void*)matrixTwo, 9 * sizeof(double));

    // Places the test array in the MATLAB workspace
    if (engPutVariable(ep, "mxMatrixOne", mxMatrixOne)) {
        fprintf(stderr, "\nCannot write matrixOne to MATLAB \n");
        system("pause");
        exit(1); // Same as return 1;
    }
    if (engPutVariable(ep, "mxMatrixTwo", mxMatrixTwo)) {
        fprintf(stderr, "\nCannot write matrixTwo to MATLAB \n");
        system("pause");
        exit(1); // Same as return 1;
    }

    // Calculates the product of the two matrices using the MATLAB engine
    if (engEvalString(ep, "productArray = mxMatrixOne * mxMatrixTwo")) {
        fprintf(stderr, "\nError calculating product  \n");
        system("pause");
        exit(1);
    }

    //Transposes mxMatrixOne, mxMatrixTwo, and productArray for easier printing in Visual Studio
    if (engEvalString(ep, "mxMatrixOneT = mxMatrixOne.'")) {
        fprintf(stderr, "\nError transposing mxMatrixOne\n");
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "mxMatrixTwoT = mxMatrixTwo.'")) {
        fprintf(stderr, "\nError transposing mxMatrixTwo\n");
        system("pause");
        exit(1);
    }
    if (engEvalString(ep, "productArrayT = productArray.'")) {
        fprintf(stderr, "\nError transposing mxMatrixTwo\n");
        system("pause");
        exit(1);
    }

    // Retrieves the product and transposed matrices
    //printf("\nRetrieving product\n");
    if ((result = engGetVariable(ep, "productArrayT")) == NULL) {
        fprintf(stderr, "\nFailed to retrieve product matrix\n");
        system("pause");
        exit(1);
    }
    if ((mxMatrixOneT = engGetVariable(ep, "mxMatrixOneT")) == NULL) {
        fprintf(stderr, "\nFailed to retrieve MatrixOne transpose\n");
        system("pause");
        exit(1);
    }
    if ((mxMatrixTwoT = engGetVariable(ep, "mxMatrixTwoT")) == NULL) {
        fprintf(stderr, "\nFailed to retrieve mxMatrixTwo transpose\n");
        system("pause");
        exit(1);
    }

    else {
        size_t sizeOfResult = mxGetNumberOfElements(result);
        size_t i = 0;
        //size_t j = 3;
        size_t column_count = 0;
        printf("The first matrix was:\n");
        for (i = 0; i < sizeOfResult; i++) {
            printf("%f ", *(mxGetPr(mxMatrixOneT) + i));
            column_count++;
            if (column_count == 3) {
                printf("\n");
                column_count = 0;
            }
        }

        column_count = 0;
        printf("\nThe second matrix was:\n");
        for (i = 0; i < sizeOfResult; i++) {
            printf("%f ", *(mxGetPr(mxMatrixTwoT) + i));
            column_count++;
            if (column_count == 3) {
                printf("\n");
                column_count = 0;
            }
        }

        column_count = 0;
        printf("\nThe matrix product is:\n");
        for (i = 0; i < sizeOfResult; i++) {
            printf("%f ", *(mxGetPr(result) + i));
            column_count++;
            if (column_count == 3) {
                printf("\n");
                column_count = 0;
            }
        }
    }

    // Null-terminates the buffer
    if (engOutputBuffer(ep, buffer, BUFSIZE)) {
        fprintf(stderr, "\nCan't create buffer for MATLAB output\n");
        system("pause");
        return 1;
    }
    buffer[BUFSIZE] = '\0';


    engEvalString(ep, "whos"); // whos is a handy MATLAB command that generates a list of all current variables
    printf("\n%s\n", buffer);

    // Closes connection to MATALB
    mxDestroyArray(mxMatrixOne);
    mxDestroyArray(mxMatrixTwo);
    mxDestroyArray(result);
    mxMatrixOne = NULL;
    mxMatrixTwo = NULL;
    result = NULL;
    if (engClose(ep)) {
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }

    system("pause"); // So the terminal window remains open long enough for you to read it
    return 0; // Because main returns 0 for successful completion
}