/*
 File:          websolver.h
 Purpose:       Contains constants, prototypes, globals
 Author:			  Your names
 Student #s:		12345678 and 12345678
 CWLs:      		cwl1 and cwl2
 Date:				  Add the date here
 */


#pragma once

  /* Preprocessor directives to define macros */
#define WEB1     "web.txt"
#define BUFFER    128

#include <stdio.h>
#include <stdlib.h>

                /* Function prototypes */
int          get_web_dimension(FILE* web_file);
double* parse_web(FILE* web_file, int dimension);