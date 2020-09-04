/*************************************
* Lab 1 Exercise 2
* Name: Wong Choon Wei
* Student No: A0202190X
* Lab Group: 11
*************************************/

#include "functions.h"

// write the necessary code to initialize the func_list
// array here, if needed

int (*func_list[5])(int);

void update_functions() 
{
    func_list[0] = add_one;
    func_list[1] = add_two;
    func_list[2] = multiply_five;
    func_list[3] = square;
    func_list[4] = cube;
    
}
