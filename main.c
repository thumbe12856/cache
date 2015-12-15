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
	int tag_to = 23;
	int tag_from = index_to + 1;


	initail_cache(cache_index, associative, index_to-index_from, tag_to-tag_from, replace_policy);

	FILE *fp;
	fp = fopen("gcc.din", "r");
	char buf[100];
	char ori_address[6];
	char bin_address[24];

	int r_data_count = 0, w_data_count = 0;

	while(fgets(buf, 100, fp))
	{
		char c = buf[0];
		get_hex_address(buf, ori_address);
		hex_to_bin(ori_address, bin_address);
		int index = get_data(bin_address, index_from, index_to);
		int tag;
		//printf("%s -> -%s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);

		switch(c)
		{
			case '0':
				//printf("read data\n\n");
				tag_to = 31;
				tag = get_data(bin_address, tag_from, tag_to);
				check_cache(index, tag);
				r_data_count++;
			break;

			case '1':
				//printf("write data\n\n");
				tag_to = 31;
				tag = get_data(bin_address, tag_from, tag_to);
				check_cache(index, tag);
				w_data_count++;
			break;
			case '2':
				//printf("instruction\n\n");
				/*get_hex_address(buf, ori_address);
				hex_to_bin(ori_address, bin_address);
				int index = get_data(bin_address, index_from, index_to);*/
				tag_to = 23;
				tag = get_data(bin_address, tag_from, tag_to);
				//printf("%s -> -%s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);
				set_cache(index, tag);
			break;
		}
	}
	cache_print();
	printf("r_data_count:%d, w_data_count:%d\n", r_data_count, w_data_count);
	fclose(fp);
	return 0;
}
