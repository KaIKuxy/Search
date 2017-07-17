#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
char input[1000000][1024];
char *dire[1000000];
bool cmp(const char *, const char*);
int main() {
	FILE *f1 = NULL, *f2;
	fopen_s(&f1, "Dic_utf8.csv", "r");
	fopen_s(&f2, "Dic_ordered_utf8.csv", "w");
	int i = 0, j = 0;
	while (fgets(input[i++], 1024, f1) != NULL)
		dire[i - 1] = input[i - 1];
	sort(dire, dire + i - 1, cmp);
	for (j = 0;j < i - 1;++j)
		fprintf_s(f2, "%s", dire[j]);
	fclose(f1);
	fclose(f2);
	return 0;
}

bool cmp(const char * a, const char* b) {
	return strcmp(a, b) < 0;
}