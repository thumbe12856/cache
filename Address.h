class Address
{                                                                                                                                               
public:                                                                                                                                         
    Address(char ori_address[], int block_size, int cache_size, int associative);
    ~Address();
    int get_index();
    int get_tag();
   
private:
    char hex_address[6];
    char bin_address[24];
    int index, tag;
};