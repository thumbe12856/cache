#include "cache.h"

int byte_to_mem = 0, byte_from_mem = 0;
int hit = 0, miss = 0;
int cache_index;
int associative;
char replace_policy[10];
//char ***cache_data;
int ***cache_data;
int *fifo;

void initail_cache(int c_index, int ass, int index_size, int tag_size, char r_policy[])
{
	int i, j, k;
	cache_index = c_index;
	associative = ass;
	strcpy(replace_policy, r_policy);
	
	/*
	 * dirty bit :	cache_data[index][block][0]
	 * valid bit :	cache_data[index][block][1]
	 * tag :		cache_data[index][block][2]
	 */
	cache_data = malloc(sizeof(int **) * cache_index);
	fifo = malloc(sizeof(int ) * cache_index);
	for(i=0; i<cache_index; i++)
	{
		cache_data[i] = malloc(sizeof(int*) * associative);
		for(j=0; j<associative; j++)
			cache_data[i][j] = malloc(sizeof(int) * 3);
	}

	for(j=0; j<cache_index; j++)
		for(i=0; i<associative; i++)
		{
			cache_data[j][i][0]=0;
			cache_data[j][i][1]=0;
			cache_data[j][i][2]=0;
		}
	return;
}

void set_cache(int index, int tag, int option)
{
	int i, miss_swit=1;
	for(i=0; i<associative; i++)
	{
		if(cache_data[index][i][1] == 0)	//when block is not full and instruction
		{
			//printf("miss\n\n");
			miss++;
			cache_data[index][i][1] = 1;
			cache_data[index][i][2] = tag;

			if(option==0 || option==1)
			{
				byte_from_mem++;//read data from memory

				//read : dirty bit = 0
				//write : dirty bit = 1
				cache_data[index][i][0] = option;
			}
			else if(option==2)
				cache_data[index][i][0] = 0;

			return;
		}
		else if(cache_data[index][i][1] && cache_data[index][i][2] == tag) //hit
		{
			//printf("hit\n\n");
			hit++;
			if(option==1)//write
				cache_data[index][i][0] = 1;
			return;
		}
	}

	if(miss_swit) // when block is full and miss
	{
		//printf("miss\n\n");
		miss++;
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
	if(strcmp(replace_policy, "FIFO")==0)
	{
		//printf("FIFO!!\n");
		mFIFO(index, tag, option);
	}
	return;
}

void mFIFO(int index, int tag, int option)
{
	//read=0 || write=1
	if(option==0 || option==1)
	{
		if(cache_data[index][fifo[index]%associative][0]==1)//dirty bit == 1
			byte_to_mem++;//write data back to memory
		byte_from_mem++;//read data from memory

		//read : dirty bit = 0
		//write : dirty bit = 1
		cache_data[index][fifo[index]%associative][0] = option;
	}
	else if(option==2)
		cache_data[index][fifo[index]%associative][0] = 0;

	cache_data[index][fifo[index]%associative][1] = 1;
	cache_data[index][fifo[index]%associative][2] = tag;

	fifo[index]++;
	return;
}

void cache_print()
{
	int i, j;
	/*cache_data[2][0][0] = 1;
	cache_data[2][0][1] = 118;*/
	for(j=0; j<cache_index; j++)
	{
		printf("%d:  ", j);
		for(i=0; i<associative; i++)
			printf("%d:  dirty:%d, valid:%d, tag:%d\t",i ,cache_data[j][i][0], cache_data[j][i][1] ,cache_data[j][i][2]);
		printf("\n");
	}

	printf("\n\nreplace_policy:%s\n", replace_policy);
	printf("cache_index:%d, associative:%d\n", cache_index, associative);
	printf("hit:%d, miss:%d\n", hit, miss);
	printf("byte_from_mem:%d, byte_to_mem:%d\n",byte_from_mem, byte_to_mem);
	return;
}