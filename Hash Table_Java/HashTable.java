public class HashTable extends Base {

	private static int counter = 0;	// number of HashTables so far
	private static boolean debug;// allocation of debug state

	// set in locate, last location checked in hash table 
	private int index = 0; 

	// set in insert/lookup, count of location in probe sequence
	private int count = 0;

	private int probeCount[];   // where we are in the probe sequence 
	private long occupancy;     // how many elements are in the Hash Table
	private int size;   // size of Hash Table
	private Base table[];       // the Hash Table itself ==> array of Base
	private int tableCount;     // which hash table it is

	// messages
	private static final String AND = " and ";
	private static final String DEBUG_ALLOCATE = " - Allocated]\n";
	private static final String DEBUG_LOCATE = " - Locate]\n";
	private static final String DEBUG_LOOKUP = " - Lookup]\n";
	private static final String BUMP = "[Bumping To Next Location...]\n";
	private static final String COMPARE = " - Comparing ";
	private static final String FOUND_SPOT = " - Found Empty Spot]\n";
	private static final String HASH = "[Hash Table ";
	private static final String HASH_VAL = "[Hash Value Is ";
	private static final String INSERT = " - Inserting ";
	private static final String PROCESSING = "[Processing ";
	private static final String TRYING = "[Trying Index ";


	/**
	 * This function sets the debug mode as on
	 */
	public static void debugOn () {
		debug = true;
	}

	/**
	 * This function sets the debug mode as off
	 */
	public static void debugOff () {
		debug = false;
	}

	/**
	 * This method allocates and initializes the memory
	 * associated with a hash table.
	 *
	 * @param  sz   The number of elements for the table...MUST BE PRIME!!!
	 */
	public HashTable (int sz) {
		// Initializing hash table parameters
		size = sz;
		table = new Base[size];
		probeCount = new int[size];
		occupancy = 0;
		counter = counter+1;
		tableCount = counter;

		//Initializing the table & probecount
		for(int i=0; i<size; i++) {
			table[i] = null;
			probeCount[i] = 0;
		}

		//debug messages
		if(debug)
			System.err.print(HASH + tableCount + DEBUG_ALLOCATE);
	}


	/**
	 * This method will insert the element in the hash table.
	 * If the element cannot be inserted, false will be returned.
	 * If the element can be inserted, the element is inserted
	 * and true is returned.  Duplicate insertions will
	 * cause the existing element to be deleted, and the duplicate
	 * element to take its place.
	 *
	 * @param   element       The element to insert.
	 * @param   initialCount  Where to start in probe seq (recursive calls) 
	 * @return  true or false indicating success or failure of insertion
	 */
	public boolean insert (Base element, int initialCount) {
		//debug messages
		if(debug) {
			System.err.println(HASH + tableCount + INSERT
					+ element.getName() + "]");
		}
		
		//Setting count to be initial count for all insertions
		count = initialCount;
		
		//Handling the full table case for no more insertions
		if(occupancy >= size) {
			return false;
		}

		//if valid location found, then insert
		if(locate(element)) {
			//Setting the probe count for probe sequence
			probeCount[index] = count;
			table[index] = element;
			//Set occupancy for successful insertion
			occupancy++;
		}//move on with subsequent hashing locate & insertion
		else {
			if(table[index] == null || element.equals(table[index])) {
				//Setting the probe count for probe sequence
				probeCount[index] = count;
				table[index] = element;
				occupancy++;
			}
			else {
				//Handling the bumped element case variables
				initialCount = initialCount + 1;
				//Saving the element to be bumped for recursive insertion
				Base bumpedElement = table[index];
				
				//Inserting the element before bumped element insertion
				if(table[index] != null) {
					table[index] = element;
					//Setting the probe count for probe sequence
					probeCount[index] = initialCount;
				}

				//debug messages
				if(debug)
					System.err.print(BUMP);
				
				//recursive insert for bumped element insertion
				insert(bumpedElement, initialCount);
			}
		}
		return true;
	}


	/**
	 * This method will locate the location in the
	 * table for the insert or lookup.
	 *
	 * @param   element  The element needing a location.
	 * @return  true if item found, or false if not found
	 */
	private boolean locate (Base element) {
		//debug messages
		if(debug) {
			System.err.print(HASH + tableCount + DEBUG_LOCATE);
			System.err.println(PROCESSING + element.getName() + "]");
		}

		int attribute, init_loc, increment;

		//Performing the initial hashing with index calculation
		attribute = element.hashCode();
		init_loc = attribute % size;
		increment = (attribute % (size-1)) + 1;

		//For the case when of first locate & insertion.
		if(count==1)
			index = init_loc;

		//debug message
		if(debug) {
			System.err.println(HASH_VAL + attribute + "]");
		}

		//Handling first insertion & first successful lookup.
		if((table[index] == null) || element.equals(table[index])) {
			//debug message
			if(debug) {
				System.err.println(TRYING + index + "]");
				if(table[index] == null)
					System.err.print(HASH + tableCount +  FOUND_SPOT);
			}
			return true;
		}//For handling the subsequent probe sequence insertions & lookups
		else {
			//When the element has the same hash code as the 
			//previously inserted ones.(locate & lookup)
			if(attribute == table[init_loc].hashCode()) {
				while(table[index]!=null) {
					count = count + 1;
					int tempIndex = index;
					//Performing subsequent hashing for next probe sequence
					index = (index + increment) % size;

					if(table[index]!=null) {
						//debug message
						if(debug) {
							System.err.println(TRYING + index + "]");
							System.err.println(HASH + tableCount +
									COMPARE + element.getName() + AND + 
									table[tempIndex].getName() + "]");
						}
						//When lookup is success, return true
						if(element.equals(table[index]))
							return true;
					}
					else {
						//debug message
						if(debug) {
							System.err.println(TRYING + index + "]");
							if(table[index]==null)
								System.err.print(HASH + tableCount
										+ FOUND_SPOT);
						}
					}
				}
			}//For distinct hash code element insertions
			else {
				int cursor=0;
				//Performing subsequent hashing for next 
				//probe sequence for look up
				index = (index + increment) % size;
				
				//Handling the case when only one element have been inserted
				if(count<=1){
					//Performing lookup using hashing
					while(cursor!=occupancy) {
						if(table[cursor]!=null) {
							//debug message
							if(debug) {
								System.err.println(TRYING + cursor + "]");
								System.err.println(HASH + tableCount +
										COMPARE + element.getName() + AND + 
										table[cursor].getName() + "]");
							}
							//When lookup is success, return true
							if(element.equals(table[cursor]))
								return true;
						}
						cursor++;
					}
					//incrementing the count for further hashed locate
					count++;
				}//For table with more than one elements, hashed lookup & locate
				else {
					cursor = index;
					//Searching the next hashed location for insertion
					while(cursor<size) {
						if(table[cursor]!=null) {
							//debug message
							if(debug) {
								System.err.println(TRYING + cursor + "]");
								System.err.println(HASH + tableCount + 
										COMPARE + element.getName() + AND +
										table[cursor].getName() + "]");
							}
						}
						else {
							//debug message
							if(debug) {
								System.err.println(TRYING + cursor + "]");
								System.err.print(HASH + tableCount + 
										FOUND_SPOT);
							}
							index = cursor;
							//Setting the probe count for probe sequence
							probeCount[index] = count + 1;
							return true;
						}
						//Performing locate using hashing
						cursor = (cursor + increment) % size;
						//incrementing the count for further hashed locate
						count++;
					}
				}
				//debug message
				if(debug) {
					if(table[index]==null)
						System.err.print(HASH + tableCount +  FOUND_SPOT);
				}
			}
			return false;
		}
	}


	/**
	 * This method will lookup the element in the hash table.  If
	 * found a pointer to the element is returned.  If the element
	 * is not found, NULL will be returned to the user.
	 *
	 * @param   element  The element to look up.
	 * @return  A pointer to the element if found, else NULL
	 */
	public Base lookup (Base element) {
		//debug message
		if(debug) {
			System.err.print(HASH + tableCount + DEBUG_LOOKUP);
		}

		//starting the lookup with first count
		count = 1;
		//Return null if table is empty
		if(occupancy < 1) {
			return null;
		}// Looking up elements in non-empty hash tables
		else {
			//if element located, return element
			if(locate(element))
				return table[index];
		}
		//if element not found, return null
		return null;
	}


	/**
	 * Creates a string representation of the hash table. The method 
	 * traverses the entire table, adding elements one by one ordered
	 * according to their index in the table. 
	 *
	 * @return  String representation of hash table
	 */
	public String toString () {
		String string = "Hash Table " + tableCount + ":\n";
		string += "size is " + size + " elements, "; 
		string += "occupancy is " + occupancy + " elements.\n";

		/* go through all table elements */
		for (int index = 0; index < size; index++) {

			if (table[index] != null) {
				string += "at index " + index + ": ";
				string += "" + table[index];
				string += " with probeCount: "; 
				string += probeCount[index] + "\n";
			}
		}

		return string;
	}
}
