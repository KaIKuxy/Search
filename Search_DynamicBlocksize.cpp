#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <ctime>
using namespace std;

FILE *fsearch = NULL, *fans = NULL, *flog = NULL;
clock_t records, recorde, tots, tote;
long Min_time, Max_time;
long Index[1000000];
int tot;
FILE *fstart;

int Find_Block(char *);
void Search_Block(char *, int, FILE*, FILE*);
void Load_Index();
int cmp(long, char *);

int main() {
	char Key[1024] = "", input[1024] = "";
	int i = 0, temp = 0;
	fopen_s(&flog, "runtime_block.log", "w");
	Load_Index();
	Min_time = 100000;
	Max_time = -1;
	tots = clock();
	fopen_s(&fstart, "Dic_ordered_utf8.csv", "r");
	fopen_s(&fsearch, "search.txt", "r");
	fopen_s(&fans, "ans_block.txt", "w");
	while (fgets(input, 1024, fsearch) != NULL) {
		records = clock();
		if (strcmp(input, "") == 0)
			break;
		for (i = 0;input[i] != ',' && input[i] != '\n';++i);
		strncpy_s(Key, input, i);
		int POS = Find_Block(Key);
		if (POS == -1)
			fprintf_s(fans, "Didn't find %s!\n", Key);
		else
			Search_Block(Key, POS, fstart, fans);
		recorde = clock();
		fprintf_s(flog, "runtime: %ld ms\n", recorde - records);
		Min_time = (Min_time > recorde - records) ? recorde - records : Min_time;
		Max_time = (Max_time < recorde - records) ? recorde - records : Max_time;
	}
	fclose(fstart);
	fclose(fsearch);
	fclose(fans);
	tote = clock();
	fprintf_s(flog, "Whole Query: %.3lf s\n", double(tote - tots) / CLOCKS_PER_SEC);
	fprintf_s(flog, "Fastest: %ldms\nSlowest: %ldms\n", Min_time, Max_time);
	fclose(flog);
	return 0;
}

void Search_Block(char *source, int POS, FILE *fstart, FILE *fans) {
	int i = 0;
	char input[1024] = "", temp[1024] = "";
	fseek(fstart, Index[POS], 0);
	while (fgets(input, 1024, fstart) != NULL) {
		for (i = 0;input[i] != ',';++i);
		strncpy_s(temp, input, i);
		if (strcmp(temp, source) == 0) {
			fprintf_s(fans, "%s Data found: %s", source, input);
			return;
		}
		if (strcmp(temp, source) > 0 || strcmp(input, "") == 0) {
			fprintf_s(fans, "Didn't find %s!\n", source);
			return;
		}
	}
}

void Load_Index() {
	records = clock();
	FILE *findex = NULL;
	fopen_s(&findex, "block_index.bin", "rb");
	fread(&tot, sizeof(int), 1, findex);
	fread(&Index, sizeof(long), tot, findex);
	recorde = clock();
	fprintf_s(flog, "Load Index: %.3lfs\n", double(recorde - records) / CLOCKS_PER_SEC);
}

int cmp(long x, char * y) {
	int i;
	char input[1024] = "", temp[1024] = "";
	fseek(fstart, x, 0);
	fgets(input, 1024, fstart);
	for (i = 0;input[i] != ',' && input[i] != '\n';++i);
	strncpy_s(temp, input, i);
	return strcmp(temp, y);
}

int Find_Block(char * Key) {
	int start = 0, end = tot - 1, mid = 0;
	while (start <= end) {
		mid = (start + end) >> 1;
		short ans1 = cmp(Index[mid], Key), ans2 = cmp(Index[mid + 1], Key);
		if (ans1 <= 0 && (ans2 > 0 || mid == tot - 1))
			return mid;
		else if (ans1 > 0)
			end = mid - 1;
		else
			start = mid + 1;
	}
	return -1;
}