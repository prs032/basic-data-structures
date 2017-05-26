/****************************************************************************
                                                Prashant Singh
                                                04/19/2017


File Name:      driver.c
Description:    This is main program & reads inputs from the user to creates
		a stack and performs several other standard stack operations.
		It also performs several error checks for boundary conditions
		to make sure Stack operations perform well.

Questions:
Answer 1:		The address returned by the malloc() in new_stack is -
				0x604830
Answer 2:		The address returned by new_stack is - 0x604848
Answer 3:		The allocated memory which pertains to this returned address
 	 	 	 	is on heap. However, the actual address returned is stored
 	 	 	 	on stack.
Answer 4:		The value of spp is - 0x7fffffffdbf0. This value is stored
				on stack.
Answer 5:		The value of *spp is 0x604848. This address is stored on
				stack.
Answer 6:		The parameter being passed to free is - 0x604830.
Answer 7:		Yes, the parameter being passed to free is SAME that was
				returned from malloc.

****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "stack.h"

int main (int argc, char * const * argv) {

        Stack * main_Stack = 0;         /* the test stack */
        unsigned long amount;        /* numbers of items possible go on stack */
        long command;                   /* stack command entered by user */
        long item = 0;                  /* item to go on stack */
        char option;                    /* the command line option */
        long status;                    /* return status of stack functions */
        
        /* initialize debug states */
        debug_off ();

        /* check command line options for debug display */
        while ((option = getopt (argc, argv, "x")) != EOF) {
                
                switch (option) {
                        case 'x': debug_on (); break;
                }
        }

        while (1) {
                command = 0;            /* initialize command, need for loops */
                writeline ("\nPlease enter a command:", stdout);
                writeline ("\n\t(a)llocate, (d)eallocate, ", stdout);
                writeline ("p(u)sh, (p)op, (t)op, (i)sempty, (e)mpty, ",stdout);
                writeline ("\n\tis(f)ull, (n)um_elements, (w)rite to stdout, "
                                                                , stdout);
                writeline ("(W)rite to stderr.\n", stdout);
                writeline ("Please enter choice:  ", stdout);
                command = getchar ();
                if (command == EOF)     /* are we done? */
                        break;
                clrbuf (command);       /* get rid of extra characters */

                switch (command) {      /* process commands */

                /* YOUR CODE GOES HERE */
                case 'a':               /* allocate */
                	    writeline ("\nPlease enter the number of objects to be able to store:  ", stdout);
                	    amount = decin ();
                	    clrbuf (0);     /* get rid of extra characters */
                	    main_Stack = new_Stack (amount);
                	    if (main_Stack != NULL && isempty_Stack(main_Stack))
                	    	writeline("\nAllocation SUCCESS", stdout);
                	    else
                	    	fprintf(stderr,"\nWARNING: Allocation FAILED \n");
                        break;

                case 'd':               /* deallocate */
                        delete_Stack(&main_Stack);
                        writeline("\nDeallocation SUCCESS", stdout);
                        break;

                case 'e':               /* empty */
                        empty_Stack(main_Stack);
                        writeline("\nEmptying the stack SUCCESS", stdout);
                        break;

                case 'f':               /* isfull */
                        if (isfull_Stack (main_Stack))
                                writeline ("Stack is full.\n",stdout);
                        else
                                writeline ("Stack is not full.\n", stdout);
                        break;

                case 'i':               /* isempty */
                        if (isempty_Stack (main_Stack))
                                writeline ("Stack is empty.\n", stdout);
                        else
                                writeline ("Stack is not empty.\n", stdout);
                        break;

                case 'n':               /* get_occupancy */
                        writeline ("Number of elements on the stack is:  ",
                                                                    stdout);
                        decout (get_occupancy (main_Stack));
                        newline (stdout);
                        break;

                case 'p':               /* pop */
                        status = pop (main_Stack, &item);
                        if (! status)
                                fprintf (stderr,"\nWARNING:  pop FAILED\n");
                        else {
                                writeline (
                                        "Number popped from the stack is:  ",
                                                                    stdout);
                                decout (item);
                                newline (stdout);
                        }
                        break;

                case 't':               /* top */
                        status = top (main_Stack, &item);
                        if (! status)
                                fprintf (stderr,"\nWARNING:  top FAILED\n");
                        else {
                                writeline (
                                        "Number at top of the stack is:  ",
                                                                    stdout);
                                decout (item);
                                newline (stdout);
                        }
                        break;

                case 'u':               /* push */
                        writeline (
                                "\nPlease enter a number to push to stack:  ",
                                                                    stdout);
                        item = decin ();
                        clrbuf (0);     /* get rid of extra characters */
                        status = push (main_Stack, item);
                        if (! status)
                                fprintf(stderr,"\nWARNING:  push FAILED\n");
                        break;

                case 'w':               /* write */
                        writeline ("\nThe Stack contains:\n", stdout);
                        write_Stack (main_Stack, stdout);
                        break;

                case 'W':               /* write */
                        writeline ("\nThe Stack contains:\n", stdout);
                        write_Stack (main_Stack, stderr);
                        break;
                }

                if (item == EOF) /* check if done */
                        break;
        }

        if (main_Stack)
                delete_Stack (&main_Stack);     /* deallocate stack */
        newline (stdout);
        return 0;
}
