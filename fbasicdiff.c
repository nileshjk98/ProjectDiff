#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include "diff.h"

/*
 * In main function, arguments are separated and
 * list of options are separated from filenames or directory names
 * Other arguments are identified i.e it is checked if we 
 * intend to find diff of directories or files and these arguments are passed
 * to the corresponding functions for next operations
 * Error checking i.e whether filenames or directory names are valid or not is also done 
 * in main
 * 
 * normal diff output gives difference in terms of deletions and insertions
 * I have not implemented 'replace' i.e. change which is basically same as deleting first 
 * and then appending at that location in first file
 * The output produced by this program can be used directly to patch two files
 * using linux patch command as it basically produces output in same format as existing diff
 * Hence I have not implemented patch command separately
 *
 * Options implemented currently include 
 * -r : recursive directory comparison
 * -c : context format output 
 * -y : side by side output format
 * -t : expand tabs to correct number of spaces
 * -b : ignores space change i.e. trailing spaces at the end or converts a sequence of 
 *  	spaces into a single space
 * -w : ignores all white space characters 	
 * -i : ignores case
 *
 *  The above options can be combined also
 *
 */

int main(int argc, char * const argv[]) {
	int flag = 0, flags = 0, d;
	file1 f1;
	file2 f2;
	dir1 d1;
	dir2 d2;
	const char *optlist;
	char c, ch1, ch2;
	optlist = "rcytiwb";
	
	if(argc < 3) {
		printf("mydiff: missing operand\n");
		exit(-1);
	}

	ignore_space_change = false;
	recursive_dir_diff = false;
	sidebyside = false;
	normal_diff = true;
	expand_tabs = false;
	ignore_case = false;
	ignore_all_space = false;
	context_format = false;

	while((c = getopt(argc, argv, optlist)) != -1) {
		switch(c) {
			case 'r' : recursive_dir_diff = true;
					   break;
			case 'y' : if(context_format == false)
						   sidebyside = true;
					   else { 
						   printf("mydiff: conflicting output style options\n");
						   exit(-1);
					   }
					   break;
			case 'c' : if(sidebyside == false)
						   context_format = true;
					   else { 
						   printf("mydiff: conflicting output style options\n");
						   exit(-1);
					   }
					   break;
			case 't' : expand_tabs = true;
					   break;
			case 'i' : ignore_case = true;
					   break;
			case 'w' : ignore_all_space = true;
					   break;
			case 'b' : ignore_space_change = true;
					   break;
			case '?' : break;
		}
	}

	d1.dirname = argv[argc - 2];
	d1.dd = opendir(d1.dirname);
	if(d1.dd == NULL) {
		d2.dirname = argv[argc - 1];
		d2.dd = opendir(d2.dirname);
		if(d2.dd != NULL) {
			printf("mydiff: %s: Not a file", d2.dirname);
			exit(-1);
		}
		f1.filename = argv[argc - 2];
		f2.filename = argv[argc - 1];
		f1.fd = open(f1.filename, O_RDONLY);
		if(f1.fd == -1) {
			printf("mydiff: %s: No such file or directory\n", f1.filename);
			return errno;
		}
		while(read(f1.fd, &ch1, 1)) {
			if(ch1 == '\0') {
				flag = 1;
				break;
			}
		}
		close(f1.fd);
		f2.fd = open(f2.filename, O_RDONLY);
		f1.fd = open(f1.filename, O_RDONLY);
		if(f2.fd == -1) {
			printf("mydiff: %s: No such file or directory\n", f2.filename);
			return errno;
		}
		while(flag == 1 && read(f2.fd, &ch2, 1) && read(f1.fd, &ch1, 1))  {
			if(ch1 != ch2) {
				flags = 1;
				break;
			}
		}
		if(flags == 1) {
			printf("Binary files %s and %s differ\n", f1.filename, f2.filename);
			exit(-1);
		}
		else if(flag == 1 && flags == 0) {
			exit(-1);
		}
		f1.nol = lineseparator(f1.filename, f1.lines);
		f2.nol = lineseparator(f2.filename, f2.lines);
		d = shortestpath(&f1, &f2);
	}
	else {
		d2.dirname = argv[argc - 1];
		d2.dd = opendir(d2.dirname);
		if(d2.dd == NULL) { 
			printf("mydiff: %s: Not a directory\n", d2.dirname);
			return errno;
		}
		recdirdiff(&d1, &d2);
	}
}

/*
 * Takes two arguments filename and array of pointers to strings
 * Separates the lines in the file in the array of pointers to strings
 * returns the number of lines in the file
 *
 */

int lineseparator(char *filename, char **lines) {
	int m, i = 0, j = 0, fd;
	char ch, templine[1024];
	fd = open(filename, O_RDONLY);
	while(read(fd, &ch, 1)) {
		if(ch != '\n') {
			templine[i++] = ch;	
		}
		else {
			templine[i] = '\0';
			lines[j++] = (char *)malloc(strlen(templine) + 1);
			strcpy(lines[j - 1], templine);
			i = 0;
		}
	}
	m = j;
	close(fd);
	return m;
}
