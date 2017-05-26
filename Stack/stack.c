/****************************************************************************

                                                Prashant Singh
                                                04/19/2017

File Name:      stack.c
Description:    This program reads inputs from the user through driver class
                & creates a stack along and performs several other standard
                stack operations.

****************************************************************************/
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */
#define NULL 0

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = TRUE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}

void debug_on (void) {
        debug = TRUE;
}

/* start of true stack code */
/*---------------------------------------------------------------------------
Function Name:                delete_Stack
Description:                  This function deallocates the stack and frees up
                              the memory.
Input:                        Pointer: to the stack being allocated in main
                              function.
Output:                       None.
---------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
	// Checking for the non-existent stack
	if(spp == NULL) {
		fprintf (stderr, DELETE_NONEXIST);
	}
	else if(*spp == NULL) { 		// Checking if the parameter is null
		fprintf (stderr, INCOMING_NONEXIST);
	}
	else {
		free(*spp - STACK_OFFSET);  // Freeing up memory allocated in new_stack
		*spp = NULL;			    // Setting pointer to null

		/* Debugging */
		if(debug)
			fprintf(stdout, DEALLOCATE, *spp[STACK_COUNT_INDEX]);

		stack_counter--;
	}
}

/*---------------------------------------------------------------------------
Function Name:                empty_Stack
Description:                  This function empties the stack but do NOT frees
                              up the memory.
Input:                        Pointer: to the stack.
Output:                       None.
---------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {
	// Checking for the non-existent stack
	if(this_Stack == NULL) {
		fprintf (stderr, EMPTY_NONEXIST);
	}
	else {
		this_Stack[STACK_POINTER_INDEX] = 0; //Emptying the stack
	}
}

/*---------------------------------------------------------------------------
Function Name:                isempty_Stack
Description:                  This function checks if the stack is empty.
Input:                        Pointer: to the stack.
Output:                       1(True) if empty otherwise 0(False).
---------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {
	// Checking for the non-existent stack
	if(this_Stack == NULL) {
		fprintf (stderr, ISEMPTY_NONEXIST);
		return FALSE;
	}
	else if(this_Stack[STACK_POINTER_INDEX] == 0) {
		return TRUE;
	}
	else
		return FALSE;

}

/*---------------------------------------------------------------------------
Function Name:                isfull_Stack
Description:                  This function checks if the stack is full.
Input:                        Pointer: to the stack.
Output:                       Long: 1(True) if full otherwise 0(False).
---------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
	// Checking for the non-existent stack
	if(this_Stack == NULL) {
		fprintf (stderr, ISFULL_NONEXIST);
		return FALSE;
	}
	else if(this_Stack[STACK_SIZE_INDEX] == (this_Stack[STACK_POINTER_INDEX])) {
		return TRUE;
	}
	else
		return FALSE;
}

/*---------------------------------------------------------------------------
Function Name:                new_Stack
Description:                  This function allocates a new stack.
Input:                        Long: Stack size to be allocated.
Output:                       Pointer: to the new allocated stack.
---------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {
	//To hold array from malloc
	void *memory = malloc((stacksize + STACK_OFFSET) * sizeof(long));
    //To return to the caller
	Stack* this_stack = (Stack*) memory + STACK_OFFSET;

	/* Initialize */
	this_stack[STACK_COUNT_INDEX] = 1;
	this_stack[STACK_SIZE_INDEX] = stacksize;
	this_stack[STACK_POINTER_INDEX] = 0;

	/* Debugging */
	if(debug)
		fprintf(stdout, ALLOCATED, this_stack[STACK_COUNT_INDEX]);

	// Incrementing the stack counter
	stack_counter++;

    return this_stack;
}

/*---------------------------------------------------------------------------
Function Name:                get_occupancy
Description:                  This function returns the number of elements
							  in the stack.
Input:                        Pointer: to the stack.
Output:                       Long: Number of elements in the stack.
---------------------------------------------------------------------------*/
long get_occupancy (Stack * this_Stack) {
	// Checking for the non-existent stack
	if(this_Stack == NULL) {
		fprintf (stderr, NUM_NONEXIST);
		return FALSE;
	}
	else {
		// Just get the stack pointer index value to get the number of elements.
		long stack_size = this_Stack[STACK_POINTER_INDEX];
		return stack_size;
	}
}

/*---------------------------------------------------------------------------
Function Name:                pop
Description:                  This function pops or removes an element from
							  the stack.
Input:                        Pointer: to the stack; to item to be popped.
Output:                       Long: 1(True) for success otherwise 0(False).
---------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {
	// Checking for the non-existent stack
	if(this_Stack == NULL) {
		fprintf (stderr, POP_NONEXIST);
		return FALSE;
	}
	else if(isempty_Stack(this_Stack)) {// Checking for the empty stack
		fprintf (stderr, POP_EMPTY);
		return FALSE;
	}
	else {
		//Fetch the last pushed element of the stack to be popped.
		*item = this_Stack[this_Stack[STACK_POINTER_INDEX]-1];

		/* Debugging */
		if(debug) {
			fprintf(stdout, POP, this_Stack[STACK_COUNT_INDEX], *item);
			fprintf(stdout, HEXPOP, this_Stack[STACK_COUNT_INDEX], *item);
		}

		// Just decrement the stack pointer index for pop.
		// We do not free up memory here.
		this_Stack[STACK_POINTER_INDEX]--;
		return TRUE;
	}
}

/*---------------------------------------------------------------------------
Function Name:                push
Description:                  This function pushes an element on to
							  the stack.
Input:                        Pointer: to the stack; Long: item to push.
Output:                       Long: 1(True) for success otherwise 0(False).
---------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {
	// Checking for the non-existent stack
	if(this_Stack == NULL) {
		fprintf (stderr, PUSH_NONEXIST);
		return FALSE;
	}
	else if(isfull_Stack(this_Stack)) {// Checking for the empty stack
		fprintf (stderr, PUSH_FULL);
		return FALSE;
	}
	else {
		/* Debugging */
		if(debug) {
			fprintf(stdout, PUSH, this_Stack[STACK_COUNT_INDEX], item);
			fprintf(stdout, HEXPUSH, this_Stack[STACK_COUNT_INDEX], item);
		}

		// Fetching the index to push the item in the stack.
		long stack_pointer = this_Stack[STACK_POINTER_INDEX];
		this_Stack[stack_pointer] = item;
		// After push, incrementing the stack pointer index for next push.
		this_Stack[STACK_POINTER_INDEX]++;

		return TRUE;
	}
}

/*---------------------------------------------------------------------------
Function Name:                top
Description:                  This function returns the top element of
							  the stack.
Input:                        Pointer: to the stack; item to top.
Output:                       Long: The top item of stack otherwise 0(False).
---------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {
	// Checking for the non-existent stack
	if(isempty_Stack(this_Stack)) {
		fprintf (stderr, TOP_EMPTY);
		return FALSE;
	}
	else if(this_Stack == NULL) {// Checking for the empty stack
		fprintf (stderr, TOP_NONEXIST);
		return FALSE;
	}
	else {
		//Fetch the last pushed element of the stack to be topped.
		*item = this_Stack[this_Stack[STACK_POINTER_INDEX]-1];

		/* Debugging */
		if(debug) {
			fprintf(stdout, TOP, this_Stack[STACK_COUNT_INDEX], *item);
			fprintf(stdout, HEXTOP, this_Stack[STACK_COUNT_INDEX], *item);
		}

		return *item;
	}
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
FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }

        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));

        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
