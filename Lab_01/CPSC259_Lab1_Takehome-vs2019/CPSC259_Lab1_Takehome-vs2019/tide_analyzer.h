/*
File:			tide_analyzer.h
Purpose:		Contains prototypes
Author:			Willem Van Dam and Liam Foster
Student #s:		33500646 and 40199382
CS Accounts:	g2q2b and h9h2b
Date:			09-21-2021
*/

/********************************************************************
YOU MAY EDIT THIS FILE (BUT YOU DON'T NEED TO)
******************************************************************/

#pragma once

/* Insert function prototypes here */
FILE* open_file(FILE* file_pointer, char* fileName, char* mode);
void process_file(double array_to_populate[], FILE* pointer_to_data_file);
void run_analysis();