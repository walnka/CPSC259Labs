/*
 File:         main.c
 Purpose:      Contains the main function for driving the program.
               You may add your own testing code here in the main
               function, but this file will not be submitted for
               credit.
 Author:       Your names
 Student #s:   12345678 and 12345678
 CWLs:         cwl1 and cwl2
 Date:         Add the date here
 */

 /* Preprocessor directives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

/*
 Main function drives the program.  Every C program must have one
 main function.  A project will not compile without one.

 In our program, the main function does nothing.  That's because
 our program doesn't do anything.  We're just implementing a linked
 list and testing it using our unit tests.

 You may add your own code for testing if you wish.

 PRE:    NULL (no pre-conditions)
 POST:	 NULL (no side-effects)
 RETURN: IF the program exits correctly
     THEN 0 ELSE 1
 */
int main(void)
{
  /* The system command forces the system to pause */
  airplane test_airplane = { 100, "Vancouver", "Toronto", 1, 900, 36000, 350 };
  airplane test_airplane2 = { 102, "Vancouver", "Toronto", 1, 900, 36000, 350 };
  node* node1 = create_node(test_airplane);
  node* node2 = create_node(test_airplane2);
  node1->next = node2;
  //node1 = NULL;
  print_list(node1);
  system("pause");
  return 0;
}