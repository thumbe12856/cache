#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "cache.h"

int main(int argc, char* argv[])
{
	int cache_size = atoi(argv[1])*1024;
	int block_size = atoi(argv[2]);
	int associative = atoi(argv[3]);
	char replace_policy[10];
	strcpy(replace_policy, argv[4]);
	int offset = power_2(block_size);

	int cache_index = (cache_size/block_size)/associative;
	int index_to = power_2(cache_index) + offset -1;
	int index_from = offset;
	int tag_to;
	int tag_from = index_to + 1;

	//printf("index_from:%d, index_to:%d\n", index_from, index_to);
	initail_cache(cache_index, associative, index_to-index_from, tag_to-tag_from, replace_policy, block_size);

	FILE *fp;
	fp = fopen("gcc.din", "r");
	char buf[100], ori_address[6], bin_address[32];
	int hex_to_bin_option, set_cache_option;
	int index, tag;
	int demand_count = 0, r_data_count = 0, w_data_count = 0;

	while(fgets(buf, 100, fp))
	{
		demand_count++;
		char c = buf[0];
		//printf("%s -> -%s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);

		switch(c)
		{
			case '0':
				//printf("read data\n");
				//printf("%s -> %s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);
				hex_to_bin_option = 31;
				tag_to = 31;
				set_cache_option = 0;
				r_data_count++;
			break;

			case '1':
				//printf("write data\n");
				//printf("%s -> %s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);
				hex_to_bin_option = 31;
				tag_to = 31;
				set_cache_option = 1;
				w_data_count++;
			break;
			case '2':
				//printf("instruction\n");
				//printf("%s -> %s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);
				hex_to_bin_option = 23;
				tag_to = 23;
				set_cache_option = 2;
			break;
		}

		get_hex_address(buf, ori_address);
		hex_to_bin(ori_address, bin_address, hex_to_bin_option);
		index = get_data(bin_address, index_from, index_to);
		tag = get_data(bin_address, tag_from, tag_to);
		set_cache(index, tag, set_cache_option);
	}
	printf("Demand fetch:%d\n", demand_count);
	printf("Read data:%d, Write data:%d\n", r_data_count, w_data_count);
	cache_print();
	fclose(fp);
	return 0;
}
