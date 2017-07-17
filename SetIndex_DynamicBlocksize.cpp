#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;
long Index[1000000];
int tot, Blocksize, Blocknum;
int main() {
	FILE *fstart = NULL, *findex = NULL;
	char input[1024] = "";
	long temp;
	int i = 0;
	fopen_s(&fstart, "Dic_ordered_utf8.csv", "r");
	temp = ftell(fstart);
	while (fgets(input, 1024, fstart) != NULL) {
		if (strcmp(input, "") == 0)
			break;
		Index[tot ++] = temp;
		temp = ftell(fstart);
	}
	fclose(fstart);
	Blocksize = int(sqrt(tot));
	Blocknum = tot / Blocksize + !(tot % Blocksize);
	fopen_s(&findex, "block_index.bin", "wb");
	fwrite(&Blocknum, sizeof(int), 1, findex);
	for (i = 0;i < tot;i = i + Blocksize)
		fwrite(&Index[i], sizeof(long), 1, findex);
	fclose(findex);
	printf_s("Set Block size: %d\n", Blocksize);
	getchar();
	return 0;
}