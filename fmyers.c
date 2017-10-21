#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "diff.h"

/* 
 * This backtracks through the x values in the graph created by shortestpath 
 * to identify and represent the shortest path and passes this data normaldiff function
 *
 */
void backtrack(int max, int x, int y, int d, int *trace, file1* f1, file2* f2) {
	int prevx, prevy, prevk;
	int k, i = d, *v, j = 0;
	f1->dellines = (int *)calloc(f1->nol, sizeof(int));
	if(f1->dellines == NULL) {
		printf("calloc failed\n");
		exit(-1);
	}
	f1->inslines = (int *)calloc(f1->nol + 1, sizeof(int));
	if(f1->inslines == NULL) {
		printf("calloc failed\n");
		exit(-1);
	}
	f2->toinslines = (int *)calloc(f2->nol, sizeof(int));
	if(f2->toinslines == NULL) {
		printf("calloc failed\n");
		exit(-1);
	}
	f1->matchlines = (int *)calloc(f1->nol, sizeof(int));
	if(f1->matchlines == NULL) {
		printf("calloc failed\n");
		exit(-1);
	}
	for(d = i; d > 0; d--) {
		k = x - y;
		v = trace + (d - 1) * (max + 1) + d - 1;
		if(k == -d || (k != d && v[k - 1] < v[k + 1]))
			prevk = k + 1;
		else 	
			prevk = k - 1;
		prevx = v[prevk];
		prevy = prevx - prevk;
		while(x > prevx && y > prevy) {
			x--;
			y--;
		}
		if(d > 0) {
			if(x > prevx && y == prevy) {
				f1->dellines[prevx] = 1;
			}
			else if(y > prevy && x == prevx) {
				f2->toinslines[prevy] = 1;
				(f1->inslines[prevx])++ ;
			}
		}
		x = prevx;
		y = prevy;
	}
	for(i = 0; i < f1->nol; i++) {
		if(f1->dellines[i] == 0) {
			j = 0;
			while(strcmp(f1->lines[i], f2->lines[j]) != 0) {
				j++;
			}
			f1->matchlines[i] = j;
		}
	}
	if(context_format == true)
		contextformat(f1, f2);
	else if(sidebyside == true)
		side_by_side(f1, f2);
	else
		normaldiff(f1, f2);
}
/*
 * This function uses graph search to find minimum number of edits i.e 
 * insertions and deletions required to convert file1 into file2. 
 *
 */
int shortestpath(file1* f1, file2* f2) {
	int d, i, k, x, y, result, j;
	int max = f1->nol + f2->nol;
	int *v, *trace, *temp;
	int down, kprev, xmid, xstart, ystart, ymid, xend, yend;
	trace = (int *)malloc(sizeof(int) * (max + 1) * (2 * max + 1));
	if(ignore_case == true) 
		ignorecase(f1, f2);
	if(ignore_all_space == true) 
		ignorespace(f1, f2);
	else if(ignore_all_space == false && ignore_space_change == true) 
		ignorespchange(f1, f2);
	if(expand_tabs == true)
		expandtabs(f1, f2);
	temp = (int *)malloc((2 * max + 1) * sizeof(int));
	for(i = 0; i < 2 * max + 1; i++) {
		temp[i] = 0;
	}
	v = &temp[max];
	v[1] = 0;
	for(d = 0; d <= max; d++) {
		for(k = -d; k <= d; k += 2) {
			down = (k == -d || (k != d && v[k - 1] < v[k + 1]));
			kprev = down ? k + 1 : k - 1;

			//start point
			xstart = v[kprev];
			ystart = xstart - kprev;
			// mid point
			xmid = down ? xstart : xstart + 1;
			ymid = xmid - k;

			// end point
			xend = xmid;
			yend = ymid;

			// follow diagonal
			while(xend < f1->nol && yend < f2->nol && (result = strcmp(f1->lines[xend], f2->lines[yend]) == 0)) {
				xend++;
				yend++;
			}

			// save end point
			v[k] = xend;

			for(i = 0; i < 2 * d + 1; i++) {
				*(trace + d * (max + 1) + i) = v[i - d];
			}
			// check for solution
			if(xend >= f1->nol && yend >= f2->nol) /* solution has been found */ {
				backtrack(max, xend, yend, d, trace, f1, f2);
				return d;
			}
		}
	}
}
 
/*
 * Produces output in normal diff format except change statement
 * Uses only delete and append to represent the output
 * This output can be used to patch the two files using patch command
 *
 */

void normaldiff(file1 *f1, file2 *f2) {
	int i = 0, j, k = 0, l, temp;
	for(i = 0; i <= f1->nol; i++) {
		if(i != f1->nol && f1->dellines[i] == 1) {
			j = i;
			while(j < f1->nol && f1->dellines[j] == 1) {
				j++;
			}
			if(i + 1 == j)
				printf("%dd%d\n", i + 1, i);
			else
				printf("%d,%dd%d\n", i + 1, j, i);
			lineseparator(f1->filename, f1->lines);
			for(;i < j; i++) {
				printf("< %s\n", f1->lines[i]);
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
					if(k + 1 == k + l)
						printf("%da%d\n", i, k + l);
					else 
						printf("%da%d,%d\n", i, k + 1, k + l);
					temp = k;
					lineseparator(f1->filename, f1->lines);
					for(;k < temp + l; k++) {
						printf("> %s\n", f2->lines[k]);
					}
					break;
				}
			}
		}
	}
}
