#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "diff.h"

/*
 * Checks if the passed string is a name of a directoy or a file 
 *
 */

int isdir(dir1 *d1, int i) {
	DIR *dir;
	char path[128];
	strcpy(path, d1->dirname);
	strcat(path, "/");
	strcat(path, d1->names[i]);
	dir = opendir(path);
	if(dir == NULL) 
		return 0;
	else 
		return 1;
}

/*
 * Opens both the directories and copies the names of elements of directory into array 
 * of pointers
 * Also counts the number of elements in the directory and stores them
 * Calls dirshortestpath to find differences
 *
 */

void recdirdiff(dir1 *d1, dir2 *d2) {
	d1->noe = 0;
	d1->dd = opendir(d1->dirname);
	while((d1->pdir = readdir(d1->dd)) != NULL) {
		if(strcmp(d1->pdir->d_name, ".") != 0 && strcmp(d1->pdir->d_name, "..") != 0) {
			d1->names[d1->noe] = (char *)malloc(strlen(d1->pdir->d_name) + 1);
			strcpy(d1->names[d1->noe], d1->pdir->d_name);
			(d1->noe)++;
		}
	}
	d2->noe = 0;
	d2->dd = opendir(d2->dirname);
	while((d2->pdir = readdir(d2->dd)) != NULL) {
		if(strcmp(d2->pdir->d_name, ".") != 0 && strcmp(d2->pdir->d_name, "..") != 0) {
			d2->names[d2->noe] = (char *)malloc(strlen(d2->pdir->d_name) + 1);
			strcpy(d2->names[d2->noe], d2->pdir->d_name);
			(d2->noe)++;
		}
	}
	dirshortestpath(d1, d2);
}

/*
 * Produces difference of directories in a format similar to diff command
 * Makes a recursive call to recdirdiff function if -r option is set to true
 *
 */

void normaldirdiff(dir1 *d1, dir2 *d2) {
	int i = 0, j, k = 0, l, temp, d, flag = 0, flags = 0;
	char path1[128], path2[128], ch1, ch2; 
	file1 f1;
	file2 f2;
	dir1 d11;
	dir2 d22;
	for(i = 0; i <= d1->noe; i++) {
		if(d1->insfiles[i] > 0) {
			for(; k < d2->noe; k++) {
				if(d2->toinsfiles[k] == 1) {
					l = 0;
					while(l < d1->insfiles[i]) {
						l++;
					}
					temp = k;
					for(;k < temp + l; k++) {
						printf("Only in %s: %s\n", d2->dirname, d2->names[k]);
					}
					break;
				}
			}
		}
		if(i != d1->noe && d1->delfiles[i] == 0) {
			if(isdir(d1, i)) {
				strcpy(path1, d1->dirname);
				strcat(path1, "/");
				strcat(path1, d1->names[i]);
				strcpy(path2, d2->dirname);
				strcat(path2, "/");
				strcat(path2, d1->names[i]);
				if(recursive_dir_diff == false)
					printf("Common Subdirectories: %s and %s\n", path1, path2); 
				else {
					d11.dirname = (char *)malloc(strlen(path1) + 1);
					strcpy(d11.dirname, path1);
					d22.dirname = (char *)malloc(strlen(path2) + 1);
					strcpy(d22.dirname, path2);
					recdirdiff(&d11, &d22);
				}
			}
			else {
				flag = 0;
				flags = 0;
				strcpy(path1, d1->dirname);
				strcat(path1, "/");
				strcat(path1, d1->names[i]);
				f1.filename = (char *)malloc(strlen(path1) + 1);
				strcpy(f1.filename, path1);
				f1.fd = open(f1.filename, O_RDONLY);
				while(read(f1.fd, &ch1, 1)) {
					if(ch1 == '\0') {
						flag = 1;
						break;
					}
				}
				close(f1.fd);
				strcpy(path2, d2->dirname);
				strcat(path2, "/");
				strcat(path2, d1->names[i]);
				f2.filename = (char *)malloc(strlen(path2) + 1);
				strcpy(f2.filename, path2);
				f1.fd = open(f1.filename, O_RDONLY);
				f2.fd = open(f2.filename, O_RDONLY);
				while(flag == 1 && read(f2.fd, &ch2, 1) && read(f1.fd, &ch1, 1))  {
					if(ch1 != ch2) {
						flags = 1;
						break;
					}
				}
				if(flags == 1) {
					printf("Binary files %s and %s differ\n", f1.filename, f2.filename);
					free(f1.filename);
					free(f2.filename);
					continue;
				}
				else if(flag == 1 && flags == 0) {
					free(f2.filename);
					free(f1.filename);
					continue;
				}
				f1.nol = lineseparator(f1.filename, f1.lines);
				f2.nol = lineseparator(f2.filename, f2.lines);
				printf("diff %s %s\n", f1.filename, f2.filename);
				d = shortestpath(&f1, &f2);
			}
		}
		if(i != d1->noe && d1->delfiles[i] == 1) {
			j = i;
			while(j < d1->noe && d1->delfiles[j] == 1) {
				j++;
			}
			for(;i < j; i++) {
				printf("Only in %s: %s\n", d1->dirname, d1->names[i]);
			}
			i--;
		}
	}
}
/*
 * Stores the data of files to be inserted and deleted
 * and passes that data to normaldirdiff
 * 
 */
void dirbacktrack(int maxop, int x, int y, int d, int *trace, dir1* d1, dir2* d2) {
	int prevx, prevy, prevk;
	int k, i = d, *vertices;
	d1->delfiles = (int *)calloc(d1->noe, sizeof(int));
	d1->insfiles = (int *)calloc(d1->noe + 1, sizeof(int));
	d2->toinsfiles = (int *)calloc(d2->noe, sizeof(int));
	for(d = i; d > 0; d--) {
		k = x - y;
		vertices = trace + (d - 1) * (maxop + 1) + d - 1;
		if(k == -d || (k != d && vertices[k - 1] < vertices[k + 1]))
			prevk = k + 1;
		else 	
			prevk = k - 1;
		prevx = vertices[prevk];
		prevy = prevx - prevk;
		while(x > prevx && y > prevy) {
			x--;
			y--;
		}
		if(d > 0) {
			if(x > prevx && y == prevy) {
				d1->delfiles[prevx] = 1;
			}
			else if(y > prevy && x == prevx) {
				d2->toinsfiles[prevy] = 1;
				(d1->insfiles[prevx])++ ;
			}
		}
		x = prevx;
		y = prevy;
	}
	normaldirdiff(d1, d2);
}
/*
 * This function uses graph search to find minimum number of edits i.e 
 * insertions and deletions required to convert directory1 to directory2. 
 *
 */
int dirshortestpath(dir1* d1, dir2* d2) {
	int d, i, k, x, y, result, j;
	int maxop = d1->noe + d2->noe;
	int *vertices, *trace, *temp;
	trace = (int *)malloc(sizeof(int) * (maxop + 1) * (2 * maxop + 1));
	temp = (int *)malloc((2 * maxop + 1) * sizeof(int));
	for(i = 0; i < 2 * maxop + 1; i++) {
		temp[i] = 0;
	}
	vertices = &temp[maxop];
	vertices[1] = 0;
	int dmove, kprev, xmid, xstart, ystart, ymid, xend, yend;
	for(d = 0; d <= maxop; d++) {
		for(k = -d; k <= d; k += 2) {
			if(k == -d) 
				dmove = 1;
			else if(k != d && vertices[k - 1] < vertices[k + 1]) 
				dmove = 1;
			else 
				dmove = 0;

			kprev = dmove ? k + 1 : k - 1;

			xstart = vertices[kprev];
			ystart = xstart - kprev;

			xmid = dmove ? xstart : xstart + 1;
			ymid = xmid - k;

			xend = xmid;
			yend = ymid;

			while(xend < d1->noe && yend < d2->noe && (result = strcmp(d1->names[xend], d2->names[yend]) == 0)) {
				xend++;
				yend++;
			}

			vertices[k] = xend;

			for(i = 0; i < 2 * d + 1; i++) {
				*(trace + d * (maxop + 1) + i) = vertices[i - d];
			}

			if(xend >= d1->noe && yend >= d2->noe) {
				dirbacktrack(maxop, xend, yend, d, trace, d1, d2);
				return d;
			}
		}
	}
}

