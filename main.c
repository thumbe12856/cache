#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "cache.h"

int main(int argc, char* argv[])
{
	int cache_size = atoi(argv[1])*1024;
	int block_size = atoi(argv[2]);
	int fully_associative_swit =  argv[3][0]=='f' ? 1 : 0;
	int associative = fully_associative_swit==1 ? (cache_size/block_size) : atoi(argv[3]);
	char replace_policy[10];
	strcpy(replace_policy, argv[4]);
	char filename[20];
	strcpy(filename, argv[5]);
	int offset = power_2(block_size);
	int cache_index = (cache_size/block_size)/associative;
	int index_to = power_2(cache_index) + offset -1;
	int index_from = offset;
	int tag_to;
	int tag_from = index_to + 1;

	//printf("cache_index:%d, offset:%d, associative:%d, index_from:%d, index_to:%d, tag_from:%d\n", cache_index, offset, associative, index_from, index_to, tag_from);
	initail_cache(cache_index, associative, index_to-index_from, tag_to-tag_from, replace_policy, block_size);
	//fprintf(stderr, "%s -> -%s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);

	FILE *fp;
	fp = fopen(filename, "r");
	char buf[100], ori_address[6], bin_address[32];
	int hex_to_bin_option, set_cache_option;
	int index, tag;
	int demand_count = 0, r_data_count = 0, w_data_count = 0;

	int test[1000] = {0}, test_now = 0;

	while(fgets(buf, 100, fp))
	{
		demand_count++;
		char c = buf[0];
		get_hex_address(buf, ori_address);

		switch(c)
		{
			case '0':
				//printf("read data\n");
				//hex_to_bin_option = 31;
				if(strlen(ori_address)==8)
				{
					hex_to_bin_option = 31;
					tag_to = 31;
				}
				else if(strlen(ori_address)==6)
				{
					hex_to_bin_option = 23;
					tag_to = 23;
				}
				//tag_to = 31;
				set_cache_option = 0;
				r_data_count++;
			break;

			case '1':
				//printf("write data\n");
				hex_to_bin_option = 31;
				if(strlen(ori_address)==8)
				{
					hex_to_bin_option = 31;
					tag_to = 31;
				}
				else if(strlen(ori_address)==6)
				{
					hex_to_bin_option = 23;
					tag_to = 23;
				}
				set_cache_option = 1;
				w_data_count++;
			break;
			case '2':
				//printf("instruction\n");
				if(strlen(ori_address)==8)
				{
					hex_to_bin_option = 31;
					tag_to = 31;
				}
				else if(strlen(ori_address)==6)
				{
					hex_to_bin_option = 23;
					tag_to = 23;
				}
				set_cache_option = 2;
			break;
		}
		//printf("tag_from:%d, tag_to:%d\n", tag_from, tag_to);
		bin_address[0] = '\0';
		hex_to_bin(ori_address, bin_address, hex_to_bin_option);
		index = fully_associative_swit==1 ? 0 : get_data(bin_address, index_from, index_to);
		tag = get_data(bin_address, tag_from, tag_to);
		//fprintf(stderr, "%s -> %s\nindex:%d, tag:%d\n\n", ori_address, bin_address, index, tag);
		set_cache(index, tag, set_cache_option, demand_count);
	}
	fclose(fp);
	cache_print(demand_count, r_data_count, w_data_count);
	return 0;
}
