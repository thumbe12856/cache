#include "cache.h"

int byte_to_mem = 0, byte_from_mem = 0;
int hit = 0, miss = 0;
int cache_index, associative, block_size;
char replace_policy[10];
//char ***cache_data;
int ***cache_data;
int *fifo;
int **lru, *lru_now;
int **lfu, *lfu_now;

void initail_cache(int c_index, int ass, int index_size, int tag_size, char r_policy[], int b_size)
{
	int i, j, k;
	cache_index = c_index;
	associative = ass;
	block_size = b_size;
	strcpy(replace_policy, r_policy);
	
	/*
	 * dirty bit :	cache_data[index][block][0]
	 * valid bit :	cache_data[index][block][1]
	 * tag :		cache_data[index][block][2]
	 */
	cache_data = (int ***)malloc(sizeof(int **) * cache_index);
	fifo = (int *)malloc(sizeof(int ) * cache_index);
	lru = (int **)malloc(sizeof(int* ) * cache_index);
	lfu = (int **)malloc(sizeof(int* ) * cache_index);
	lfu_now = (int *)malloc(sizeof(int ) * cache_index);
	lru_now = (int *)malloc(sizeof(int ) * cache_index);
	for(i=0; i<cache_index; i++)
	{
		cache_data[i] = (int **)malloc(sizeof(int*) * associative);
		lru[i] =  (int *)malloc(sizeof(int ) * associative);
		lfu[i] =  (int *)malloc(sizeof(int ) * associative);
		for(j=0; j<associative; j++)
			cache_data[i][j] = (int*)malloc(sizeof(int) * 4);
	}

	for(j=0; j<cache_index; j++)
	{
		fifo[j] = 0;
		lru_now[j] = 0;
		lfu_now[j] = 0;
		for(i=0; i<associative; i++)
		{
			lru[j][i] = 0;
			lfu[j][i] = 0;
			cache_data[j][i][0]=0;
			cache_data[j][i][1]=0;
			cache_data[j][i][2]=0;
		}
	}
	return;
}

void set_cache(int index, int tag, int option)
{
	int i, j, miss_swit=1;
	for(i=0; i<associative; i++)
	{
		if(cache_data[index][i][1] == 0)	//when block is not full and instruction
		{
			miss++;
			byte_from_mem++;//read data from memory
			lru_now[index]++;//lru now +1
			for(j=0; j<lru_now[index]; j++)
				lru[index][j]++;

			lfu_now[index]++;//lfu now +1
			lfu[index][i] = 1;

			which_cache_to_replace(index, tag, i, option);
			return;
		}
		else if(cache_data[index][i][1]!=0 && cache_data[index][i][2] == tag) //hit
		{
			hit++;
			if(option==1)//write
				cache_data[index][i][0] = 1;
			
			if(strcmp(replace_policy, "LRU")==0) hLRU(index, i);
			else if(strcmp(replace_policy, "LFU")==0) hLFU(index, i);
			return;
		}
	}

	if(miss_swit) // when block is full and miss
	{
		miss++;
		byte_from_mem++;//read data from memory
		replace(index, tag, option);
	}
	return;
}

/* option:
 * 0: read
 * 1: write
 * 2: instruction
 */
void replace(int index, int tag, int option)
{
	if(strcmp(replace_policy, "FIFO")==0) mFIFO(index, tag, option);
	else if(strcmp(replace_policy, "LRU")==0) mLRU(index, tag, option);
	else if(strcmp(replace_policy, "LFU")==0) mLFU(index, tag, option);
	return;
}

void which_cache_to_replace(int index, int tag, int ass, int option)
{
	if(cache_data[index][ass][0]==1)//dirty bit == 1
		byte_to_mem++;//write data back to memory
	//read || write
	if(option==0 || option==1)
	{
		//read : dirty bit = 0
		//write : dirty bit = 1
		cache_data[index][ass][0] = option;
	}
	else if(option==2)
		cache_data[index][ass][0] = 0;

	cache_data[index][ass][1] = 1;
	cache_data[index][ass][2] = tag;
	return;
}

void mFIFO(int index, int tag, int option)
{
	which_cache_to_replace(index, tag, fifo[index]%associative, option);
	fifo[index]++;
	return;
}

void hLRU(int index, int target)
{
	int i;
	for(i=0; i<lru_now[index]; i++)
		lru[index][i]++;
	lru[index][target] = 1;
	return;
}

void mLRU(int index, int tag, int option)
{
	int i, old = -1, old_index;
	for(i=0; i<lru_now[index]; i++)
	{
		if(lru[index][i] > old)
		{
			old = lru[index][i];
			old_index = i;
		}
		lru[index][i]++;
	}
	lru[index][old_index] = 1;
	which_cache_to_replace(index, tag, old_index, option);
	return;
}

void hLFU(int index, int target)
{
	lfu[index][target]++;
	//lru[index][target] = 1;
	return;
}

void mLFU(int index, int tag, int option)
{
	int i, smallest = 2100000000, smallest_index;
	for(i=0; i<lfu_now[index]; i++)
	{
		lru[index][i]++;
		if(smallest > lfu[index][i])
		{
			smallest = lfu[index][i];
			smallest_index = i;
		}
		else if(smallest == lfu[index][i])//frequency the same
		{
			if(lru[index][smallest_index] < lru[index][i])//who is older
			{
				smallest = lfu[index][i];
				smallest_index = i;
			}
		}
	}
	lfu[index][smallest_index] = 1;
	lru[index][smallest_index] = 1;

	which_cache_to_replace(index, tag, smallest_index, option);
}

void cache_print(int demand_count, int r_data_count, int w_data_count)
{
	int i, j;
	for(j=0; j<cache_index; j++)
	{
		//printf("*%d:  ", j);
		for(i=0; i<associative; i++)
		{
			//printf("***%d:  dirty:%d, valid:%d, tag:%d, lfu times:%d, lru times:%d\n",i ,cache_data[j][i][0], cache_data[j][i][1] ,cache_data[j][i][2], lfu[j][i], lru[j][i]);
			if(cache_data[j][i][0]==1) byte_to_mem++;//dirty bit ==1, write back to memory
		}
		//printf("\n");
	}
	printf("Demand fetch:\t\t%10d\n", demand_count);
	printf("Cache hit:\t\t%10d\nCache miss:\t\t%10d\nMiss rate:\t\t%10.4f\n", hit, miss, ((float)miss/(float)(hit+miss)));
	printf("Read data:\t\t%10d\nWrite data:\t\t%10d\n", r_data_count, w_data_count);
	printf("Bytes from memory:\t%10d\nBytes to memory:\t%10d\n",byte_from_mem*block_size, byte_to_mem*block_size);
	return;
}
