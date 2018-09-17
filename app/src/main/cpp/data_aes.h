#ifndef  _DATA_AES_H__
#define  _DATA_AES_H__




#include "aes/aes_crypt.h"



void ble_pack_encrypt( unsigned char* in, unsigned char* out);
void ble_pack_decrypt( uint8_t* rawdata,  uint8_t* decrypt_data);

void data_encrypt( unsigned char* in, unsigned char* out, const unsigned char* key, unsigned int length );
void data_decrypt( unsigned char* in, unsigned char* out, const unsigned char* key, unsigned int length );











#endif





