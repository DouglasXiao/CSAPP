#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#define MAGIC_LRU_NUM 999

typedef struct
{
	int valid;
	int tag;
	int LruNumber;
} Line;

typedef struct {
	Line* lines;
} Set;

typedef struct {
	int set_num;
	int line_num;
	Set* sets;
} Cache;

int get_Opt(int argc, char **argv, int *s, int *E, int *b, char *tracefileName, int *isVerbose);
void printHelpMenu();
void checkOptarg(char *outOptarg);
void initMemory(int s, int E, Cache *cache);
int getSet(int addr, int s, int b);
int getTag(int addr, int s, int b);
void updateCacheLRU(Cache cache, int setBits, int i);
void modifyData(Cache cache, int setBits, int tagBits, int isVerbose);
void saveData(Cache cache, int setBits, int tagBits, int isVerbose);
void loadData(Cache cache, int setBits, int tagBits, int isVerbose);
void freeMemory(Cache *cache);

int hits;
int misses;
int evictions;

int main(int argc, char **argv)
{
	int s, E, b, isVerbose = 0;
	char tracefileName[100], opt[10];
	Cache cache;
	hits = 0;
	misses = 0;
	evictions = 0;

	int addr, size;

	get_Opt(argc, argv, &s, &E, &b, tracefileName, &isVerbose);
	initMemory(s, E, &cache);

	FILE *fp = fopen(tracefileName, "r");
	if (fp == NULL) {
		printf("File open error.\n");
		exit(0);
	}
	while (fscanf(fp, "%s %x,%d", opt, &addr, &size) != EOF) {
		if (strcmp(opt, "I") == 0) {
			continue;
		}
		//printf("addr:%x, size:%d, opt: %s\n", addr, size, opt);
		printf("---------------------------------------------------\n");
		int setBits = getSet(addr, s, b);
		int tagBits = getTag(addr, s, b);
		printf("setBits: %d , tagBits: %d \n", setBits, tagBits);
		//printf("setBits: %d, tagBits: %d\n", setBits, tagBits);
		if (isVerbose == 1) {
			printf("%s %x,%d ",opt,addr,size);
		}
		if (strcmp(opt, "L") == 0) {
			loadData(cache, setBits, tagBits, isVerbose);
		} else if (strcmp(opt, "S") == 0) {
			saveData(cache, setBits, tagBits, isVerbose);
		} else if (strcmp(opt, "M") == 0) {
			modifyData(cache, setBits, tagBits, isVerbose);
		}
		if (isVerbose == 1) {
			printf("\n");
		}
	}

    printSummary(hits, misses, evictions);
    //freeMemory(&cache);
    return 0;
}

void freeMemory(Cache *cache) {
	//free(cache->sets);
	free(cache);
}

void updateCacheLRU(Cache cache, int setBits, int i) {
	int j;
	for (j = 0; j < cache.line_num; ++j) {
		if (j == i) {
			cache.sets[setBits].lines[j].LruNumber = MAGIC_LRU_NUM;
		} else {
			cache.sets[setBits].lines[j].LruNumber--;
		}
	}
}

void loadData(Cache cache, int setBits, int tagBits, int isVerbose) {
	int j;
	int smallest = MAGIC_LRU_NUM + 1;
	int rec;
	for (j = 0; j < cache.line_num; ++j) {
		if (cache.sets[setBits].lines[j].tag == tagBits && cache.sets[setBits].lines[j].valid == 1) { // Means already hit
			updateCacheLRU(cache, setBits, j);
			if (isVerbose == 1) {
				printf("hit ");
			}
			hits++;
			return;
		}
	}

	// If cannot find the corresponding tag, try to find the first empty space
	for (j = 0; j < cache.line_num; ++j) {
		if (cache.sets[setBits].lines[j].valid == 0) {
			cache.sets[setBits].lines[j].tag = tagBits;
			cache.sets[setBits].lines[j].valid = 1;
			updateCacheLRU(cache, setBits, j);
			if (isVerbose == 1) {
				printf("miss ");
			}
			misses++;
			return;
		}
	}

	// If cannot find an empty space,then find the smallest LRU number to evict
	for (j = 0; j < cache.line_num; ++j) {
		if (smallest > cache.sets[setBits].lines[j].LruNumber) {
			smallest = cache.sets[setBits].lines[j].LruNumber;
			rec = j;
		}
	}
	if (isVerbose == 1) {
		printf("miss eviction ");
	}
	misses++;
	evictions++;
	cache.sets[setBits].lines[rec].valid = 1;
	cache.sets[setBits].lines[rec].tag = tagBits;
	updateCacheLRU(cache, setBits, rec);
	return;
}

void saveData(Cache cache, int setBits, int tagBits, int isVerbose) {
	loadData(cache, setBits, tagBits, isVerbose);
}

void modifyData(Cache cache, int setBits, int tagBits, int isVerbose) {
	loadData(cache, setBits, tagBits, isVerbose);
	saveData(cache, setBits, tagBits, isVerbose);
}

int getSet(int addr, int s, int b) {
	return ((addr >> b) & ((1 << s) - 1));
}

int getTag(int addr, int s, int b) {
	return (addr >> (s + b));
}

void initMemory(int s, int E, Cache *cache) {
	if (s < 0) {
		printf("Invalid set number");
		exit(0);
	}
	cache->set_num = 2 << s;
	cache->line_num = E;
	cache->sets = (Set *)malloc(cache->set_num * sizeof(Set));
	if (!cache->sets) {
		printf("Cache sets memory error!");
		exit(0);
	}
	int i, j;
	for (i = 0; i < cache->set_num; ++i) {
		cache->sets[i].lines = (Line *)malloc(cache->line_num * sizeof(Line));
		if (!cache->sets[i].lines) {
			printf("Cache lines memory error!");
			exit(0);
		}
		for (j = 0; j < cache->line_num; ++j) {
			cache->sets[i].lines[j].valid = 0;
			cache->sets[i].lines[j].LruNumber = 0;
		}
	}
	return;
}

int get_Opt(int argc, char **argv, int *s, int *E, int *b, char *tracefileName, int *isVerbose) {
	int c;
	while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
		switch (c) {
			case 'v':
			*isVerbose = 1;
			break;
			case 's':
			checkOptarg(optarg);
			*s = atoi(optarg);
			break;
			case 'E':
			checkOptarg(optarg);
			*E = atoi(optarg);
			break;
			case 'b':
			checkOptarg(optarg);
			*b = atoi(optarg);
			break;
			case 't':
			checkOptarg(optarg);
			strcpy(tracefileName, optarg);
			break;
			case 'h':
			default:
			printHelpMenu();
			exit(0);
		}
	}

	printf("v=%d; s=%d; E=%d; b=%d; tracefileName=%s\n", *isVerbose, *s, *E, *b, tracefileName);
	return 1;
}

void printHelpMenu() {
	printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("-h         Print this help message.\n");
    printf("-v         Optional verbose flag.\n");
    printf("-s <num>   Number of set index bits.\n");
    printf("-E <num>   Number of lines per set.\n");
    printf("-b <num>   Number of block offset bits.\n");
    printf("-t <file>  Trace file.\n\n\n");
    printf("Examples:\n");
    printf("linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
	printf("linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}


void checkOptarg(char *outOptarg) {
	if (outOptarg[0] == '-') {
		printf("./csim :Missing required command line argument\n");
		printHelpMenu();
		exit(0);
	}
}