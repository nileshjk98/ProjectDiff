#include<string.h>
#include<stdlib.h>
#include "diff.h"
/* 
 * Converts string into lowercase characters
 *
 */
void strlwr(char *str) {
	int i;
	for(i = 0; str[i] != '\0'; i++) {
		if(str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = str[i] + ('a' - 'A');
		}
	}
}
/* 
 * Ignores all whitespace characters
 * It removes all the spaces between the lines
 *
 */
void ignorespace(file1 *f1, file2 *f2) {
	char str[1024];
	int i, j, k = 0;
	for(i = 0; i < f1->nol; i++) {
		for(j = 0; f1->lines[i][j] != '\0'; j++) {
			if(f1->lines[i][j] != ' ' && f1->lines[i][j] != '\t') {
				str[k++] = f1->lines[i][j];
			}
		}		
		str[k] = '\0';
		f1->lines[i] = (char *)malloc(strlen(str) + 1);
		strcpy(f1->lines[i], str);
		k = 0;
	}
	for(i = 0; i < f2->nol; i++) {
		for(j = 0; f2->lines[i][j] != '\0'; j++) {
			if(f2->lines[i][j] != ' ' && f2->lines[i][j] != '\t') {
				str[k++] = f2->lines[i][j];
			}
		}		
		str[k] = '\0';
		f2->lines[i] = (char *)malloc(strlen(str) + 1);
		strcpy(f2->lines[i], str);
		k = 0;
	}
}
/* 
 * Ignores space change
 * It converts sequence of whitespace characters into a single whitespace character
 *
 */
void ignorespchange(file1 *f1, file2 *f2) {
	int i, j, k = 0;
	char str[1024], prev;
	for(i = 0; i < f1->nol; i++) {
		if(f1->lines[i][strlen(f1->lines[i]) - 1] == ' ') {
			j = strlen(f1->lines[i]) - 1;
			while(f1->lines[i][j] == ' ') {
				j--;
			}
			f1->lines[i][j + 1] = '\0';
		}
		prev = f1->lines[i][0];
		for(j = 0; f1->lines[i][j] != '\0'; j++) {
			if(prev != ' ') {
				str[k++] = f1->lines[i][j];
				prev = f1->lines[i][j];
			}
		}
		str[k] = '\0';
		k = 0;
		f1->lines[i] = (char *)malloc(strlen(str) + 1);
		strcpy(f1->lines[i], str);
	}
	for(i = 0; i < f2->nol; i++) {
		if(f2->lines[i][strlen(f2->lines[i]) - 1] == ' ') {
			j = strlen(f2->lines[i]) - 1;
			while(f2->lines[i][j] == ' ') {
				j--;
			}
			f2->lines[i][j + 1] = '\0';
		}
		prev = f2->lines[i][0];
		for(j = 0; f2->lines[i][j] != '\0'; j++) {
			if(prev != ' ') {
				str[k++] = f2->lines[i][j];
				prev = f2->lines[i][j];
			}
		}
		str[k] = '\0';
		k = 0;
		f2->lines[i] = (char *)malloc(strlen(str) + 1);
		strcpy(f2->lines[i], str);
	}

}
/* 
 * Ignores case change 
 * Converts all the characters in all the lines to lowercase and then compares
 *
 */
void ignorecase(file1 *f1, file2* f2) {
	int i;
	for(i = 0; i < f1->nol; i++) {
		strlwr(f1->lines[i]);		
	}
	for(i = 0; i < f2->nol; i++) {
		strlwr(f2->lines[i]);		
	}
}
