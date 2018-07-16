/*
 * BOOK: Ritchie & Kernighan ( The C programming language )
 * Exercise 1-24
 * Write a program to check a C program for syntax errors like unbalanced parenthesis, brackets and braces. 
 * Don't forget about quotes, both single and double, escape sequences, and comments.
 * Cases:
 * () 	0
 * [] 	1
 * {} 	2
 * ''	4
 * 0x0a ESC < Ignored. 
 * \/\* 5
 * gcc source.c -o out
 * ./out path/to/file.c
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
void checkF(char s[]);
void checkL(char s[], int pattern, int n_found, int opattern, int cpattern);
int getValidPat(int opat, int cpat);
int main(int argc, char *argv[]){
	if(argc <= 1 ){
		printf("%s path/to/file\n", argv[0]);
	}else{
		FILE* file;
		if((file = fopen(argv[1], "r")) == NULL){
			printf("File cannot be opened OR it's empty.\n");
			return -1;
		}else{
			size_t pos = ftell(file);    // Current position
 			fseek(file, 0, SEEK_END);    // Go to end
 			size_t len = ftell(file); // read the position which is the size
 			fseek(file, pos, SEEK_SET);  // restore original position
			
			// Setting up the buffer to hold the text of the file.
			char* text = (char*) malloc (sizeof(char)*len);
			if (text == NULL) {fputs ("Memory error",stderr); return (-1);}
			size_t result = fread (text,1,len,file);
			if (result != len) {fputs ("Reading error",stderr); return (-1);}

			//printf("LENGTH: %ld\n", len);
			checkF(text);
			free(text);
			fclose(file);
			printf("File length:%ld\nDone.\n", len);
		}
	}

	return 0;
}

void checkF(char s[]){
	int i;
	int total;
	if(strstr(s, "(") != NULL || strstr(s, ")") != NULL){
		i = 0;
		int opar = 0;
		int cpar = 0;
		for (int c = 0; c < strlen(s); c++){ 
			if( s[c] == '(' ){
				opar++;
				i++;
			}
			if( s[c] == ')' ){
				cpar++;
				i--;
			}
		}
		printf("Found %d matched parentheses.\n", getValidPat(opar, cpar));
		checkL(s, 0, i, opar, cpar); 
	}
	if(strstr(s, "[") != NULL || strstr(s, "]") != NULL){
		i = 0;
		int obrak = 0;
		int cbrak = 0;
		for (int c = 0; c < strlen(s); c++){ 
			if( s[c] == '[' ){
				obrak++;
				i++;
			}
			if( s[c] == ']' ){
				cbrak++;
				i--;
			}
		}
		printf("Found %d matched brackets.\n", getValidPat(obrak, cbrak));
		checkL(s, 1, i, obrak, cbrak); 
	}
	if(strstr(s, "{") != NULL || strstr(s, "}") != NULL){
		i = 0;
		int obrac = 0;
		int cbrac = 0;
		for (int c = 0; c < strlen(s); c++){ 
			if( s[c] == '{' ){
				obrac++;
				i++;
			}
			if( s[c] == '}' ){
				cbrac++;
				i--;
			}
		}
		printf("Found %d matched braces.\n", getValidPat(obrac, cbrac));
		checkL(s, 2, i, obrac, cbrac); 
	}
	if(strstr(s, "/*") != NULL || strstr(s, "*/") != NULL){
		i = 0;
		int ocmnt = 0;
		int ccmnt = 0;
		for (int c = 0; c < strlen(s); c++){ 
			if( s[c] == '/' )
				if( s[c+1] == '*' ){
					ocmnt++;
					i++;
				}	
			if( s[c] == '*' )
				if( s[c+1] == '/' ){
					ccmnt++;
					i--;
				}	
		}
		printf("Found %d matched comments.\n", getValidPat(ocmnt, ccmnt));
		checkL(s, 3, i, ocmnt, ccmnt); 

	}
	if(strstr(s, "\"") != NULL){
		i = 0;
		int dquote = 0;
		for (int c = 0; c < strlen(s); c++){ 
			if( s[c] == '"' ){
				dquote++;
				i++;
			}
		}
		printf("Found %d double quotes.\n", i);
		checkL(s, 4, i, dquote, dquote); 

	}
	if(strstr(s, "'") != NULL){
		i = 0;
		int squote = 0;
		for (int c = 0; c < strlen(s); c++){ 
			if( s[c] == '\'' ){
				squote++;
				i++;
			}
		}
		printf("Found %d single quotes.\n", i);
		checkL(s, 5, i, squote, squote); 


	}
	/* Unrecognized */
	if(strstr(s, "(") == NULL \
			&& strstr(s, "[") == NULL \
			&& strstr(s, "{")  == NULL \
			&& strstr(s, "/*") == NULL \
			&& strstr(s, "\"") == NULL \
			&& strstr(s, "'")  == NULL){
		//printf("Unrecognized.\n");
	}
}
void checkL(char s[],int pattern, int n_found, int opattern, int cpattern){
	int nn_found = 0;
	switch(pattern){
		case 0:
			if(n_found == 0)
				printf("(OK)\n");
			else{
				int base;
				int aprch;
				if(opattern > cpattern){
					base  = opattern;
					aprch = cpattern;
				}
				else if(opattern < cpattern){
					base  = cpattern;
					aprch = opattern;
				}
				printf("Unmatched parentheses\n");
				printf("Syntax error: f:%d-')'-n:%d\n", getValidPat(opattern, cpattern)
											  , base-aprch);
			}
			break;
		case 1:
			if(n_found == 0)
				printf("[OK]\n");
			else{
				int base;
				int aprch;
				if(opattern > cpattern){
					base  = opattern;
					aprch = cpattern;
				}
				else if(opattern < cpattern){
					base  = cpattern;
					aprch = opattern;
				}
				printf("Unmatched brackets\n");
				printf("Syntax error: f:%d-']'-n:%d\n", getValidPat(opattern, cpattern)
											  , base-aprch);
			}

			break;
		case 2:
			if(n_found == 0)
				printf("{OK}\n");
			else{
				int base;
				int aprch;
				if(opattern > cpattern){
					base  = opattern;
					aprch = cpattern;
				}
				else if(opattern < cpattern){
					base  = cpattern;
					aprch = opattern;
				}
				printf("Unmatched braces\n");
				printf("Syntax error: f:%d-'}'-n:%d\n", getValidPat(opattern, cpattern)
											  , base-aprch);
			}
			break;

		case 3:
			if(n_found == 0)
				printf("/*OK*/\n");
			else{
				int base;
				int aprch;
				if(opattern > cpattern){
					base  = opattern;
					aprch = cpattern;
				}
				else if(opattern < cpattern){
					base  = cpattern;
					aprch = opattern;
				}
				printf("Unmatched comments\n");
				printf("Syntax error: f:%d-'*/'-n:%d\n", getValidPat(opattern, cpattern)
											  , base-aprch);
			}
			break;
		
		case 4:
			if(n_found%2 == 0)
				printf("\"OK\"\n");
			else{
				printf("Unmatched double quotes\n");
				printf("Syntax error: f:%d-'\"'-n:%d\n", n_found, n_found/n_found); 
			}
			break;

		case 5:
			if(n_found%2 == 0)
				printf("'OK'\n");
			else{
				printf("Unmatched quotes\n");
				printf("Syntax error: f:%d-\"'\"-n:%d\n", n_found, n_found/n_found); 
			}

			break;
	}
}
int getValidPat(int opat, int cpat){
	int vbrak = 0;
	if(opat != 0 && cpat != 0){
		while(opat != 0 && cpat != 0){
			vbrak++;
			opat--;
			cpat--;
		}
	}else{
		vbrak = 0;
	}
	return vbrak;

}
