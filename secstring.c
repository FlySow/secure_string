#include "secstring.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


string stringify(char* array, int len) {
	if(len == 0) return (string) {size: 0, root: NULL, tail: NULL};
	char_node* root = malloc(sizeof(char_node));
	if(len == 1) {
		root->byte = array[0];
		root->pred = NULL;
		root->next = NULL;
		return (string) {size: 1, root: root, tail: root};
	}
	char_node* temp = root;
	temp->pred = NULL;
	for(int i = 0; i < len-1; i++) {
		temp->byte = array[i];
		temp->next = malloc(sizeof(char_node));
		temp->next->pred = temp;
		temp = temp->next;
	}
	temp->next = NULL;
	temp->byte = array[len-1];
	return (string) {size: len, root: root, tail: temp};
}


string scat(string a, string b) {
	if(a.size == 0) return b;
	if(b.size == 0) return a;

	a.tail->next = b.root;
	b.root->pred = a.tail;
	return (string) {size: a.size+b.size, root: a.root, b.tail};
}


string scopy(string str) {
	if(str.size == 0) return (string) {size: 0, root: NULL, tail: NULL};
	char_node* root = malloc(sizeof(char_node));
	char_node* cpy = root;
	char_node* temp = str.root;
	cpy->pred = NULL;
	while(temp->next != NULL) {
		cpy->byte = temp->byte;
		cpy->next = malloc(sizeof(char_node));
		cpy->next->pred = cpy;
		cpy = cpy->next;
		temp = temp->next;
	}
	cpy->byte = temp->byte;
	cpy->next = NULL;
	return (string) {size: str.size, root: root, tail: cpy};
}


char sget(string str, int i) {
	assert(i < str.size);
	
	char_node* temp = str.root;
	for(int j = 0; j < i; j++) {
		temp = temp->next;
	}
	return temp->byte;
}


char_node* sget_p(string str, int i) {
	assert(i < str.size);
	
	char_node* temp = str.root;
	for(int j = 0; j < i; j++) {
		temp = temp->next;
	}
	return temp;
}


int scmp(string a, string b) {
	if(a.size == 0 || b.size == 0) return -1;
	char_node* temp1 = a.root;
	char_node* temp2 = b.root;
	
	while(temp1->byte == temp2->byte && temp1->next != NULL && temp2->next != NULL) {
		temp1 = temp1->next;
		temp2 = temp2->next;
	}

	return temp1->byte - temp2->byte;
}


// sget -> retrieve something in the string; p -> returns pointer; cn -> char_node; o -> uses an offset to navigate through char_node* chain
char_node* sgetpcno(char_node* node, int offset) {
	if(offset > 0) {
		for(int i = 0; i < offset; i++) {
			if(node == NULL) return NULL;
			node = node->next;
		}
		return node;
	}
	for(int i = 0; i > offset; i--) {
		if(node == NULL) return NULL;
		node = node->pred;
	}
	return node;
}


int socc(string str, string occ) {
	if(str.size < occ.size) return -1;

	if(occ.size == 0) return -1;

	int global_index = 0;
	int offset = occ.size-1;
	
	char_node* temp_str = str.root;
	char_node* temp_occ = occ.tail;
	
	while((temp_str = sgetpcno(temp_str, offset)) != NULL) {
		int i = 0;
		while(temp_occ->byte == temp_str->byte && temp_occ->pred != NULL && temp_str->pred != NULL) {
			temp_occ = temp_occ->pred;
			temp_str = temp_str->pred;
			i++;
		}
		if(temp_occ->byte == temp_str->byte) return global_index;
		
		char current_byte = temp_str->byte;
		
		int j = 0;
		while(temp_occ->byte != current_byte && temp_occ->pred != NULL) {
			temp_occ = temp_occ->pred;
			j++;
		}
		if(temp_occ->byte != current_byte) j++;

		offset = i + j;
		global_index+= j;
		
		temp_occ = occ.tail;
	}
	return -1;
}

//Free shit
slinked_list* split_one(string str, string occ) {
	int i = socc(str, occ);
	if(i == -1) {
		slinked_list* res = malloc(sizeof(slinked_list));
		res->str = str;
		res->pred = NULL;
		res->next = NULL;
		return res;
	}

	if(i == 0) {
		if(str.size == occ.size) return NULL;

		char_node* strt = sgetpcno(str.root, occ.size);
		strt->pred = NULL;
		slinked_list* res = malloc(sizeof(slinked_list));
		res->str = (string) {size: str.size-occ.size, root: strt, tail: str.tail};
		res->pred = NULL;
		res->next = NULL;
		return res;
	}
	if(i+occ.size >= str.size) {
		char_node* endstr = sgetpcno(str.root, i-1);
		endstr->next = NULL;

		slinked_list* res = malloc(sizeof(slinked_list));
		res->str = (string) {size: str.size-occ.size, root: str.root, tail: endstr};
		res->pred = NULL;
		res->next = NULL;
		return res;
	}
	
	char_node* endstr1 = sgetpcno(str.root, i-1);
	char_node* strtstr2 = sgetpcno(endstr1, occ.size+1);
	endstr1->next = NULL;
	strtstr2->pred = NULL;

	slinked_list* root = malloc(sizeof(slinked_list));
	root->str = (string) {size: i, root: str.root, tail: endstr1};
	root->pred = NULL;
	root->next = malloc(sizeof(slinked_list));
	root->next->next = NULL;
	root->next->pred = root;
	root->next->str = (string) {size: str.size-i-occ.size, root: strtstr2, tail: str.tail};
	return root;
}


slinked_list* split(string str, string occ) {
	slinked_list* root = NULL;
	slinked_list* temp = NULL;
	
	string temp_str = str;

	while(socc(temp_str, occ) != -1) {
		slinked_list* res = split_one(temp_str, occ);
		if(res->next == NULL) {
			temp_str = res->str;
		}else {
			if(root == NULL) {
				root = res;
				temp = res;
			} else {
				temp->next = res;
				res->pred = temp;
				temp = res;
			}
			temp_str = res->next->str;
			free(res->next);
			res->next = NULL;
		}
	}
	temp->next = malloc(sizeof(slinked_list));
	temp->next->str = temp_str;
	temp->next->next = NULL;
	temp->next->pred = temp;
	return root;	
}


void println(string str) {
	char_node* temp = str.root;

	for(int i = 0; i < str.size; i++) {
		printf("%c", temp->byte);
		temp = temp->next;
	}
	printf("\n");
}


void printsll(slinked_list* ll) {
	while(ll != NULL) {
		println(ll->str);
		ll = ll->next;
	}
}


void sfree(string str) {
	if(str.size == 0) return;

	char_node* temp = str.root;
	for(int i = 0; i < str.size-1; i++) {
		temp = temp->next;
		free(temp->pred);
	}
	free(temp);
}


string sadd(string str, char c, int i) {
	assert(i <= str.size);
	
	char_node* new = malloc(sizeof(char_node));
	new->pred = NULL;
	new->next = NULL;
	new->byte = c;

	if(i == str.size) {
		if(str.size == 0) {
			str.root = new;
			str.tail = new;
		} else {
			new->pred = str.tail;
			str.tail->next = new;
			str.tail = new;
		}
		str.size++;
		return str;
	}

	char_node* temp = i < str.size/2 ? sgetpcno(str.root, i) : sgetpcno(str.tail, i-str.size+1);
	
	if(i == 0) {
		str.root->pred = new;
		new->next = str.root;
		new->pred = NULL;
		str.root = new;
	} else {
		char_node* temp = i < str.size/2 ? sgetpcno(str.root, i) : sgetpcno(str.tail, str.size-i-1);
		temp->pred->next = new;
		new->pred = temp->pred;
		new->next = temp;
		temp->pred = new;
	}
	str.size++;
	return str;
}


string srem(string str, int i) {
	assert(i < str.size);

	char_node* temp = i < str.size/2 ? sgetpcno(str.root, i) : sgetpcno(str.tail, i-str.size+1);
	
	if(i == 0) {
		str.root = temp->next;
		if(i != str.size-1) temp->next->pred = NULL;
	} else if(i == str.size-1) {
		str.tail = temp->pred;
		if(i != 0) temp->pred->next = temp->next;
	} else {
		temp->pred->next = temp->next;
		temp->next->pred = temp->pred;
	}
	free(temp);
	str.size--;
	return str;
}


string scati(string a, string b, int i) {
	assert(i <= a.size);

	if(i == 0) return scat(b, a);
	if(i == a.size) return scat(a, b);

	char_node* endstr1 = i < a.size/2 ? sgetpcno(a.root, i-1) : sgetpcno(a.tail, i-a.size);
	char_node* strtstr2 = endstr1->next;
	strtstr2->pred = NULL;
	endstr1->next = NULL;
	return scat((string) {size: i, root: a.root, tail: endstr1}, scat(b, (string) {size: a.size-i, root: strtstr2, tail: a.tail}));
}


string sremrange(string str, int strt, int end) {
	assert(strt < str.size && end < str.size);
	
	char_node* temp = strt < str.size/2 ? sgetpcno(str.root, strt) : sgetpcno(str.tail, strt-str.size+1);
	char_node* endstr1 = temp->pred;
	for(int i = strt; i < end; i++) {
		temp = temp->next;
		free(temp->pred);
	}
	char_node* strtstr2 = temp->next;
	free(temp);

	if(strt == 0 && end == str.size-1) {
		str.root = NULL;
		str.tail = NULL;
	} else if(strt == 0)  {
		str.root = strtstr2;
		strtstr2->pred = NULL;
	} else if(end == str.size-1) {
		str.tail = endstr1;
		endstr1->next = NULL;
	} else {
		endstr1->next = strtstr2;
		strtstr2->pred = endstr1;
	}
	str.size = str.size - end + strt - 1;
	return str;
}


void sset(string str, char c, int i) {
	assert(i < str.size);

	char_node* temp = i < str.size/2 ? sgetpcno(str.root, i) : sgetpcno(str.tail, i-str.size+1);

	temp->byte = c;
}
