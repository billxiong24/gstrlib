						GString Library

What is it?
-----------

The GString Library provides an easier way to create and manipulate 
strings in C, similar to Java's StringBuilder class. 
It does all the necessary memory allocations and provides a function 
for deallocating memory on the heap. The library includes standard string 
functions as well as other useful functions not found in the C 
string header file (i.e. remove_char, set_char, replace, trim, reverse, 
append, etc.). The GString Library also allows access to contents of strings with 
negative indices (similar to Python). It supports a robost error-handling system as 
well. 

-------------------------------------------------------------------------------------

How to use
----------

1. Include the header file (gstrlib.h) at the top of the source file.
2. Create the object file (.o) from the source file by typing in the command,
   replacing "sourcefile" with the name of your program. The object file will
   have the same name, but with a ".o" extension:

						cc -c sourcefile.c 

3. Similarly, create the object files for gstrlib.c and aux.c:

						cc -c gstrlib.c
						cc -c aux.c

4. Link the two object files together to create the library called libgstr.a:

						ar rc libgstr.a gstrlib.o aux.o

5. Create the executable file by linking the library with the object file 
   of the source file created in step 1. This will create an executable called
   "sourcefile" (can be any name):

   						cc -o sourcefile sourcefile.o libgstr.a 

6. You should now have an executable file in your directory. Run the program:

						./sourcefile


Alternatively, you can also compile the source files directly to create an executable:
			
						gcc -o sourcefile sourcefile.c aux.c gstrlib.c

This will create an executable called "sourcefile" which you can the run.

-------------------------------------------------------------------------------------








