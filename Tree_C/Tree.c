//==========================================================================
// cs12xre                         Homework 7              Prashant Singh
//--------------------------------------------------------------------------
// File: Tree.c
//
// Description: Contains the TNode structure and the member functions of
//              the Tree class.
//==========================================================================
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "Tree.h"
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";

struct TNode {
	Base * data;
	TNode * left, * right, *parent;
	static long occupancy;
	unsigned long hasBeenDeleted;

	// left child's height - right child's height
	long balance;

	// 1 + height of tallest child, or 0 for leaf
	long height;

	/**
	 * This method allocates and initializes the memory
	 * associated with a TNode.
	 *
	 * @param  element   The element for data of the TNode.
	 */
	TNode (Base * element) : data (element), left (0), right (0),
			parent (0), hasBeenDeleted (FALSE), balance (0), height(0) {

		/* Initializing TNode */
		data = element;
		left = NULL;
		right = NULL;
		parent = NULL;
		balance = 0;
		height = 0;
		hasBeenDeleted = FALSE;
		occupancy++;

	}

	/**
	 * This method deallocates the memory
	 * associated with a TNode.
	 *
	 * @param  void
	 */
	~TNode (void) {
		// Deallocating the memory
		delete(data);
		delete(left);
		delete(right);
		delete(parent);
	}

	ostream & Write (ostream & stream) const {
		stream << "at height:  " << height << "  with balance:  "
				<< balance << "  ";
		return data->Write (stream) << "\n";
	}
	ostream & Write_AllTNodes (ostream & stream) const {
		if (left)
			left->Write_AllTNodes (stream);
		if (!hasBeenDeleted)
			Write (stream);
		if (right)
			right->Write_AllTNodes (stream);

		return stream;
	}

	/**
	 * This method deallocates the memory
	 * associated with all TNodes in the Tree
	 * recursively.
	 *
	 * @param  void.
	 */
	void delete_AllTNodes (void) {
		// Deleting the nodes recursively
		if (left)
			left->delete_AllTNodes ();
		if (right)
			right->delete_AllTNodes ();

		if(this != NULL)
			delete (this);
	}
};

// initialization of static data fields of Tree
long TNode :: occupancy = 0;
bool Tree :: debug_on = 0;

/***************************************************************************
% Routine Name : Tree :: Set_Debug  (public)
% File :         Tree.c
%
% Description :  Sets the debug option as on or off.
***************************************************************************/
void Tree :: Set_Debug (bool option) {

	debug_on = option;
}

Tree :: Tree (const char * name) : root (0), tree_name (strdup (name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
% 
% Description :  Initializes Tree, root pointer to NULL, and occupancy to 0.
***************************************************************************/

{
	/* Debug Message*/
	if(debug_on) {
		cerr << TREE << name << ALLOCATE << endl;
	}

	/* Initializing the tree */
	tree_name = name;
	root = NULL;

}

Tree :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
	// freeing the memory
	free( (void *)tree_name);
}	/* end: ~Tree */

/***************************************************************************
% Routine Name : Tree :: Insert  (public)
% File :         Tree.c
%
% Description :  Inserts the element into the binary tree. Returns true or
%				 false indicating success of insertion.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            A reference to the element to insert in the tree.
% <return>           1(TRUE) if success or 0(FALSE) if failed to insert.
***************************************************************************/
unsigned long Tree :: Insert (Base * element) {

	/* Insertion - Begins */
	TNode* working = NULL;
	/* If root is NULL, insert as root Node */
	if(root == NULL) {
		/* Debug Message*/
		if(debug_on) {
			cerr<<TREE << tree_name <<
					INSERT << (const char *) * element << "]" << endl;
		}
		root = new TNode(element);
		root->parent = root;
		working = root;
	}/* else begins inserting somewhere down the tree */
	else {
		working = root;
		/* When child nodes are not NULL */
		while(working->right != NULL || working->left != NULL) {
			/* Debug Message*/
			if(debug_on) {
				cerr<<TREE << tree_name <<
						COMPARE << (const char *) * element <<
						AND << (const char *) * (working->data)<< "]" << endl;
			}
			/* element greater than working - move right */
			if(*element > *(working->data)) {
				if(working->right == NULL) {
					/* Debug Message*/
					if(debug_on) {
						cerr<<TREE << tree_name <<
								INSERT << (const char *) * element << "]" << endl;
					}
					working->right = new TNode(element);
					working->right->parent = working;
					return TRUE;
				}
				else {
					/* setting height while traversing */
					working->height += 1;
					working = working->right;
				}
			}/* else traverse left */
			else {
				if(working->left == NULL) {
					/* Debug Message*/
					if(debug_on) {
						cerr<<TREE << tree_name <<
								INSERT << (const char *) * element << "]" << endl;
					}
					working->left = new TNode(element);
					working->left->parent = working;
					return TRUE;
				}
				else
					/* setting height while traversing */
					working->height += 1;
				working = working->left;
			}
		}

		/* When child nodes are NULL */
		if(*(working->data) > *element) {
			/* Debug Message*/
			if(debug_on) {
				cerr<<TREE << tree_name <<
						COMPARE << (const char *) * element <<
						AND <<(const char *) * (working->data) << "]" << endl;
				cerr<<TREE << tree_name <<
						INSERT << (const char *) * element << "]" << endl;
			}
			/* setting height while inserting */
			working->height += 1;
			working->left = new TNode(element);
			working->left->parent = working;
		}
		else {
			/* Debug Message*/
			if(debug_on) {
				cerr<<TREE << tree_name <<
						COMPARE << (const char *) * element <<
						AND << (const char *) * (working->data) << "]" << endl;
				cerr<<TREE << tree_name <<
						INSERT << (const char *) * element << "]" << endl;
			}
			/* setting height while inserting */
			working->height += 1;
			working->right = new TNode(element);
			working->right->parent = working;
		}
	}
	/* Insertion - Ends */

	/* Calculating the balance while moving up the tree - Begins */
	while(working!=root) {
		/* setting balance of working node */
		working->balance = (working->left==NULL? -1:working->left->height)
	    							- (working->right==NULL ? -1:working->right->height);
		/* setting balance of working's parent node */
		working->parent->balance = (working->parent->left==NULL?
				-1:working->parent->left->height)
	    							- (working->parent->right==NULL ?
	    									-1:working->parent->right->height);
		/* Traversing up the tree */
		working = working->parent;
	}
	/* Calculating balance - Ends */

	return TRUE;
}

/***************************************************************************
% Routine Name : Tree :: Lookup  (public)
% File :         Tree.c
%
% Description :  Looks up the matching data in the binary tree. Returns a
%				 pointer to the data if found, null otherwise.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            A reference to the element to lookup.
% <return>           A reference to the data found or null if not.
***************************************************************************/
const Base * Tree :: Lookup (const Base * element) const {

	/* Look Up - Begins */
	TNode* working = NULL;
	/* If element is root */
	if(root != NULL && root->data == element) {
		if(!root->hasBeenDeleted)
			return root->data;
		else
			return NULL;
	}/* else find element somewhere down the tree */
	else {
		working = root;
		/* When child nodes are not NULL */
		while(working->right != NULL || working->left != NULL) {
			/* element equal to working */
			/* Debug Message*/
			if(debug_on) {
				cerr << TREE << tree_name <<
						COMPARE << (const char *) * element <<
						AND << (const char *) * (working->data) << "]"<< endl;
			}
			if(*(working->data) == *element) {
				/* check working if its deleted */
				if(!working->hasBeenDeleted)
					return working->data;
				else
					return NULL; // not there
			}/* else traverse right */
			else if(*element > *(working->data)) {
				if(working->right != NULL)
					working = working->right;
				else
					return NULL; // not there
			}/* else traverse left */
			else {
				if(working->left != NULL)
					working = working->left;
				else
					return NULL; // not there
			}
		}

		/* When child nodes are null */
		/* Debug Message*/
		if(debug_on) {
			cerr << TREE << tree_name <<
					COMPARE << (const char *) * element <<
					AND << (const char *) * (working->data) << "]"<< endl;
		}
		if(*(working->data) == *element) {
			/* check working if its deleted */
			if(!working->hasBeenDeleted)
				return working->data;
			else
				return NULL; // not there
		}
	}
	/* Look Up - Ends */
	return NULL;
}

/***************************************************************************
% Routine Name : Tree :: Remove  (public)
% File :         Tree.c
%
% Description :  Removes the matching data from the binary tree. Returns a
%				 pointer to the data if found, null otherwise.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            A reference to the element to remove in the tree.
% <return>           A reference to the element removed or null if failed
%					 to removed.
***************************************************************************/
Base * Tree :: Remove (const Base * element) {
	/* Remove - Begins */
	TNode* working = NULL;
	/* If element is root & is NULL */
	if(root == NULL) {
		return NULL;
	}/* If element is root & is not NULL */
	else if(root != NULL && root->data == element) {
		if(!root->hasBeenDeleted) {
			root->hasBeenDeleted = TRUE;
			return root->data;
		}
		else
			return NULL;
	}/* else find element somewhere down the tree */
	else {
		working = root;
		/* When child nodes are not NULL */
		while(working->right != NULL || working->left != NULL) {
			/* element equal to working */
			/* Debug Message*/
			if(debug_on) {
				cerr << TREE << tree_name <<
						COMPARE << (const char *) * element <<
						AND << (const char *) * (working->data) << "]"<< endl;
			}
			if(*(working->data) == *element) {
				/* check working if its deleted */
				if(!working->hasBeenDeleted) {
					working->hasBeenDeleted = TRUE;
					return working->data;
				}
				else
					return NULL; // not there
			}/* else traverse right */
			else if(*element > *(working->data)) {
				if(working->right != NULL)
					working = working->right;
				else
					return NULL; // not there
			}/* else traverse left */
			else {
				if(working->left != NULL)
					working = working->left;
				else
					return NULL; // not there
			}
		}

		/* When child nodes are NULL */
		/* Debug Message*/
		if(debug_on) {
			cerr << TREE << tree_name <<
					COMPARE << (const char *) * element <<
					AND << (const char *) * (working->data) << "]"<< endl;
		}
		if(*(working->data) == *element) {
			/* check working if its deleted */
			if(!working->hasBeenDeleted) {
				working->hasBeenDeleted = TRUE;
				return working->data;
			}
			else
				return NULL; // not there
		}
	}
	/* Remove - Ends */

	return NULL;
}

ostream & Tree :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
        stream << "Tree " << tree_name << ":\n"
		<< "occupancy is " << TNode :: occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}       /* end: Write */

