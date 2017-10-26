#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int lineseparator(char *filename, char **lines); 
int main(int argc, char *argv[]) {
	int fd, i = 0, fnol, pnol, flag = 0, x = 0, y = 0, start, end, f = 0, k, j = 0, m = 0;
	char *flines[1024], *plines[1024], str[32], *final[1024], lsnum[32], lenum[32];
	typedef struct dellines {
		int start;
		int end;
	}dellines;
	typedef	struct appendat {
		int index;
		int start;
		int end;
	}appendat;
	dellines d[60];
	appendat a[60];
	fnol = lineseparator(argv[1], flines);
	pnol = lineseparator(argv[2], plines);
	for(i = 0; i < pnol; i++) {
		k = 0;
		if(plines[i][0] == '<' || plines[i][0] == '>') {
			continue;	
		}
		else {
			for(j = 0; plines[i][j]; j++) {
				if(plines[i][j] != 'd' && plines[i][j] != 'a') {
					str[k++] = plines[i][j];
				}
				else if(plines[i][j] == 'd') {
					str[k] = '\0';
					m = 0;
					for(k = 0; str[k]; k++) {
						if(str[k] != ',') {
							lsnum[m++] = str[k];
						}
						else {
							flag = 1;
							lsnum[m] = '\0';
							d[x].start = atoi(lsnum);
							m = 0;
						}
							
					}
					if(flag == 1) {
						lsnum[m] = '\0';
						d[x].end = atoi(lsnum);
						m = 0;
						flag = 0;
					}
					else {
						lsnum[m] = '\0';
						d[x].start = atoi(lsnum);
						d[x].end = d[x].start;
						m = 0;
					}
					x++;
					k = 0;
					break;
				}
				else if(plines[i][j] == 'a') {
					str[k] = '\0';
					a[y].index = atoi(str);
					k = 0;
					for(j = j + 1; plines[i][j]; j++) {
						if(plines[i][j] != ',') {
							str[k++] = plines[i][j];
						}
						else {
							flag = 1;
							str[k] = '\0';
							start = atoi(str);	
							k = 0;
						}
					}
					if(flag == 1) {
						str[k] = '\0';
						end = atoi(str);
						flag = 0;
					}
					else {
						str[k] = '\0';
						start = atoi(str);
						end = start;
					}
					k = 0;
					if(start == end) {
						a[y].start = i + 2;
						a[y].end = i + 2;
					}
					else {
						a[y].start = i + 2;
						a[y].end = end - start + i + 2;
					}
					y++;
					break;
				}
			}
		}
	}
	x = 0;
	y = 0;
	f = 0;
	if(a[y].index == 0) {
		for(i = a[y].start - 1; i < a[y].end; i++) {
			final[f] = (char *)malloc(strlen(plines[i] + 1));
			strcpy(final[f], &plines[i][2]);
			f++;
		}
		y++;
	}
	for(i = 0; i < fnol; i++) {
		if(i == a[y].index - 1) {
			for(j = a[y].start - 1; j < a[y].end; j++) {
				final[f] = (char *)malloc(strlen(plines[j] + 1));
				strcpy(final[f], &plines[j][2]); 
				f++;
			}
		}
		if(!(i >= d[x].start - 1 && i < d[x].end)) {
			final[f] = (char *)malloc(strlen(flines[i]) + 1);
			strcpy(final[f], flines[i]); 
			f++;
		}
		if(i >= d[x].end) 
			x++;
		if(i >= a[y].index) 
			y++;
	}
	i = 0;
	fd = open(argv[1], O_TRUNC | O_CREAT | O_WRONLY);
	while(i < f) {
		for(j = 0; final[i][j]; j++) {
			write(fd, &final[i][j], 1);	
		}
		write(fd, "\n", 1);
		i++;
	}
	close(fd);
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
