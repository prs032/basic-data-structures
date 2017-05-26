import java.io.EOFException;

/**
 * <YOU FILL IN THE CLASS HEADER>
 */ 
class UCSDStudent extends Base {
        private String name;
        private long Studentnum;

        public String toString () {

                return "name: " + name + " Studentnum: " + Studentnum;
        }

        // UCSDStudent parameterized  constructors
        public UCSDStudent (String nm, long number) {
        	name = new String (nm);
        	Studentnum = number;
        }
        
        public UCSDStudent (UCSDStudent student) {
            name = new String (student.name);
            Studentnum = student.Studentnum;
        }

        //getter/setters methods
		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}

		public long getStudentnum() {
			return Studentnum;
		}

		public void setStudentnum(long studentnum) {
			Studentnum = studentnum;
		}
		
		//UCSDStudent equals method definition
		public boolean equals (Object other) {
            if (this == other) 
                    return true;
            
            if (!(other instanceof UCSDStudent)) 
                    return false;
            
            UCSDStudent otherVar = (UCSDStudent)other;
            
            return name.equals (otherVar.getName ());
		}
		
		//UCSDStudent object's hashCode method
		public int hashCode () {
            int retval = 0;
            int index = 0;

            while (index != name.length ()) {
                    retval += name.charAt (index);
                    index ++;
            }

            return retval;
		}
		
		public UCSDStudent assign (long val) {
            
			UCSDStudent retval;        // return value

            // give variable its value
            Studentnum = val;
            retval = new UCSDStudent (this);

            return retval;
		}
}

public class Driver {

        public static void main (String [] args) {

                /* initialize debug states */
                HashTable.debugOn();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                HashTable.debugOn();
                }

                /* The real start of the code */
                SymTab symtab = new SymTab (5);
                String buffer = null;
                char command;
                long number = 0;

                System.out.print ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = 0;    // reset command each time in loop
                        System.out.print ("Please enter a command:  "
                                + "((i)nsert, (l)ookup, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'i':
                                System.out.print (
                                "Please enter UCSD Student name to insert:  ");
                                buffer = MyLib.getline ();// formatted input

                                System.out.print (
                                        "Please enter UCSD Student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create Student and place in symbol table
                                if(!symtab.insert (
                                        new UCSDStudent (buffer, number), 1)){

                                        System.out.println("Couldn't insert " + 
                                                           "student!!!"); 
                                }
                                break;

                        case 'l': {
                                Base found;     // whether found or not

                                System.out.print (
                                "Please enter UCSD Student name to lookup:  ");

                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println ("Student found!!!");
                                        System.out.println (found);
                                }
                                else
                                        System.out.println ("Student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.print (
                                    "The Symbol Table contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }

                System.out.print ("\nFinal Symbol Table:\n" + symtab);
        }
}
