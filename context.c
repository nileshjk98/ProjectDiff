#include<time.h>
#include<sys/stat.h>
#include<stdio.h>
#include "diff.h"

/* 
 * Shows output in context format
 *
 */

void contextformat(file1 *f1, file2 *f2) {
	int i = 0, j = 0, count = 0, k = 0, flag = 0, l = 0;
	struct stat attrib;
	stat(f1->filename, &attrib);
	char time[100];
	strftime(time, 100, "%Y-%m-%d %H:%M:%S", localtime(&attrib.st_mtime));
	printf("*** %s %s\n", f1->filename, time);
	stat(f2->filename, &attrib);
	strftime(time, 100, "%Y-%m-%d %H:%M:%S", localtime(&attrib.st_mtime));
	printf("--- %s %s\n", f2->filename, time);
	printf("***************\n");
	printf("*** %d,%d ****\n", i + 1, f1->nol);
	lineseparator(f1->filename, f1->lines);
	lineseparator(f2->filename, f2->lines);
	for(i = 0 ; i < f1->nol; i++) {
		if(f1->dellines[i] == 0) {
			printf("  %s\n", f1->lines[i]);
		}
		else {
			printf("- %s\n", f1->lines[i]);
		}
	}
	i = 0;
	printf("--- %d,%d ----\n", i + 1, f2->nol);
	for(i = 0 ; i < f2->nol; i++) {
		if(f2->toinslines[i] == 0) {
			printf("  %s\n", f2->lines[i]);
		}
		else {
			printf("+ %s\n", f2->lines[i]);
		}
	}

}
