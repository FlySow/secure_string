#ifndef __SECURE_STRING__
#define __SECURE_STRING__

#include <stdlib.h>

typedef struct s_char_node {
	struct s_char_node* next;
	struct s_char_node* pred;
	char byte;
} char_node;


typedef struct s_string {
	char_node* root;
	char_node* tail;
	size_t size;
} string;

typedef struct s_string_ll {
	string str;
	struct s_string_ll* next;
	struct s_string_ll* pred;
} slinked_list;

// Turns a char array into a string according to the struct above
string stringify(char* array, int len);

// str[i] 
char sget(string str, int i);

// Concatenate the two strings (both strings will be altered, advising to copy both strings first)
string scat(string a, string b);

// Copies the entire string, by reallocating etc, beware to free it
string scopy(string str);

// Compares the two strings by returning the difference of the first chars that arn't equal
int scmp(string a, string b);

// Returns the index of the first occurrence of occ in str
int socc(string str, string occ);

// Split on the first occurrence of occ in str, returns a linked list
slinked_list* split_one(string str, string occ);

// Split everytime occ string appears in string str, and returns a linked list (str will be altered, advising to copy str first)
slinked_list* split(string str, string occ);

// Simply prints str with a new line
void println(string str);

void printsll(slinked_list* ll);

// Freeing the string
void sfree(string str);

// Remove chars from position strt to end
string sremrange(string str, int strt, int end);

// Concatenate b with a at the position i in a
string scati(string a, string b, int i);

// Set char to position i in str
void sset(string str, char c, int i);

#endif
