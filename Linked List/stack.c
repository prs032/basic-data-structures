//==========================================================================
// 	                         Homework 5              Prashant Singh
//--------------------------------------------------------------------------
// File: stack.c
//
// Description: Contains the the member functions of the Stack class. The
//		member function provides basic stack data structure
//		functionalities implemented through list.
//==========================================================================
#include <stdio.h>
#include "list.h"
#include "stack.h"

/*---------------------------------------------------------------------------
Function Name:                delete_Stack
Description:                  This function deallocates the stack and frees up
                              the memory.
Input:                        Pointer to the pointer to the stack being
			      allocated in main function.
Output:                       void.
---------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
        delete_List (spp);
}

/*---------------------------------------------------------------------------
Function Name:                empty_Stack
Description:                  This function empties the stack but do NOT frees
                              up the memory.
Input:                        Pointer: to the this_stack.
Output:                       Returns (long) 1 for success & 0 for fail.
---------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_stack) {
        return isempty_List (this_stack);
}

/*---------------------------------------------------------------------------
Function Name:                new_Stack
Description:                  This function allocates a new stack.
Input:                        copy_func - a pointer to the function which
			      makes copies of the elements stored in this_list.
			      delete_func - a pointer to the function which
			      frees the memory associated with elements stored
			      in this_list.
			      is_greater_than_func - a pointer to the function
			      which compares elements in this_list.
			      write_func - a pointer to the function which
			      writes elements in this_list.
Output:                       Pointer: to the new allocated stack.
---------------------------------------------------------------------------*/
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

        return new_List (copy_func, delete_func, 
		is_greater_than_func, write_func);
}

/*---------------------------------------------------------------------------
Function Name:                pop
Description:                  This function pops or removes an element from
			      the stack.
Input:                        this_stack - Pointer to the stack;
Output:                       void pointer - to the item popped
---------------------------------------------------------------------------*/
void * pop (Stack * this_stack) {
        return remove_List (this_stack, END);
}

/*---------------------------------------------------------------------------
Function Name:                push
Description:                  This function pushes an element on to
			      the stack.
Input:                        this_stack - Pointer to the stack;
			      element: item to push.
Output:                       Long: 1(True) for success otherwise 0(False).
---------------------------------------------------------------------------*/
long push (Stack * this_stack, void * element) {
        return insert (this_stack, element, END);
}

/*---------------------------------------------------------------------------
Function Name:                top
Description:                  This function returns the top element of
			      the stack.
Input:                        Pointer: to the stack;
Output:                       void pointer: The top item of stack.
---------------------------------------------------------------------------*/
void * top (Stack * this_stack) {
        return view (this_stack, END);
}

/*--------------------------------------------------------------------------
Function Name:         write_Stack.
Purpose:               This function Writes the elements of this_stack,
		       starting with the first item.
Input:                 this_Stack - a pointer to the stack we wish to write.
		       stream - output to be written in either stdout or stderr.
Result:                Returns the stream for either stdout for list to be
		       written or stderr for error encountered.
--------------------------------------------------------------------------*/
FILE * write_Stack (Stack * this_stack, FILE * stream) {
        return write_List (this_stack, stream);
}
