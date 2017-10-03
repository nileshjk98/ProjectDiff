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

enum {
	false, true 
};
typedef int bool;
/*
bool ignore_space_change = false;
bool recursive_dir_diff = false;
bool context_format = false;
bool side_by_side = false;
bool normal_diff = true;
bool expand_tabs = false;
bool ignore_case = false;
bool ignore_all_space = false;
*/
int lineseparator(char *filename, char **lines);
int shortestpath(file1 *f1, file2 *f2);
void normaldiff(file1 *f1, file2 *f2);
