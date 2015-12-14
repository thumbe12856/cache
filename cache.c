#include "cache.h"

int cache_index;
int associative;
int **cache_data;

void set_cache(int index, int ass)
{
	cache_index = index;
	associative = ass;
	cache_data = malloc(sizeof(int *) * cache_index);
	int i, j;
	for(i=0; i<cache_index; i++)
		cache_data[i] = malloc(sizeof(int) * associative);

	for(j=0; j<cache_index; j++)
		for(i=0; i<associative; i++)
			cache_data[j][i]=0;
	return;
}

void cache_print()
{
	printf("cache_index:%d, associative:%d\n", cache_index, associative);
	int i, j;
	for(j=0; j<cache_index; j++)
	{
		printf("%d:  ", j);
		for(i=0; i<associative; i++)
		{
			printf("%d ",cache_data[j][i]);
		}
		printf("\n");
	}
}