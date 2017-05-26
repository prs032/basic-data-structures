//==========================================================================
// cs12xre                         Homework 7              Prashant Singh
//--------------------------------------------------------------------------
// File: Tree.java
//
// Description: Contains the TNode object and the member functions of
//              the Tree class.
//==========================================================================
public class Tree<UCSDStudent extends Base> extends Base {

	/* data fields */
	private TNode root;
	private long occupancy; 
	private String treeName;

	/* debug flag */
	private static boolean debug;

	/* debug messages */
	private static final String ALLOCATE = " - Allocating]\n";
	private static final String AND = " and ";
	private static final String COMPARE = " - Comparing ";
	private static final String INSERT = " - Inserting ";
	private static final String TREE = "[Tree ";


	/**
	 * This method allocates and initializes the memory
	 * associated with a Tree.
	 *
	 * @param  name   The name of the tree.
	 */
	public Tree (String name) {
		/* Debug Message*/
		if(debug) {
			System.err.println(TREE + name + ALLOCATE);
		}

		/* Initializing the tree */
		treeName = name;
		occupancy = 0;
		root = null;

	}

	/**
	 * This function sets the debug mode as off
	 */
	public static void debugOff () {

		debug = false;
	}

	/**
	 * This function sets the debug mode as on
	 */
	public static void debugOn () {

		debug = true;
	}

	/**
	 * Returns the tree's name
	 * @return name of the tree
	 */
	public String getName() {
		return treeName;
	}

	/**
	 * This method will insert the element in the Tree.
	 * If the element cannot be inserted, false will be returned.
	 * If the element can be inserted, the element is inserted
	 * and true is returned.
	 *
	 * @param   element       The element to insert.
	 * @return  true or false indicating success or failure of insertion
	 */
	public boolean insert (UCSDStudent element) {

		/* Insertion - Begins */
		TNode working = null;
		/* If root is null, insert as root Node */
		if(root == null) {
			/* Debug Message*/
			if(debug) {
				System.err.println(TREE + treeName + 
						INSERT + element.getName() + "]");
			}
			root = new TNode(element);
			root.parent = root;
			working = root;
		}/* else begins inserting somewhere down the tree */
		else {
			working = root;
			/* When child nodes are not null */
			while(working.right != null || working.left != null) {
				/* Debug Message*/
				if(debug) {
					System.err.println(TREE + treeName + 
							COMPARE + element.getName() + 
							AND + working.data.getName() + "]");
				}
				/* element greater than working - move right */
				if(element.isGreaterThan(working.data)) {              	
					if(working.right == null) {
						/* Debug Message*/
						if(debug) {
							System.err.println(TREE + treeName + 
									INSERT + element.getName() + "]");
						}
						working.right = new TNode(element);
						working.right.parent = working;
						return true;
					}
					else {
						/* setting height while traversing */
						working.height += 1;
						working = working.right;
					}
				}/* else traverse left */
				else {
					if(working.left == null) {
						/* Debug Message*/
						if(debug) {
							System.err.println(TREE + treeName + 
									INSERT + element.getName() + "]");
						}
						working.left = new TNode(element);
						working.left.parent = working;
						return true;
					}
					else
						/* setting height while traversing */
						working.height += 1;
					working = working.left;
				}
			}

			/* When child nodes are null */
			if(working.data.isGreaterThan(element)) {
				/* Debug Message*/
				if(debug) {
					System.err.println(TREE + treeName + 
							COMPARE + element.getName() + 
							AND + working.data.getName() + "]");
					System.err.println(TREE + treeName + 
							INSERT + element.getName() + "]");
				}
				/* setting height while inserting */
				working.height += 1;
				working.left = new TNode(element);
				working.left.parent = working;
			}
			else {
				/* Debug Message*/
				if(debug) {
					System.err.println(TREE + treeName + 
							COMPARE + element.getName() + 
							AND + working.data.getName() + "]");
					System.err.println(TREE + treeName + 
							INSERT + element.getName() + "]");
				}
				/* setting height while inserting */
				working.height += 1;
				working.right = new TNode(element);
				working.right.parent = working;
			}
		}
		/* Insertion - Ends */

		/* Calculating the balance while moving up the tree - Begins */
		while(working!=root) {
			/* setting balance of working node */
			working.balance = (working.left==null? -1:working.left.height)
					- (working.right==null ? -1:working.right.height);
			/* setting balance of working's parent node */
			working.parent.balance = (working.parent.left==null? 
					-1:working.parent.left.height)
					- (working.parent.right==null ? 
							-1:working.parent.right.height);
			/* Traversing up the tree */
			working = working.parent;
		}
		/* Calculating balance - Ends */

		return true;
	}

	/**
	 * This method will locate the location in the
	 * tree for the insert or lookup.
	 *
	 * @param   element  The element needing a location.
	 * @return  element if item found, or NULL if not found
	 */
	public UCSDStudent lookup (UCSDStudent element) {

		/* Look Up - Begins */
		TNode working = null;
		/* If element is root */
		if(root != null && root.data.equals(element)) {
			if(!root.hasBeenDeleted)
				return root.data;
			else
				return null;
		}/* else find element somewhere down the tree */
		else {
			working = root;
			/* When child nodes are not null */
			while(working.right != null || working.left != null) {
				/* element equal to working */
				/* Debug Message*/
				if(debug) {
					System.err.println(TREE + treeName + 
							COMPARE + element.getName() + 
							AND + working.data.getName() + "]");
				}
				if(working.data.equals(element)) {
					/* check working if its deleted */
					if(!working.hasBeenDeleted)
						return working.data;
					else
						return null; // not there
				}/* else traverse right */
				else if(element.isGreaterThan(working.data)) {
					if(working.right != null)
						working = working.right;
					else
						return null; // not there
				}/* else traverse left */
				else {
					if(working.left != null)
						working = working.left;
					else
						return null; // not there
				}
			}

			/* When child nodes are null */
			/* Debug Message*/
			if(debug) {
				System.err.println(TREE + treeName + 
						COMPARE + element.getName() + 
						AND + working.data.getName() + "]");
			}
			if(working.data.equals(element)) {
				/* check working if its deleted */
				if(!working.hasBeenDeleted)
					return working.data;
				else
					return null; // not there
			}
		}
		/* Look Up - Ends */

		return null;    // not there
	}

	/**
	 * This method will remove the element in the hash table.  If
	 * removed, the element is returned.  If the element
	 * is not found, NULL will be returned to the user.
	 *
	 * @param   element  The element to look up.
	 * @return  element if found, else NULL
	 */
	public UCSDStudent remove (UCSDStudent element) {

		/* Remove - Begins */
		TNode working = null;
		/* If element is root & is null */
		if(root == null) {
			return null;
		}/* If element is root & is not null */
		else if(root != null && root.data.equals(element)) {
			if(!root.hasBeenDeleted) {
				root.hasBeenDeleted = true;
				occupancy--;
				return root.data;
			}
			else
				return null;
		}/* else find element somewhere down the tree */
		else {
			working = root;
			/* When child nodes are not null */
			while(working.right != null || working.left != null) {
				/* element equal to working */
				/* Debug Message*/
				if(debug) {
					System.err.println(TREE + treeName + 
							COMPARE + element.getName() + 
							AND + working.data.getName() + "]");
				}
				if(working.data.equals(element)) {
					/* check working if its deleted */
					if(!working.hasBeenDeleted) {
						working.hasBeenDeleted = true;
						occupancy--;
						return working.data;
					}
					else
						return null; // not there
				}/* else traverse right */
				else if(element.isGreaterThan(working.data)) {
					if(working.right != null)
						working = working.right;
					else
						return null; // not there
				}/* else traverse left */
				else {
					if(working.left != null)
						working = working.left;
					else
						return null; // not there
				}
			}

			/* When child nodes are null */
			/* Debug Message*/
			if(debug) {
				System.err.println(TREE + treeName + 
						COMPARE + element.getName() + 
						AND + working.data.getName() + "]");
			}
			if(working.data.equals(element)) {
				/* check working if its deleted */
				if(!working.hasBeenDeleted) {
					working.hasBeenDeleted = true;
					occupancy--;
					return working.data;
				}
				else
					return null; // not there
			}
		}
		/* Remove - Ends */

		return null;    // not there
	}

	/**
	 * Creates a string representation of this tree. This method first
	 * adds the general information of this tree, then calls the
	 * recursive TNode function to add all nodes to the return string 
	 *
	 * @return  String representation of this tree 
	 */
	public String toString () {
		String string = "Tree " + treeName + ":\noccupancy is ";
		string += occupancy + " elements.";

		if(root != null)
			string += root.writeAllTNodes();

		return string;
	}

	private class TNode {

		public UCSDStudent data;
		public TNode left, right, parent;
		public boolean hasBeenDeleted;

		/* left child's height - right child's height */
		public long balance;
		/* 1 + height of tallest child, or 0 for leaf */
		public long height;

		/**
		 * This method allocates and initializes the memory
		 * associated with a TNode.
		 *
		 * @param  element   The data for the TNode.
		 */
		public TNode (UCSDStudent element) {

			/* Initializing TNode */
			data = element;
			left = null;
			right = null;
			parent = null;
			balance = 0;
			height = 0;
			hasBeenDeleted = false;
			occupancy++;
		}

		/**
		 * Creates a string representation of this node. Information
		 * to be printed includes this node's height, its balance,
		 * and the data its storing.
		 *
		 * @return  String representation of this node 
		 */

		public String toString () {
			return "at height:  " + height + "  with balance: " +
					balance + "  " + data;
		}

		/**
		 * Writes all TNodes to the String representation field. 
		 * This recursive method performs an in-order
		 * traversal of the entire tree to print all nodes in
		 * sorted order, as determined by the keys stored in each
		 * node. To print itself, the current node will append to
		 * tree's String field.
		 */
		public String writeAllTNodes () {
			String string = "";
			if (left != null)
				string += left.writeAllTNodes ();
			if (!hasBeenDeleted) 
				string += "\n" + this;          
			if (right != null)
				string += right.writeAllTNodes ();

			return string;
		}
	}
}
