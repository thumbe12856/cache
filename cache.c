#include "cache.h"

int hit = 0, miss = 0;
int cache_index;
int associative;
char replace_policy[10];
//char ***cache_data;
int ***cache_data;
int *fifo;

void initail_cache(int c_index, int ass, int index_size, int tag_size, char r_policy[])
{
	cache_index = c_index;
	associative = ass;
	strcpy(replace_policy, r_policy);
	//valid bit: cache_data[index][block][0]
	//tag: cache_data[index][block][1]
	cache_data = malloc(sizeof(int **) * cache_index);
	fifo = malloc(sizeof(int ) * cache_index);
	int i, j, k;
	for(i=0; i<cache_index; i++)
	{
		cache_data[i] = malloc(sizeof(int*) * associative);
		for(j=0; j<associative; j++)
			cache_data[i][j] = malloc(sizeof(int) * 2);
	}

	for(j=0; j<cache_index; j++)
		for(i=0; i<associative; i++)
			cache_data[j][i][0]=0;
	return;
}

void set_cache(int index, int tag)
{
	int i, miss_swit=1;
	for(i=0; i<associative; i++)
	{
		if(cache_data[index][i][0] == 0)	//when block is not full
		{
			miss++;
			cache_data[index][i][0] = 1;
			cache_data[index][i][1] = tag;
			return;
		}
		else if(cache_data[index][i][0] && cache_data[index][i][1] == tag) //hit
		{
			hit++;
			miss_swit=0;
			break;
		}
	}

	if(miss_swit) // when block is full and miss
	{
		miss++;
		replace(index, tag);
	}
	return;
}

void replace(int index, int tag)
{
	if(strcmp(replace_policy, "FIFO")==0)
	{
		//printf("FIFO!!\n");
		mFIFO(index, tag);
	}
	return;
}

void mFIFO(int index, int tag)
{
	cache_data[index][fifo[index]%associative][0] = 1;
	cache_data[index][fifo[index]%associative][1] = tag;
	fifo[index]++;
	return;
}


void check_cache(int index, int tag)
{
	int i, miss_swit=1;
	for(i=0; i<associative; i++)
	{
		if(cache_data[index][i][0] == 0)	//when block is not full
		{
			miss++;
			return;
		}
		else if(cache_data[index][i][0] && cache_data[index][i][1] == tag) //hit
		{
			hit++;
			return;
		}
	}
	if(miss_swit) // when block is full and miss
		miss++;
	return;
}

void cache_print()
{
	printf("\n\nreplace_policy:%s\n", replace_policy);
	printf("cache_index:%d, associative:%d\n", cache_index, associative);
	printf("hit:%d, miss:%d\n", hit, miss);
	int i, j;
	/*cache_data[2][0][0] = 1;
	cache_data[2][0][1] = 118;*/
	for(j=0; j<cache_index; j++)
	{
		printf("%d:  ", j);
		for(i=0; i<associative; i++)
		{
			printf("%d:  valid:%d, tag:%d\t",i ,cache_data[j][i][0] ,cache_data[j][i][1]);
		}
		printf("\n");
	}
}