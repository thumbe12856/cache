#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void initail_cache(int c_index, int ass, int index_size, int tag_size, char r_policy[], int b_size);
void set_cache(int index, int tag, int option);
void replace(int index, int tag, int option);
void which_cache_to_replace(int index, int tag, int ass, int option);
void mFIFO(int index, int tag, int option);
void hLRU(int index, int tag);
void mLRU(int index, int tag, int option);
void hLFU(int index, int tag);
void mLFU(int index, int tag, int option);
void cache_print(int demand_count, int r_data_count, int w_data_count);