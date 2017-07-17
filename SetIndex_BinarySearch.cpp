#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;
long Index[1000000];
int main() {
	char input[1024] = "";
	FILE *fstart = NULL, *findex = NULL;
	fopen_s(&fstart, "Dic_ordered_utf8.csv", "r");
	fopen_s(&findex, "binary_index.bin", "wb");
	int tot = 0;
	long Now = ftell(fstart);
	while (fgets(input, 1024, fstart) != NULL) {
		if (strcmp(input, "") == 0)
			break;
		Index[tot++] = Now;
		Now = ftell(fstart);
	}
	fwrite(&tot, sizeof(int), 1, findex);
	fwrite(Index, sizeof(long), tot, findex);
	fclose(fstart);
	fclose(findex);
	printf_s("Index number: %d\n", tot);
	getchar();
	return 0;
}