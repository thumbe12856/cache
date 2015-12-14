#include "common.h"

void get_hex_address(char buf[], char ori_address[])
{
	int i, j, swit=0;
	for(i=0, j=0; i<strlen(buf); i++)
	{
		if(buf[i]=='\n') swit=0;

		if(swit) ori_address[j++] = buf[i];

		if(buf[i]==' ') swit=1;
	}
	ori_address[j++] = '\0';
	return;
}

void hex_to_bin(char hex_data[], char bin_data[])
{
	char temp_addr[4];
	int i, j=23, k, ii, temp;
	for(i=0; i<6; i++)
	{
		if(hex_data[i] <='9' && '0'<=hex_data[i]) temp = hex_data[i] - '0';
		else temp = hex_data[i] - 'a' + 10;

		k=0;
		while(k!=4)
		{
			temp_addr[k++] = temp%2 + '0';
			temp/=2;
		}
		for(k=3; k>=0; k--) bin_data[j--] = temp_addr[k];
	}
	return;
}

int power_2(int data)
{
	int result = 0;
	while(data!=1)
	{
		data/=2;
		result++;
	}
	return result;
}

int get_data(char data[], int from, int to)
{
	int i, result, k=1;
	result = 0;
	for(i=from; i<=to; i++)
	{
		result += (data[i]-'0') * k;
		k*=2;
	}
	return result;
}