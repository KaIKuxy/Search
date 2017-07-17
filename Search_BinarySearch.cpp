#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <ctime>
using namespace std;

long *Index;
int tot;

void Load_Index();
void Binary_Search(char *, FILE*, FILE *);

int main() {
	FILE *fsearch = NULL, *fdic = NULL, *fans = NULL, *flog = NULL;
	clock_t tstart, tend, ostart, oend, Max = -1, Min = 999999, temp;
	tstart = clock();
	int i = 0;
	char input[1024] = "", Key[1024] = "";
	fopen_s(&flog, "runtime_bi.log", "w");
	ostart = clock();
	Load_Index();
	oend = clock();
	fprintf_s(flog, "Load index: %ldms\n", oend - ostart);
	fopen_s(&fsearch, "search.txt", "r");
	fopen_s(&fdic, "Dic_ordered_utf8.csv", "r");
	fopen_s(&fans, "fans_bi.txt", "w");
	while (fgets(input, 1024, fsearch) != NULL) {
		ostart = clock();
		if (strcmp(input, "") == 0)
			break;
		for (i = 0;input[i] != ',' && input[i] != '\n';++i);
		strncpy_s(Key, input, i);
		Binary_Search(Key, fdic, fans);
		oend = clock();
		temp = oend - ostart;;
		fprintf_s(flog, "runtime: %ldms\n", temp);
		Max = max(Max, temp);
		Min = min(Min, temp);
	}
	fclose(fsearch);
	fclose(fdic);
	fclose(fans);
	tend = clock();
	fprintf_s(flog, "Whole Query: %.3lfs\n", double((tend - tstart) / CLOCKS_PER_SEC));
	fprintf_s(flog, "Fastest: %ldms\nSlowest: %ldms\n", Min, Max);
	fclose(flog);
	return 0;
}

void Load_Index() {
	FILE * findex = NULL;
	fopen_s(&findex, "binary_index.bin", "rb");
	fread(&tot, sizeof(int), 1, findex);
	Index = (long*)malloc(sizeof(long) * tot);
	fread(Index, sizeof(long), tot, findex);
	fclose(findex);
}

void Binary_Search(char *Key, FILE * fdic, FILE *fans) {
	int start = 0, end = tot - 1, mid = 0;
	while (start <= end) {
		mid = (start + end) >> 1;
		int i = 0;
		char input[1024] = "", temp[1024] = "";
		fseek(fdic, Index[mid], 0);
		fgets(input, 1024, fdic);
		for (i = 0;input[i] != ',' && input[i] != '\n';++i);
		strncpy_s(temp, input, i);
		int ans = strcmp(temp, Key);
		if (ans == 0) {
			fprintf_s(fans, "%s Data found: %s", Key, input);
			//printf_s("Found!\n");
			return;
		}
		else if (ans > 0)
			end = mid - 1;
		else
			start = mid + 1;
	}
	fprintf_s(fans, "Didn't find %s!\n", Key);
	//printf_s("Didn't find!\n");
}