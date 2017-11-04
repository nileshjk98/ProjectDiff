#include "diff.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
 * Produces output in two columns
 * > represents that line is present in 2nd file only
 * < represents that line is present in 1st file only
 *
 */
void side_by_side(file1 *f1, file2 *f2) {
	int i = 0, j, k = 0, l, temp;
	lineseparator(f1->filename, f1->lines);
	lineseparator(f2->filename, f2->lines);
	expandtabs(f1, f2);
	for(i = 0; i <= f1->nol; i++) {
		if(i != f1->nol && f1->dellines[i] == 0) {
			printf("%-61.61s   %-61.61s\n", f1->lines[i], f2->lines[f1->matchlines[i]]);
		}
		if(i != f1->nol && f1->dellines[i] == 1) {
			j = i;
			while(j < f1->nol && f1->dellines[j] == 1) {
				j++;
			}
			for(;i < j; i++) {
				printf("%-61.61s <\n", f1->lines[i]);
			}
			i--;
		}
		if(f1->inslines[i] > 0) {
			for(; k < f2->nol; k++) {
				if(f2->toinslines[k] == 1) {
					l = 0;
					while(l < f1->inslines[i]) {
						l++;
					}
					temp = k;
					for(;k < temp + l; k++) {
						printf("%-61.61s > %-61.61s\n", " ", f2->lines[k]);
					}
					break;
				}
			}
		}
	}
}

/* 
 * Converts tabs to correct number of spaces in the output
 *
 */

void expandtabs(file1 *f1, file2 *f2) {
	int i = 0, j, m;
	char temp[1024];
	for(i = 0; i < f1->nol; i++) {
		for(j = 0; j < strlen(f1->lines[i]); j++) {
			if(f1->lines[i][j] == '\t') {
				strncpy(temp, f1->lines[i], j);
				for(m = j; m < j + 8; m++) {
					temp[m] = ' ';
				}
				temp[j + 8] = '\0';
				if(f1->lines[i][j + 1] != '\0')
					strcat(temp, &(f1->lines[i][j + 1]));
				f1->lines[i] = (char *)malloc(strlen(temp) + 1);
				strcpy(f1->lines[i], temp);
				j = j + 7;
			}
		}
	}
	for(i = 0; i < f2->nol; i++) {
		for(j = 0; j < strlen(f2->lines[i]); j++) {
			if(f2->lines[i][j] == '\t') {
				strncpy(temp, f2->lines[i], j);
				for(m = j; m < j + 8; m++) {
					temp[m] = ' ';
				}
				temp[j + 8] = '\0';
				if(f2->lines[i][j + 1] != '\0')
					strcat(temp, &(f2->lines[i][j + 1]));
				f2->lines[i] = (char *)malloc(strlen(temp) + 1);
				strcpy(f2->lines[i], temp);
				j = j + 7;
			}
		}
	}
}
