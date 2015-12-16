#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void initail_cache(int c_index, int ass, int index_size, int tag_size, char r_policy[]);
void set_cache(int index, int tag, int option);
void replace(int index, int tag, int option);
void mFIFO(int index, int tag, int option);
/*void read_cache(int index, int tag);
void write_cache(int index, int tag);*/
void cache_print();