#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <ctime>
#include <cmath>
#define MAXLEN 1024

FILE *dict = NULL, *query = NULL, *ans = NULL, *flog = NULL;
clock_t ts, te;

struct Trie {
	bool is_tail, is_end;
	long file_index;
	char* word;
	Trie *index[200];
	Trie() {
		is_tail = false;
		is_end = false;
		file_index = 0;
		for (int i = 0;i < 200;++ i)
			index[i] = NULL;
		word = "\0";
	}
};

class TrieTree {
public:
	TrieTree();
	~TrieTree();
	bool del(Trie *curr);
	bool insert(char *str, long file_index);
	long get(char *str);
	bool check(char *a, char *b);
	void adjust(Trie *curr);
	void load();
	void search();
private:
	Trie *root;
}test;

char *write_tail(char *str);

TrieTree::TrieTree() {
	root = new Trie();
}

TrieTree::~TrieTree() {
	del(root);
}

bool TrieTree::del(Trie *curr) {
	if (curr == NULL)
		return false;
	for (int i = 0;i < 200;++ i)
		del(curr->index[i]);
	delete curr;
	return true;
}

bool TrieTree::insert(char *str, long file_index) {
	Trie *curr = root, *new_node = NULL;
	if (str == NULL || str[0] == '\0')
		return false;
	for (;*(str) != ',';++str) {
		int temp = -int(*str);
		if (curr->index[temp] == NULL) {
			new_node = new Trie();
			new_node->word = write_tail(str + 1);
			new_node->is_tail = (*(new_node->word) == '\0') ? false : true;
			new_node->is_end = (*(new_node->word) == '\0') ? true : false;
			new_node->file_index = file_index;
			curr->index[temp] = new_node;
			return true;
		}
		else {
			if (curr->index[temp]->is_tail == true)
				adjust(curr->index[temp]);
			curr = curr->index[temp];
		} 
	}
	curr->is_end = true;
	return true;
}

long TrieTree::get(char *str) {
	Trie* curr = root;
	if (str == NULL || *str == '\0')
		return false;
	for (;*str != ',';++str) {
		int temp = -int(*str);
		if (curr->is_tail == true)
			return (check(str, curr->word) == true) ? curr->file_index : -1;
		else {
			if (curr->index[temp] == NULL)
				return -1;
			else
				curr = curr->index[temp];
		}
	}
	return (curr->is_end == true) ? curr ->file_index : -1;
}

bool TrieTree::check(char *input, char *word) {
	for (;*input != ',' && *word != '\0';++input, ++word) {
		if (*input != *word)
			return false;
	}
	if (*input != ',' || *word != '\0')
		return false;
	else
		return true;
}

void TrieTree::adjust(Trie *curr) {
	Trie *new_node = NULL;
	char temp[MAXLEN] = "", ch = *(curr -> word);
	int pos = 1, next = -int(ch);
	while (*(curr -> word + pos) != '\0') {
		temp[pos - 1] = *(curr -> word + pos);
		++ pos;
	}
	temp[pos - 1] = *(curr -> word + pos);
	++ pos;
	new_node = new Trie();
	new_node->is_tail = (pos == 2) ? false : true;
	new_node->is_end = (pos == 2) ? true : false;
	new_node->file_index = curr->file_index;
	new_node->word = write_tail(temp);
	free(curr->word);
	curr->is_tail = false;
	curr->index[next] = new_node;
	curr->file_index = 0;
}

char *write_tail(char *str) {
	char *p = NULL;
	int len = 0;
	while (*(str + len) != ',' && *(str + len) != '\0')	++len;
	p = (char *)malloc(sizeof(char) * len + 1);
	for (int i = 0;i < len;++ i)
		*(p + i) = *(str + i);
	*(p + len) = '\0';
	return p;
}

void TrieTree::load() {
	fopen_s(&flog, "runtime_compressed_trie.log", "w");
	fopen_s(&dict, "Dic_ordered_utf8.csv", "r");
	clock_t loads = clock(), loade;
	char input[MAXLEN] = "";
	long pos = ftell(dict);
	while ((fgets(input, MAXLEN, dict)) != NULL) {
		if (*input == '\0')
			break;
		insert(input, pos);
		pos = ftell(dict);
	}
	fclose(dict);
	loade = clock();
	fprintf_s(flog, "Constructing compressed trie: %.3lfs\n", double(loade - loads) / CLOCKS_PER_SEC);
}

void TrieTree::search() {
	fopen_s(&query, "search.txt", "r");
	fopen_s(&ans, "ans_compressed_trie.txt", "w");
	fopen_s(&dict, "Dic_ordered_utf8.csv", "r");
	char input[MAXLEN] = "";
	clock_t s, e;
	while (fgets(input, MAXLEN, query) != NULL) {
		s = clock();
		long index = get(input);
		if (index > -1) {
			fseek(dict, index, 0);
			fgets(input, MAXLEN, dict);
			fprintf_s(ans, "Found! %s", input);
		}
		else
			fprintf_s(ans, "Didn't find...\n");
		e = clock();
		fprintf_s(flog, "runtime: %ldms\n", e - s);
	}
	fclose(query);
	fclose(ans);
	fclose(dict);
}

int main() {
	ts = clock();
	test.load();
	test.search();
	te = clock();
	fprintf_s(flog, "Total: %.3lfs\n", double(te - ts) / CLOCKS_PER_SEC);
	fclose(flog);
	return 0;
}