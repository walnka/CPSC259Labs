/*
 File:          eigenvalues.c
 Purpose:       Exercise to learn how to calculate eigenvalues using MATLAB from a Visual Studio project
 Author:		Liam Foster and Willem Van Dam
 Student #s:	40199382 and 33500646
 CWLs:		    lifost00 and wvandam
 Date:			November 30th, 2021
*/

#define _CRT_SECURE_NO_WARNINGS

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
    double* web = NULL;


    /* Opens and parses the web file.*/
    web_file = fopen(WEB1, "r");

    if (web_file) {

        dimension = get_web_dimension(web_file);
        web = parse_web(web_file, dimension);

    }
    else {
        fprintf(stderr, "Unable to parse maze file: %s\n", WEB1);
        system("pause");
    }


    /* Variables */
    Engine* ep = NULL; // A pointer to a MATLAB engine object
    mxArray* webArray = NULL, * result = NULL; // mxArray is the fundamental type underlying MATLAB data
    //char buffer[BUFSIZE + 1]; For Debugging


    /* Starts a MATLAB process */
    if (!(ep = engOpen(NULL))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        system("pause");
        return 1;
    }

    //Defines matlab matrix webArray and copies parsed web into it
    webArray = mxCreateDoubleMatrix(dimension, dimension, mxREAL);
    memcpy((void*)mxGetPr(webArray), (void*)web, sizeof(double) * dimension * dimension);

    //Imports webArray into matlab
    if (engPutVariable(ep, "webArray", webArray)) {
        fprintf(stderr, "\nCannot write web array to MATLAB \n");
        system("pause");
        exit(1); // Same as return 1;
    }

  /*For Debugging  
    if (engOutputBuffer(ep, buffer, BUFSIZE)) { 
        fprintf(stderr, "\nCan't create buffer for MATLAB output\n");
        system("pause");
        return 1;
    }*/

    //Calculates PageRank
    if (engEvalString(ep, "dimension = size(ConnectivityMatrix, 1); columnsums = sum(ConnectivityMatrix, 1);")) {
        fprintf(stderr, "\nError calculating dimension and columnsums \n");
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "p = 0.85; zerocolumns = find(columnsums~=0);")) {
        fprintf(stderr, "\nError calculating p and zerocolumns \n");
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "D = sparse( zerocolumns, zerocolumns, 1./columnsums(zerocolumns), dimension, dimension); StochasticMatrix = ConnectivityMatrix * D; ")) {
        fprintf(stderr, "\nError calculating D and StochasticMatrix \n");
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "[row, column] = find(columnsums==0); StochasticMatrix(:, column) = 1. / dimension; ")) {
        fprintf(stderr, "\nError calculating rows, columns, and adjusting StochasticMatrix \n");
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "Q = ones(dimension, dimension); TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q / dimension);")) {
        fprintf(stderr, "\nError calculating Q and TransitionMatrix \n");
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "rank = ones(dimension, 1); for i = 1:100 rank = TransitionMatrix * rank; end")) {
        fprintf(stderr, "\nError iterating rank \n");
        system("pause");
        exit(1);
    }

    if (engEvalString(ep, "rank = rank / sum(rank);")) {
        fprintf(stderr, "\nError calculating rank \n");
        system("pause");
        exit(1);
    }

    printf("\nRetrieving pagerank\n");
    //Gets page rank from matlab
    if ((result = engGetVariable(ep, "rank")) == NULL) {
        fprintf(stderr, "\nFailed to retrieve pagerank\n");
        system("pause");
        exit(1);
    }

    //Writes pagerank output
    else {
        size_t i = 0;
        printf("NODE\tRANK\n---\t----\n");
        for (i = 0; i < dimension; ++i) {
            printf("%d\t%f\n", i+1,*(mxGetPr(result) + i));
        }
    }

    mxDestroyArray(webArray);
    mxDestroyArray(result);
    webArray = NULL;
    result = NULL;
    if (engClose(ep)) {
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }

    system("pause"); // So the terminal window remains open long enough for you to read it
    return 0; // Because main returns 0 for successful completion
}

double* parse_web(FILE* web_file, int dimension)
{
    /* Variables */
    char        line_buffer[BUFSIZE];
    int         row = 0;
    int         column = 0;
    double* web = NULL;

    //Initializes 1D web array
    web = (double*)calloc(dimension * dimension, sizeof(double));
    row = 0;

    //Writes each linebuffer from file into the web array taking into account spaces
    while (fgets(line_buffer, BUFSIZE, web_file)) {
        for (column = 0; column < dimension; column++) {
            web[row + column] = atoi(line_buffer + (column * 2));
        }
        row += dimension;
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
       ELSE    reduce strlen by 1 in order to omit '\n' from each line 
       
       Accounts for every other character being a space and divides the dimension count accordingly*/
    if (strchr(line_buffer, '\r') != NULL) {
        dimension -= 2;
        dimension -= dimension/2;
        return dimension;
    }
    else {
        dimension--;
        dimension -= dimension/2;
        return dimension;
    }
}