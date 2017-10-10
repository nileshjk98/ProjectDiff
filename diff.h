#define MAX 1024

typedef struct file1 {
	int fd;
	char *filename;
	int nol;
	char *lines[MAX];
	int *dellines;
	int *inslines;
}file1;

typedef struct file2 {
	int fd;
	char *filename;
	int nol;
	char *lines[MAX];
	int *toinslines;
}file2;

typedef enum bool {
	false, true 
}bool;

int lineseparator(char *filename, char **lines);
int shortestpath(file1 *f1, file2 *f2);
void normaldiff(file1 *f1, file2 *f2);
void strlwr(char *str);
void ignorespace(file1 *f1, file2 *f2);
void ignorespchange(file1 *f1, file2 *f2);
void ignorecase(file1 *f1, file2 *f2);
