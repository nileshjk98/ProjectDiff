Diff command implementation using Myer's Diff Algorithm and Patch command

Name : Nilesh Kshirsagar
MIS : 111603032

In this project, I implemented diff command using Myer's diff algorithm. The
code works for files with number of lines upto 2048. The number of lines can
be increased at the cost of space required for the program. The output produced
using normal diff(without any options) can be used to directly to patch the
file1 using linux patch command. I have also impemented patch command which can
used for the same purpose. For binary files, it states that files differ if the 
two input files are different, otherwise there is no output.
The options for which the code works are:
-r : recursive directory comparison
-c : context format output
-y : side by side output format
-t : expand tabs to correct number of spaces
-b : ignores space change i.e. trailing spaces at the end or converts a sequence of 
 	spaces into a single space
-w : ignores all white space characters
-i : ignores case

The above options can be combined also(except -y and -c).

