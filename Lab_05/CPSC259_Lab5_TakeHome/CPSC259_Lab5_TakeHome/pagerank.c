/*
 File:          eigenvalues.c
 Purpose:       Exercise to learn how to calculate eigenvalues using MATLAB from a Visual Studio project
 Author:		Liam Foster and Willem Van Dam
 Student #s:	40199382 and 33500646
 CWLs:		    lifost00 and wvandam
 Date:			November 30th, 2021
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagerank.h"
#include "engine.h"

#define BUFSIZE 256



int main(void) {
    /* Variables */
    int dimension = 0;
    FILE* web_file = NULL;
    int** web = NULL;


    /* Opens and parses the maze file.  Replace the first parameter of fopen with
      different file names defined in the preprocessor section of the header file
      to test your mazesolver with our sample mazes. */
    web_file = fopen(WEB1, "r");

    if (web_file) {

        /* Calls the functions that:
          a) get the size of the maze and stores it in the dimension variable
          b) copies the maze into memory */
          // INSERT YOUR CODE HERE (2 lines)
        dimension = get_web_dimension(web_file);
        web = parse_web(web_file, dimension);

    }
    else {
        fprintf(stderr, "Unable to parse maze file: %s\n", WEB1);
        system("pause");
    }








    /* Variables */
    Engine* ep = NULL; // A pointer to a MATLAB engine object
    mxArray* testArray = NULL, * result = NULL; // mxArray is the fundamental type underlying MATLAB data
    double time[3][3] = { { 1.0, 2.0, 3.0 }, {4.0, 5.0, 6.0 }, {7.0, 8.0, 9.0 } }; // Our test 'matrix', a 2-D array
    char buffer[BUFSIZE + 1];

    /* Starts a MATLAB process */
    if (!(ep = engOpen(NULL))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        system("pause");
        return 1;
    }
    testArray = mxCreateDoubleMatrix(3, 3, mxREAL);

    memcpy((void*)mxGetPr(testArray), (void*)time, 9 * sizeof(double));

    if (engPutVariable(ep, "testArray", testArray)) {
        fprintf(stderr, "\nCannot write test array to MATLAB \n");
        system("pause");
        exit(1); // Same as return 1;
    }

    if (engEvalString(ep, "testArrayEigen = eig(testArray)")) {
        fprintf(stderr, "\nError calculating eigenvalues  \n");
        system("pause");
        exit(1);
    }

    printf("\nRetrieving eigenvector\n");
    if ((result = engGetVariable(ep, "testArrayEigen")) == NULL) {
        fprintf(stderr, "\nFailed to retrieve eigenvalue vector\n");
        system("pause");
        exit(1);
    }
    else {
        size_t sizeOfResult = mxGetNumberOfElements(result);
        size_t i = 0;
        printf("The eigenvalues are:\n");
        for (i = 0; i < sizeOfResult; ++i) {
            printf("%f\n", *(mxGetPr(result) + i));
        }
    }

    if (engOutputBuffer(ep, buffer, BUFSIZE)) {
        fprintf(stderr, "\nCan't create buffer for MATLAB output\n");
        system("pause");
        return 1;
    }
    buffer[BUFSIZE] = '\0';

    engEvalString(ep, "whos"); // whos is a handy MATLAB command that generates a list of all current variables
    printf("%s\n", buffer);

    mxDestroyArray(testArray);
    mxDestroyArray(result);
    testArray = NULL;
    result = NULL;
    if (engClose(ep)) {
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }

    system("pause"); // So the terminal window remains open long enough for you to read it
    return 0; // Because main returns 0 for successful completion
}

int** parse_web(FILE* web_file, int dimension)
{
    /* Variables */
    char        line_buffer[BUFSIZE];
    int         row = 0;
    int         column = 0;
    int** web = NULL;

    /* Allocates memory for correctly-sized web */
    web = (int**)calloc(dimension, sizeof(int*));

    for (row = 0; row < dimension; ++row) {
        web[row] = (int*)calloc(dimension, sizeof(int));
    }

    /* Copies web file to memory */
    row = 0;
    while (fgets(line_buffer, BUFSIZE, web_file)) {
        for (column = 0; column < dimension; ++column) {
            web[row][column] = line_buffer[column];
        }
        row++;
    }
    return web;
}

int get_web_dimension(FILE* web_file) {

    int  dimension = 0;
    char line_buffer[BUFSIZE];

    dimension = strlen(fgets(line_buffer, BUFSIZE, web_file));

    /* You don't need to know this.  It 'resets' the file's internal pointer to the
       beginning of the file. */
    fseek(web_file, 0, SEEK_SET);

    /* Checks if text file was created in Windows and contains '\r'
       IF TRUE reduce strlen by 2 in order to omit '\r' and '\n' from each line
       ELSE    reduce strlen by 1 in order to omit '\n' from each line */
    if (strchr(line_buffer, '\r') != NULL) {
        dimension -= 2;
        return dimension;
    }
    else {
        dimension--;
        return dimension;
    }
}