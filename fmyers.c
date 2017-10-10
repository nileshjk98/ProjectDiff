#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "diff.h"

void backtrack(int max, int x, int y, int d, int (*trace)[2 * max + 1], file1* f1, file2* f2) {
	int prevx, prevy, prevk;
	int k, i = d, *v;
	f1->dellines = (int *)calloc(f1->nol, sizeof(int));
	f1->inslines = (int *)calloc(f1->nol + 1, sizeof(int));
	f2->toinslines = (int *)calloc(f2->nol, sizeof(int));
	for(d = i; d > 0; d--) {
		k = x - y;
		v = &trace[d - 1][d - 1];
		if(k == -d || (k != d && v[k - 1] < v[k + 1]))
			prevk = k + 1;
		else 	
			prevk = k - 1;
		prevx = v[prevk];
		prevy = prevx - prevk;
		while(x > prevx && y > prevy) {
			//printf("(%d,%d)->(%d, %d)\n", x - 1, y - 1, x, y);
			x--;
			y--;
		}
		if(d > 0) {
			//printf("(%d,%d)->(%d, %d)\n", prevx, prevy, x, y);
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
	/*printf("Lines to be deleted\n");
	for(i = 0; i < f1->nol; i++) {
		if(f1->dellines[i] == 1) {
			printf("%d %s\n", i, f1->lines[i]);
		}
	}
	printf("Lines to be inserted at position\n");
	for(i = 0; i <= f1->nol; i++) {
		if(f1->inslines[i] > 0) {
			printf("%d %d\n", i, f1->inslines[i]);
		}
	}
	printf("Lines to be inserted\n");
	for(i = 0; i < f2->nol; i++) {
		if(f2->toinslines[i] == 1) {
			printf("%d %s\n", i, f2->lines[i]);
		}
	}*/
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
	int *v, trace[max + 1][2 * max + 1], *temp;
	temp = (int *)malloc((2 * max + 1) * sizeof(int));
	for(i = 0; i < 2 * max + 1; i++) {
		temp[i] = 0;
	}
	v = &temp[max];
	v[1] = 0;
	int down, kprev, xmid, xstart, ystart, ymid, xend, yend;
	for(d = 0; d <= max; d++) {
		for(k = -d; k <= d; k += 2) {
			down = (k == -d || (k != d && v[k - 1] < v[k + 1]));
			kprev = down ? k + 1 : k - 1;

			//start point
			xstart = v[kprev];
			ystart = xstart - kprev;
			//printf("\n%d %d\n", xstart, ystart);
			// mid point
			xmid = down ? xstart : xstart + 1;
			ymid = xmid - k;
			//printf("\n%d %d\n", xmid, ymid);

			// end point
			xend = xmid;
			yend = ymid;
			//printf("\n%d %d\n", xend, yend);

			// follow diagonal
			while(xend < f1->nol && yend < f2->nol && (result = strcmp(f1->lines[xend], f2->lines[yend]) == 0)) {
				xend++;
				yend++;
			}

			// save end point
			v[k] = xend;

			for(i = 0; i < 2 * d + 1; i++) {
				trace[d][i] = v[i - d];
				//printf("%d ", trace[d][i]);
			}
			//printf("\n");
			// check for solution
			if(xend >= f1->nol && yend >= f2->nol) /* solution has been found */ {
				backtrack(max, xend, yend, d, trace, f1, f2);
				return d;
			}
		}
	}
}
void normaldiff(file1 *f1, file2 *f2) {
	int i = 0, countd = 0, j, k = 0, l, temp;
	for(i = 0; i <= f1->nol; i++) {
		if(i != f1->nol && f1->dellines[i] == 1) {
			j = i;
			while(j < f1->nol && f1->dellines[j] == 1) {
				j++;
				countd++;
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
			//printf("Inside Insertion\n");
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
