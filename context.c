#include<time.h>
#include<sys/stat.h>
#include<stdio.h>
#include "diff.h"

/* 
 * Shows output in context format
 *
 */

void contextformat(file1 *f1, file2 *f2) {
	int i = 0, j = 0, count = 0, k = 0, flag = 0, l = 0, x = 0;
	struct stat attrib;
	stat(f1->filename, &attrib);
	char time[100];
	strftime(time, 100, "%Y-%m-%d %H:%M:%S", localtime(&attrib.st_mtime));
	printf("*** %s %s\n", f1->filename, time);
	stat(f2->filename, &attrib);
	strftime(time, 100, "%Y-%m-%d %H:%M:%S", localtime(&attrib.st_mtime));
	printf("--- %s %s\n", f2->filename, time);
	lineseparator(f1->filename, f1->lines);
	lineseparator(f2->filename, f2->lines);
	while(i < f1->nol && j < f2->nol) {
		count = 0;
		k = i;
		for(; i < f1->nol && count < 3; i++) {
			if(f1->dellines[i] == 0) {
				count++;
			}
			else {
				count = 0;
				flag = 1;
			}
		}
		printf("***************\n");
		printf("*** %d,%d ****\n", k + 1, i);
		if(flag == 1) {
			for( ; k < i; k++) {
				if(f1->dellines[k] == 0) {
					printf("  %s\n", f1->lines[k]);
					count++;
				}
				else {
					printf("- %s\n", f1->lines[k]);
				}
			}
		}
		flag = 0;
		count = 0;
		k = j;
		for(; j < f2->nol && count < 3; j++) {
			if(f2->toinslines[j] == 0) {
				count++;
			}
			else { 
				count = 0;
				flag = 1;
			}
		}
		printf("--- %d,%d ----\n", k + 1, j);
		if(flag == 1 ) {
			for(; k < j; k++) {
				if(f2->toinslines[k] == 0) {
					printf("  %s\n", f2->lines[k]);
					count++;
				}
				else {
					printf("+ %s\n", f2->lines[k]);
				}
			}
		}
		flag = 0;
	}
	if(i < f1->nol) {
		printf("*** %d, %d ****\n", i + 1, f1->nol);
		while(i < f1->nol) {
			if(f1->dellines[i] == 0) {
				printf("  %s\n", f1->lines[i]);
			}
			else {
				printf("- %s\n", f1->lines[i]);
			}
			i++;
		}
	}
	else if(j < f2->nol) {
		printf("--- %d, %d ----\n", j + 1, f2->nol);
		for(; j < f2->nol; j++) {
			if(f2->toinslines[j] == 0) {
				printf("  %s\n", f2->lines[j]);
			}
			else {
				printf("+ %s\n", f2->lines[j]);
			}
		}
	}
}
