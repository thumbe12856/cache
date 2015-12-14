#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int main(int argc, char* argv[])
{
	int cache_size = atoi(argv[1])*1024;
	int block_size = atoi(argv[2]);
	int associative = atoi(argv[3]);
	int offset = power_2(block_size);

	int index_to = power_2((cache_size/block_size)/associative) + offset -1;
	int index_from = offset;
	int tag_to = 23;
	int tag_from = index_to + 1;

	FILE *fp;
	fp = fopen("temp.din", "r");
	char buf[100];
	char ori_address[6];
	char bin_address[24];
	while(fgets(buf, 100, fp))
	{
		switch(buf[0])
		{
			case '2':
				get_hex_address(buf, ori_address);
				hex_to_bin(ori_address, bin_address);
				int index = get_data(bin_address, index_from, index_to);
				int tag = get_data(bin_address, tag_from, tag_to);
				printf("%s -> -%s\nindex:%d, tag:%d\n", ori_address, bin_address, index, tag);
			break;
		}
	}
	fclose(fp);
	/*char ori_address[] = "408ed4";
	

	hex_to_bin(ori_address, bin_address);

	int index_to = power_2((cache_size/block_size)/associative) + offset -1;
	int index_from = offset;
	int index = get_data(bin_address, index_from, index_to);

	int tag_to = 23;
	int tag_from = index_to + 1;
	int tag = get_data(bin_address, tag_from, tag_to);*/
	return 0;
}
