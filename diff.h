#include<sys/types.h>
#include<dirent.h>

#define MAX 1024
/* 
 * Structure to store file1 data
 * nol is number of lines
 * dellines is pointer to array that stores 0 if the line is
 * repeated in second file and 1 if it is not 
 * matchlines stores the position of matching line in second line
 * inslines is also pointer to array that stores the number of
 * lines to be inserted from file 2 in file1 at that particular index
 *
 */

typedef struct file1 {
	int fd;
	char *filename;
	int nol;
	char *lines[MAX];
	int *dellines;
	int *matchlines;
	int *inslines;
}file1;

/* 
 * Structure to store file2 data
 * toinslines is pointer to array that stores 1 if the line is
 * inserted in first file and 0 if it is not 
 *
 */

typedef struct file2 {
	int fd;
	char *filename;
	int nol;
	char *lines[MAX];
	int *toinslines;
}file2;

/* 
 * Structure to store dir1 data
 * noe is number of elements
 * names store the elements in the directory
 * delfiles is pointer to array that stores 0 if the file is
 * present in second directory and 1 if it is not 
 * insfiles is also pointer to array that stores the number of
 * files to be inserted from file 2 in file1 at that particular index
 *
 */

typedef struct dir1 {
	DIR *dd;
	struct dirent *pdir; 
	char *dirname;
	char *names[512];
	int noe;
	int *delfiles;
	int *insfiles;
}dir1;

/* 
 * Structure to store dir2 data
 * toinsfiles is pointer to array that stores 1 if the file is to be
 * inserted in first directory and 0 if it is not 
 *
 */

typedef struct dir2 {
	DIR *dd;
	struct dirent *pdir; 
	char *dirname;
	char *names[512];
	int noe;
	int *toinsfiles;
}dir2;

typedef enum bool {
	false, true 
}bool;

/* 
 * These are set to true if corresponding option is to be implemented
 *
 */
bool ignore_space_change;
bool recursive_dir_diff ;
bool context_format;
bool sidebyside;
bool normal_diff;
bool expand_tabs;
bool ignore_case;
bool ignore_all_space;

int lineseparator(char *filename, char **lines);
int shortestpath(file1 *f1, file2 *f2);
void normaldiff(file1 *f1, file2 *f2);
void strlwr(char *str);
void ignorespace(file1 *f1, file2 *f2);
void ignorespchange(file1 *f1, file2 *f2);
void ignorecase(file1 *f1, file2 *f2);
void side_by_side(file1 *f1, file2 *f2);
void expandtabs(file1 *f1, file2 *f2);
void recdirdiff(dir1 *d1, dir2 *d2);
int dirshortestpath(dir1 *d1, dir2 *d2);
int isdir(dir1 *d1, int i);
void contextformat(file1 *f1, file2 *f2); 
