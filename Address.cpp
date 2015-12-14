#include <iostream>
#include <cstdlib>
#include "Address.h"
#include "common.h"
using namespace std; 

Address::Address(char ori_address[], int block_size, int cache_size, int associative)
{
	hex_to_bin(ori_address, bin_address);
	int block_size_byte_power2 = power_2(block_size);

	int index_to = power_2((cache_size/block_size)/associative) + block_size_byte_power2 -1;
	int index_from = block_size_byte_power2;
	index = get_data(bin_address, index_from, index_to);

	int tag_to = 23;
	int tag_from = index_to + 1;
	tag = get_data(bin_address, tag_from, tag_to);

	//cout << "index:" << index << ", tag:" << tag;
	//cout << "index_to:" <<index_to << ", index_from:" << index_from << ", tag_to:" << tag_to << ", tag_from:" << tag_from <<endl;

	return;
}

Address::~Address()
{
	return;
}

int Address::get_index()
{
	return index;
}

int Address::get_tag()
{
	return tag;
}