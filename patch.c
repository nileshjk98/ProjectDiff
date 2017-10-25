#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char *argv[]) {
	int fd1, fd2, i = 0, fnol, pnol, flag = 0, x = 0, y = 0, start, end;
	char *flines[1024], *plines[1024], str[32], *final[1024], *lsnum, *lenum;
	struct dellines {
		int start;
		int end;
	}dellines;
	struct appendat {
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
			for(j = 0; p[i][j]; j++) {
				if(p[i][j] != 'd' && p[i][j] != 'a') {
					str[k++] = p[i][j];
				}
				else if(p[i][j] == 'd') {
					str[k] = '\0';
					lsnum = strtok(str, ",");
					d[x].start = atoi(lsnum);
					lenum = strtok(NULL, "\0");
					d[x].end = atoi(lenum);
					x++;
					k = 0;
					break;
				}
				else if(p[i][j] == 'a') {
					str[k] = '\0';
					a[y].index = atoi(str);
					k = 0;
					for(j = j + 1; p[i][j]; j++) {
						if(p[i][j] != ',') {
							str[k++] = p[i][j];
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
						k = 0;
						flag = 0;
					}
					else {
						str[k] = '\0';
						start = atoi(str);
						end = start;
						k = 0;
					}
					if(start == end) {
						a[y].start = i + 1;
						a[y].end = i + 1;
						y++;
					}
					else {
						a[y].start = i + 1;
						a[y].end = end - start + i + 1;
						y++;
					}
					break;
				}
			}
		}
	}
	for(i = 0; i < fnol; i++) {
			
	}

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
