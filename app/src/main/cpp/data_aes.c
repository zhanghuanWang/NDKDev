#include "data_aes.h"

//加密密钥
const unsigned char encrypt_key_1[16] = {'K','M','1','9','3','0','P','e','d','o','m','e','t','e','r','!'};   //第一次加密密钥
const unsigned char encrypt_key_2[16] = {'L','X','Y','G','W','Q','F',',','Q','S','G','C','T','Y','S','!'};   //第二次加密密钥
//两次加密密钥



//蓝牙包加密
void ble_pack_encrypt( unsigned char* in, unsigned char* out)
{

	unsigned char i = 0;
	unsigned char mid_encrypt[35] = {0};
	unsigned char out_encrypt[35] = {0};

	data_encrypt(in, mid_encrypt, encrypt_key_1, 16);							//加密前16字节
	mid_encrypt[16] = in[16];													//拷贝后4字节
	mid_encrypt[17] = in[17];
	mid_encrypt[18] = in[18];
	mid_encrypt[19] = in[19];

	data_encrypt(mid_encrypt+4, out_encrypt+4, encrypt_key_2, 16);				//加密后16字节
	out_encrypt[0] = mid_encrypt[0];											//拷贝前4字节
	out_encrypt[1] = mid_encrypt[1];
	out_encrypt[2] = mid_encrypt[2];
	out_encrypt[3] = mid_encrypt[3];

	for(i=0; i<20; i++)
	{
		out[i] = out_encrypt[i];
	}
}

//蓝牙包解密
void ble_pack_decrypt( uint8_t* rawdata,  uint8_t* decrypt_data)
{

	unsigned char i = 0;
	unsigned char mid_decrypt[35] = {0};
	unsigned char out_decrypt[35] = {0};

	data_decrypt(rawdata+4, mid_decrypt+4, encrypt_key_2, 16);						//解密后16字节
	mid_decrypt[0] = rawdata[0];														//拷贝前4字节
	mid_decrypt[1] = rawdata[1];
	mid_decrypt[2] = rawdata[2];
	mid_decrypt[3] = rawdata[3];

	data_decrypt(mid_decrypt, out_decrypt, encrypt_key_1, 16);					//解密前16字节
	out_decrypt[16] = mid_decrypt[16];											//拷贝后4字节
	out_decrypt[17] = mid_decrypt[17];
	out_decrypt[18] = mid_decrypt[18];
	out_decrypt[19] = mid_decrypt[19];

	for(i=0; i<20; i++)
	{
		decrypt_data[i] = out_decrypt[i];
	}
}


//16字节数据加密函数
/*
参数说明
in：  需要加密的数据数组
out： 存放加密后的数据数组
key： 加密密钥
length：加密长度

*/
void data_encrypt( unsigned char* in, unsigned char* out, const unsigned char* key, unsigned int length )
{
	AES_CTX c;
	int out_len = 0;
	int offset = 0;

	aes_cbc_encrypt_init(&c, key);												//密钥连接到结构

	while(length > 16)															//以16字节为单位，分包加密
	{
		aes_cbc_encrypt_update(&c, in+offset, out+offset);
		offset += 16;
		length -= 16;
	}

	aes_cbc_encrypt_final(&c, in+offset, length, out+offset, &out_len);
}

//数据解密
void data_decrypt( unsigned char* in, unsigned char* out, const unsigned char* key, unsigned int length )
{
	AES_CTX c;
	int out_len = 0;
	int offset = 0;

	aes_cbc_decrypt_init(&c, key);												//密钥连接到结构

	while(length > 16)															//以16字节为单位，分包解密
	{
		aes_cbc_decrypt_update(&c, in+offset, out+offset);
		offset += 16;
		length -= 16;
	}

	aes_cbc_decrypt_final(&c, in+offset, out+offset, &out_len);
}




