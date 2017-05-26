//==========================================================================
// 	                         Homework 5              Prashant Singh
//--------------------------------------------------------------------------
// File: list.c
//
// Description: Contains the Node & List structure and the member functions of
//              the List class. The member function provides basic list data
//		structure functionalities.
//
// Debugger Questions: Using the debugger, insert three nodes into your list.
// 		       List the values of the following pointers after the
//		       three insertions:
//1. front:		   0x605890
//2. Node 1 data:	   0x605870
//3. Node 1 pre:	   0x605950
//4. Node 1 next:	   0x6058d0
//5. Node 2 data:	   0x6058b0
//6. Node 2 pre:	   0x605890
//7. Node 2 next:	   0x605910
//8. Node 3 data:	   0x6058f0
//9. Node 3 pre:	   0x6058d0
//10. Node 3 next:	   0x605950
//==========================================================================
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

typedef struct Node {
	struct Node * pre;      /* how to access the prior Node */
	struct Node * next;     /* how to access the next Node */
	void * data;            /* the data to store */
} Node;

typedef struct List {
	Node * front;             /* the front of the list */
	long list_count;        /* which list is it */
	long occupancy;
	void *(*copy_func) (void *);
	void (*delete_func) (void *);
	long (*is_greater_than_func) (void *, void *);
	FILE *(*write_func) (void *, FILE *);
} List;

/* private Node function declarations */
static void delete_Node (Node **, void (*delete_func) (void *));
static Node * insert_Node (Node *, void *, void *(*copy_func) (void *)); 
static int locate (List * this_list, void * element);
static Node * new_Node (void *, void *(*copy_func) (void *));
static void * remove_Node (Node *);
static void * view_Node (Node *); 
static FILE * write_Node (Node *, FILE *, FILE *(*write_func) (void *, FILE *));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] = 
		"Advance next from non-existent list!!!\n";
static const char ADNEXT_EMPTY[] = 
		"Advance next from empty list!!!\n";
static const char ADPRE_NONEXIST[] = 
		"Advance pre from non-existent list!!!\n";
static const char ADPRE_EMPTY[] = 
		"Advance pre from empty list!!!\n";
static const char DELETE_NONEXIST[] =
		"Deleting from non-existent list!!!\n";
static const char DELETE_NONEXISTNODE[] =
		"Deleting a non-existent node!!!\n";
static const char ISEMPTY_NONEXIST[] =
		"Is empty check from non-existent list!!!\n";
static const char INSERT_NONEXIST[] =
		"Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =
		"Removing from non-existent list!!!\n";
static const char REMOVE_EMPTY[] =
		"Remove from empty list!!!\n";
static const char VIEW_NONEXIST[] = 
		"Viewing a non-existent list!!!\n";
static const char VIEW_NONEXISTNODE[] = 
		"Viewing a non-existent node!!!\n";
static const char VIEW_EMPTY[] =
		"Viewing an empty list!!!\n";
static const char WRITE_NONEXISTFILE[] =
		"Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
		"Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
		"Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
		"Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";
static const char INSERT[] = "[List %ld - Inserting node]\n";
static const char REMOVE[] = "[List %ld - Removing node]\n";
static const char VIEW[] = "[List %ld - Viewing node]\n";
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n";

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */

/*--------------------------------------------------------------------------
Function Name:         set_debug_on.
Purpose:               This function sets the debug mode as on.
Description:           Sets the debug_on variable as true.
Input:                 void.
Result:                sets debug mode on with nothing to return.
--------------------------------------------------------------------------*/
void set_debug_on (void) {
	debug_on = TRUE;
}

/*--------------------------------------------------------------------------
Function Name:         set_debug_off.
Purpose:               This function sets the debug mode as off.
Description:           Sets the debug_on variable as false.
Input:                 void.
Result:                sets debug mode off with nothing to return.
--------------------------------------------------------------------------*/
void set_debug_off (void) {
	debug_on = FALSE;
}

/*--------------------------------------------------------------------------
Function Name:         advance_next_List.
Purpose:               This function causes the front pointer of this_list to
					   move forward by one Node.
Description:           The function moves the front of the list to the front
					   node's next..
Input:                 this_list - a pointer to the list we wish to shift.
Result:                The function shifts the front of the list to next node.
--------------------------------------------------------------------------*/
void advance_next_List (List * this_list) {
	/* debug message */
	if(debug_on)
		fprintf(stderr, ADNEXT, this_list->list_count);

	/* checking if list do not exist */
	if (!this_list) {
		fprintf (stderr, ADNEXT_NONEXIST);
	}

	/* checking if list is empty */
	if(isempty_List(this_list)) {
		fprintf(stderr, ADNEXT_EMPTY);
	}

	this_list->front = this_list->front->next;

}

/*--------------------------------------------------------------------------
Function Name:         advance_pre_List.
Purpose:               This function causes the front pointer of this_list to
					   move backward by one Node.
Description:           The function moves the front of the list to the front
					   node's pre.
Input:                 this_list - a pointer to the list we wish to shift.
Result:                The function shifts the front of the list to pre node.
--------------------------------------------------------------------------*/
void advance_pre_List (List * this_list) {
	/* debug message */
	if(debug_on)
		fprintf(stderr, ADPRE, this_list->list_count);

	/* checking if list do not exist */
	if (!this_list) {
		fprintf (stderr, ADPRE_NONEXIST);
	}

	/* checking if list is empty */
	if(isempty_List(this_list)) {
		fprintf(stderr, ADPRE_EMPTY);
	}

	this_list->front = this_list->front->pre;
}

/*--------------------------------------------------------------------------
Function Name:         delete_List.
Purpose:               This function deletes the entire allocated list along
					   freeing up the memory.
Description:           The function traverse through the list using front
					   pointer and starts deleting node along with freeing
					   memory. It does so by deleting the front's pre node
					   and shifting the front to front's next. When we
					   arrive at the last node, we do not shift the front
					   pointer and delete the node along with the list pointer
					   freeing up the entire memory.
Input:                 this_list - a pointer to the list we wish to shift.
Result:                The function shifts the front of the list to pre node.
--------------------------------------------------------------------------*/
void delete_List (List ** lpp) {
	/* Setting up the variables */
	List * this_list;
	this_list = *lpp;				/* setting the main list */

	long count;             		/* to know how many elements to print */
	Node ** working;                /* working node */

	/* checking if list do not exist */
	if (!this_list) {
		fprintf (stderr, DELETE_NONEXIST);
	}

	/* Deletion of list begins */
	if (this_list->occupancy >= 1){
		/* storing the occupancy for looping through */
		long occupancy = this_list->occupancy;

		for (count = 1; count <= occupancy; count++) {
			/* checking if node to delete is not the last one in the list */
			if(count!=occupancy) {

				/* making the front's pre node as working node */
				working = &this_list->front->pre;

				/* if list not empty then advance the front pointer */
				if(!isempty_List(this_list) && occupancy!=2)
					advance_next_List(this_list);

			}/* else if last node of the list to delete */
			else
			{
				/* setting the working as this last node */
				working = &this_list->front;
			}

			/* delete the working node */
			delete_Node(working, this_list->delete_func);

			/* reset the list's occupancy upon deletion of each node */
			this_list->occupancy--;
		}
	}

	/* debug message */
	if(debug_on)
		fprintf(stderr, LIST_DEALLOCATE, this_list->list_count);

	/* delete list */
	free (this_list);

	/* assign list pointer to NULL */
	this_list = NULL;
}

/*--------------------------------------------------------------------------
Function Name:         insert.
Purpose:               This function Inserts the element into this_list either
					   at the front, end or in sorted form.
Description:           The function checks if the list is empty or not and then
					   perform insertion as per the "where" value, either front,
					   end or some other location.
Input:                 this_list - a pointer to the list we wish to check.
					   element - a pointer to the object we wish to insert in
					   the list.
					   where - the place in the list where element should be
					   stored after the call to insert (1 for first item, 0
					   for last item and 2 for sorted insertion of the item).
Result:                Returns true if insertion is a success.
--------------------------------------------------------------------------*/
long insert (List * this_list, void * element, long where) {
	/* debug message */
	if(debug_on)
		fprintf(stderr, INSERT, this_list->list_count);

	/* checking if list do not exist */
	if (!this_list) {
		fprintf (stderr, INSERT_NONEXIST);
		return FALSE;
	}

	/* If the list is empty */
	if(isempty_List(this_list)) {
		this_list->front = insert_Node (this_list->front, element,
				this_list->copy_func);
		this_list->occupancy += 1;
		return TRUE;
	}/* If the list is NOT empty */
	else {
		/* saving the front for subsequent restoration */
		Node * front = this_list->front;

		/* If insertion is to be done in the end */
		if(where == END) {
			insert_Node (this_list->front, element,
					this_list->copy_func);
		}
		/* If insertion is to be done in the front */
		else if(where == FRONT){
			insert_Node (this_list->front, element,
					this_list->copy_func);
			this_list->front = this_list->front->pre;
		}
		/* If insertion is to be done in the sorted list */
		else {
			/* if the front node's data is greater than the element's data */
		   if(this_list->is_greater_than_func(this_list->front->data, element))
			{
				/* insert in front */
				insert_Node (this_list->front, element,
						this_list->copy_func);
				/* advance pre the front pointer */
				advance_pre_List(this_list);
			}
			else {
			/* locating the right sorted place for insertion of new node */
				locate(this_list, element);
				insert_Node (this_list->front, element,
						this_list->copy_func);
			/* restoring the list's front node after insertion of new node */
				this_list->front = front;
			}
		}

		/* Incrementing the occupancy of the list after successful insert */
		this_list->occupancy += 1;

		/* return true for successful insertion */
		return TRUE;
	}
	return FALSE;
}

/*--------------------------------------------------------------------------
Function Name:         locate.
Purpose:               This function is used to locate where the node should
					   be in a sorted list.
Description:           The function checks if the occupancy is zero or front
					   of the list is null.
Input:                 this_list - a pointer to the list we wish to check.
					   element - a pointer to the object we wish to insert
					   in the list
Result:                Result is expected to be true inserting at the front
					   of the list.
--------------------------------------------------------------------------*/
static int locate (List * this_list, void * element) {
	/* To locate the right place, advance the list till the front's data
	 * is not larger than the element */
	while(!this_list->is_greater_than_func(this_list->front->data, element)) {
		advance_next_List(this_list);
	}
	return TRUE;
}

/*--------------------------------------------------------------------------
Function Name:         isempty_List.
Purpose:               This function Checks to see if this_list is empty.
Description:           The function checks if the occupancy is zero or front
					   of the list is null.
Input:                 this_list - a pointer to the list we wish to check.
Result:                Returns true if list is empty otherwise false.
--------------------------------------------------------------------------*/
long isempty_List (List * this_list) {
	/* checking if list do not exist */
	if (!this_list) {
		fprintf (stderr, ISEMPTY_NONEXIST);
		return FALSE;
	}

	/* Checking the occupancy */
	if(this_list->occupancy==0 || this_list->front == NULL)
		return TRUE;
	else
		return FALSE;
}

/*--------------------------------------------------------------------------
Function Name:         new_List.
Purpose:               This function allocates and initializes the new list.
Description:           It initializes the list data fields, increment the list
					   counter and returns a pointer to the list. All new
					   lists should be empty.
Input:                 copy_func - a pointer to the function which makes copies
					   of the elements stored in this_list.
					   delete_func - a pointer to the function which frees the
					   memory associated with elements stored in this_list.
					   is_greater_than_func - a pointer to the function which
					   compares elements in this_list.
					   write_func - a pointer to the function which writes
					   elements in this_list.
Result:                The function returns a pointer to the new list allocated
					   and initialized.
--------------------------------------------------------------------------*/
List * new_List (
		void *(*copy_func) (void *),
		void (*delete_func) (void *),
		long (*is_greater_than_func) (void *, void *),
		FILE *(*write_func) (void *, FILE *)) {

	/* allocate */
	List * this_list = (List *) malloc(sizeof(List));

	/* initialize */
	this_list->front = NULL;
	this_list->occupancy = 0;
	this_list->list_count = list_counter + 1;
	this_list->copy_func = copy_func;
	this_list->delete_func = delete_func;
	this_list->is_greater_than_func = is_greater_than_func;
	this_list->write_func = write_func;

	/* increasing the list counter */
	list_counter++;

	/* debug message */
	if(debug_on)
		fprintf(stderr, LIST_ALLOCATE, list_counter);

	/* returning the new list */
	return this_list;
}

/*--------------------------------------------------------------------------
Function Name:         remove_List.
Purpose:               This function removes an element from the list at a
					   specified location.
Description:           The function checks if the occupancy is zero and if
					   if not, it removes node by arranging the surrounding
					   pointers and freeing the memory.
Input:                 this_list - a pointer to the list from which we wish
					   to remove.
					   where - the place in the number of the element in the
					   list we wish to remove insert (1 for first item, 0 for
					   last item).
Result:                Returns the pointer data of the node removed.
--------------------------------------------------------------------------*/
void * remove_List (List * this_list, long where) {
	/* checking if list is empty */
	if(isempty_List(this_list)) {
		fprintf(stderr, REMOVE_EMPTY);
		return NULL;
	}

	/* checking if list do not exist */
	if (!this_list) {
		fprintf (stderr, REMOVE_NONEXIST);
		return NULL;
	}

	/* debug message */
	if(debug_on)
		fprintf(stderr, REMOVE, this_list->list_count);

	/* removing node from either front or end */
	void * data = NULL;
	if(where == FRONT) {
		data = remove_Node(this_list->front);
		/* shifting the front of the list(if not empty) after removal */
		advance_next_List(this_list);
	}
	else {
		/* remove node till front's pre isnt equal to the front */
		if(this_list->front->pre != this_list->front)
			data = remove_Node(this_list->front->pre);
		else { /* removing the only remaning front node */
			data = this_list->front->data;
			this_list->front = NULL;
		}
	}
	/* Reducing occupancy by 1 after removal of node */
	this_list->occupancy--;
	/* returning the pointer to the data of the removed node */
	return data;
}

/*--------------------------------------------------------------------------
Function Name:         view.
Purpose:               This function is to view the data of a particular node
					   in the list on a specific location.
Description:           The function checks if the occupancy is zero or front
					   of the list is null.
Input:                 this_list - a pointer to the list we wish to view.
					   where - the location of the node to view. Could be
					   FRONT or END.
Result:                Returns a pointer to the object stored at location
					   where for viewing
--------------------------------------------------------------------------*/
void * view (List * this_list, long where) {
	/* debug message */
	if(debug_on)
		fprintf(stderr, VIEW, this_list->list_count);

	/* checking if list do not exist */
	if (!this_list) {
		fprintf (stderr, VIEW_NONEXIST);
	}

	/* checking if list is empty */
	if(isempty_List(this_list)) {
		fprintf(stderr, VIEW_EMPTY);
	}

	/* If the view is to be from front of the node */
	if(where == FRONT) {
		return view_Node(this_list->front);
	}
	/* If the view is to be from end of the node */
	else {
		return view_Node(this_list->front->pre);
	}
}

FILE * write_List (List * this_list, FILE * stream) {
	long count;             		/* to know how many elements to print */
	Node * working;                 /* working node */

	if (!stream) {
		fprintf (stderr, WRITE_NONEXISTFILE);
		return NULL;
	}

	if (!this_list) {
		fprintf (stderr, WRITE_NONEXISTLIST);
		return NULL;
	}

	if (stream == stderr)
		fprintf (stream, "List %ld has %ld items in it.\n",
				this_list->list_count, this_list->occupancy);

	if (!this_list->write_func) {
		fprintf (stream, WRITE_MISSINGFUNC);
		return stream;
	}

	if (this_list->occupancy >= 1)
		working = this_list->front;

	for (count = 1; count <= this_list->occupancy; count++) {
		if (stream == stderr)
			fprintf (stream, "\nelement %ld:  ", count);
		write_Node (working, stream, this_list->write_func);
		working = working->next;
	}

	return stream;
}

/*--------------------------------------------------------------------------
Function Name:         write_reverse_List.
Purpose:               This function Writes the elements of this_list backwards,
					   starting with the last item.
Description:           The function checks if the occupancy is not zero and
					   then access the last node of the list through pre
					   pointer of the front node and then traverse backwards
					   while printing the data till it finishes all the nodes.
Input:                 this_list - a pointer to the list we wish to write.
					   stream - output to be written in either stdout or stderr.
Result:                Returns the stream for either stdout for list to be
					   written or stderr for error encountered.
--------------------------------------------------------------------------*/
FILE * write_reverse_List (List * this_list, FILE * stream) {
	long count;             		/* to know how many elements to print */
	Node * working;                 /* working node */

	if (!stream) {
		fprintf (stderr, WRITE_NONEXISTFILE);
		return NULL;
	}

	if (!this_list) {
		fprintf (stderr, WRITE_NONEXISTLIST);
		return NULL;
	}

	if (stream == stderr)
		fprintf (stream, "List %ld has %ld items in it.\n",
				this_list->list_count, this_list->occupancy);

	if (!this_list->write_func) {
		fprintf (stream, WRITE_MISSINGFUNC);
		return stream;
	}

	if (this_list->occupancy >= 1)
		working = this_list->front->pre;

	for (count = 1; count <= this_list->occupancy; count++) {
		if (stream == stderr)
			fprintf (stream, "\nelement %ld:  ", count);
		write_Node (working, stream, this_list->write_func);
		/* Moving in reverse through working node's pre pointer */
		working = working->pre;
	}

	return stream;
}

/*--------------------------------------------------------------------------
Function Name:         delete_Node.
Purpose:               This function deletes the node of a list along with
					   freeing up the memory.
Description:           The function checks for the node existence and if it
					   does, calls the delete_func to delete the node thereby
					   freeing the memory as well setting the pointer to null.
Input:                 npp - a double pointer to the node we wish to delete.
					   delete_func - The function used to delete the element.
Result:                The function deletes & frees up the node along with its
					   element.
--------------------------------------------------------------------------*/
static void delete_Node (Node ** npp, void (*delete_func) (void *)) {

	/* does the node exist??? */
	if (!npp || !*npp) {
		fprintf (stderr, DELETE_NONEXISTNODE);
		return;
	}

	/* call function to delete element */
	if (delete_func && (*npp)->data)
		(*delete_func) (&((*npp)->data));

	/* delete element */
	free (*npp);

	/* assign node to NULL */
	*npp = NULL;
}

/*--------------------------------------------------------------------------
Function Name:         insert_Node.
Purpose:               To insert a Node after the parameter Node.
Description:           Creates a new node to hold element, or, if copy_func is
					   non-NULL, a copy of element. This new node is then
					   incorporated into the list at the location BEFORE this_Node.
Input:                 this_Node - a pointer to the Node that the new node is
					   inserted BEFORE.
					   element - a pointer to the object we wish to store in the
					   new node.
					   copy_func - the pointer to the copy function passed into
					   the list constructor or NULL if no copy needs to be made.
Result:                The function returns a pointer to the new node inserted
					   and initialized.
--------------------------------------------------------------------------*/
static Node * insert_Node (Node * this_Node, void * element, 
		void * (*copy_func) (void *)) {

	/* For case of the very first node in the list */
	if(!this_Node) {
		this_Node= new_Node(element, copy_func);

		/* Update the pointers for circular list */
		this_Node->pre = this_Node->next = this_Node;
		return this_Node;
	}
	else {
		Node * working = new_Node(element, copy_func);

		/* attaching the new node(working) */
		working->pre = this_Node->pre;
		working->next = this_Node;

		/* integrating the new node(working) with this_Node */
		this_Node->pre->next = working;
		this_Node->pre = working;
		return working;
	}
}

static Node* new_Node (void * element, void * (*copy_func) (void *)) {
	/* allocate memory */
	Node *this_Node = (Node *) malloc (sizeof (Node));

	/* initialize memory */
	this_Node->next = this_Node->pre = NULL;
	this_Node->data = (copy_func) ? (*copy_func) (element) : element;

	return this_Node;
}

/*--------------------------------------------------------------------------
Function Name:         remove_Node.
Purpose:               This function unlinks a node from the list and returns
					   the pointer to the data of the removed node.
Description:           The function arrange the pointers of the surrounding
					   node so they no longer point to this node. Along with
					   this we free the memory associated to this node.
Input:                 this_Node - a pointer to the Node to be removed.
Result:                Returns a pointer to removed node's data.
--------------------------------------------------------------------------*/
static void * remove_Node (Node * this_Node) {
	/* arranging the pointers of the surrounding Nodes */
	this_Node->pre->next = this_Node->next;
	this_Node->next->pre = this_Node->pre;

	void * data = this_Node->data;

	/* freeing the memory associated with this_Node */
	free(this_Node);
	this_Node = NULL;

	/* returning the pointer to the removed node's data */
	return data;
}

/*--------------------------------------------------------------------------
Function Name:         view_Node.
Purpose:               This function is to view the data of a given node.
Description:           The function access the data of the node and returns it.
Input:                 this_Node - a pointer to the Node whose data we wish to
					   view.
Result:                Returns a pointer to this_Node's data.
--------------------------------------------------------------------------*/
static void * view_Node (Node * this_Node) {
	/* checking if node do not exist */
	if (!this_Node) {
		fprintf (stderr, VIEW_NONEXISTNODE);
		return NULL;
	}

	/* return the node's data for viewing */
	return this_Node->data;
}

static FILE* write_Node (Node * this_Node, FILE * stream,
		FILE * (*write_func) (void *, FILE *)) {

	if (!stream) {
		fprintf (stderr, WRITE_NONEXISTFILE);
		return NULL;
	}

	if (!this_Node) {
		fprintf (stream, WRITE_NONEXISTNODE);
		return stream;
	}

	if (!write_func) {
		fprintf (stream, WRITE_MISSINGFUNC);
		return stream;
	}

	return (*write_func) (this_Node->data, stream);
}
