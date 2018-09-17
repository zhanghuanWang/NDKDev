#ifndef  _DATA_PACK_H__
#define   _DATA_PACK_H__

//#include "includes.h"

#include "data_struct.h"



//命令码宏定义  byte4数据
#define CMD_SEND         0x01         // byte4=0x01 数据传输，用于dev向rec发送数据
#define CMD_UPDATE       0x02         //固件升级
#define CMD_INFORMTION   0x03
#define CMD_CONTROL      0x04         // byte4=0x04 控制命令
#define CMD_TESTMODE     0xF1         // byte4=0x04 控制命令













void GetDataPacket(void);

void Struct_Remove_Mark(RXCMD_STRUCT *source);
void ARRAY_Insert_Mark(uint8_t*raw_arry,uint8_t arry_Len, SEND_STRUCT *Tx_struct);









#endif





