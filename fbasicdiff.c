#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include "diff.h"

int main(int argc, char *argv[]) {
	int i = 0, j = 0, m, n, d;
	file1 f1;
	file2 f2;
	f1.filename = argv[1];
	f2.filename = argv[2];
	f1.fd = open(f1.filename, O_RDONLY);
	if(f1.fd == -1) {
		printf("mydiff: %s: No such file or directory\n", f1.filename);
		return errno;
	}
	f2.fd = open(f2.filename, O_RDONLY);
	if(f2.fd == -1) {
		printf("mydiff: %s: No such file or directory\n", f2.filename);
		return errno;
	}
	f1.nol = lineseparator(f1.filename, f1.lines);
	f2.nol = lineseparator(f2.filename, f2.lines);
	for(i = 0; i < f1.nol; i++) {
		//printf("%s\n", f1.lines[i]);
	}
	for(i = 0; i < f2.nol; i++) {
		//printf("%s\n", f2.lines[i]);
	}
	d = shortestpath(&f1, &f2);
	//printf("Shortest number of edits %d\n", d);
}
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
