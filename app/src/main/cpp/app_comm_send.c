#include "app_comm_send.h"
#include "data_struct.h"
#include "app_crc.h"
#include "data_pack.h"
 SEND_STRUCT  send_packet_struct;
 RINGBUF8  ble_packet_struct;        //



/*---------------------------------------
发送计算结果通信包
send_packet_struct结构体中的数姐包含了包头包序列包长度等完整的通信包数据

stepcnt  ： 累计步数值
speed    ： 速度值
----------------------------------------------*/
void PackingAccResult(uint16_t stepcnt, uint16_t speed)
{
	uint8_t result_data[10];
	uint8_t DATA_LEN,CRC_H,CRC_L;
	uint8_t i;
	send_packet_struct.packet_serial++;
	if(send_packet_struct.packet_serial>127) send_packet_struct.packet_serial=0;
	send_packet_struct.send_arry[2]=send_packet_struct.packet_serial;   //包序列
	send_packet_struct.send_arry[3]=0x05;                              //包长，原始长度
	send_packet_struct.send_arry[4]=0x01;                              //命令码，数据发送

	send_packet_struct.send_arry[7]=0x03;                               //模式，骑行模式

	result_data[0]=0x01;               //数据种类，计步数据
	result_data[1]=0x01;               //数据类型，计算结果
	result_data[2]=0x03;               //模式，骑行模式

	result_data[3]=stepcnt>>8;
	result_data[4]=stepcnt;
	result_data[5]=speed>>8;
	result_data[6]=speed;
	DATA_LEN=0X07;
	crc16_get(result_data, DATA_LEN, &CRC_H, &CRC_L);
	result_data[7]=CRC_H;
	result_data[8]=CRC_L;
	ARRAY_Insert_Mark(result_data,0x09, &send_packet_struct);  //插0处理
	DATA_LEN =send_packet_struct.packet_length;
	send_packet_struct.send_arry[3]=DATA_LEN;

	RINGBUF8_Fill(&ble_packet_struct,0x00);  //第一个字节为0
	for(i=0;i<19;i++)
	{
		RINGBUF8_Fill(&ble_packet_struct,send_packet_struct.send_arry[i]);
	}
}

/*-------------------------------------------------------
发送原始数据
每个通信包包含10个采样点的原始数据，每个点包括三个轴，每轴两字节
三轴数据格式：X1_H X1_L Y1_H Y1_L Z1_H Z1_L X2_H Z2_L……

包头  包序列  包长  命令码  计步数据   采样速率  量程   三轴数据   CRC16校验值
FAFA   XX     xx   01      xyz        05       01      xxxx      xxxxx

50Hz采样， 正负4g量程

axisnum ：数据类型， =0x02 XYZ三轴原始数据；
                    =0x03  X轴原始数据；
                    =0x04 Y轴,
                    =0X05 Z轴
                    =0x06 Y轴原始数据+计算结果
---------------------------------------------------------*/
void PackingAccRawData(RINGBUF8 *p_databuf,uint8_t axisnum)
{
	uint8_t result_data[160];
	uint8_t i,j,blepacket_cnt;
	uint8_t DATA_LEN,CRC_H,CRC_L;
	send_packet_struct.packet_serial++;
	if(send_packet_struct.packet_serial>127) send_packet_struct.packet_serial=0;
	send_packet_struct.send_arry[2]=send_packet_struct.packet_serial;   //包序列
	send_packet_struct.send_arry[3]=0x09;                              //包长，原始长度
	send_packet_struct.send_arry[4]=0x01;                              //命令码，数据发送

	result_data[0]=axisnum;            //数据种类，计步数据
	result_data[1]=0x02;               //数据类型，三个轴的原始数据
	result_data[2]=0x03;               //采样速度 50Hz
	result_data[3]=0x04;               //量程

	for(i=0;i<60;i++)
	{
		RINGBUF8_GetOneElement(p_databuf,&result_data[i+4]);    //取出66个字节数据
	}
	DATA_LEN=64;
	crc16_get(result_data, DATA_LEN, &CRC_H, &CRC_L);      //
	result_data[64]=CRC_H;
	result_data[65]=CRC_L;
	ARRAY_Insert_Mark(result_data,66, &send_packet_struct);  //插0处理

	DATA_LEN =send_packet_struct.packet_length;
	send_packet_struct.send_arry[3]=DATA_LEN;
	DATA_LEN=DATA_LEN+9;
	//计算拆分包数
	while((blepacket_cnt*20)<DATA_LEN)
	{
		blepacket_cnt++;
	}

	//拆包
	for(j=0;j<blepacket_cnt;j++)
	{
		RINGBUF8_Fill(&ble_packet_struct,j);  //第一个字节为0
		for(i=0;i<19;i++)
		{
			RINGBUF8_Fill(&ble_packet_struct,send_packet_struct.send_arry[i]);
		}
	}
	for(i=0;i<200;i++)
	{
		send_packet_struct.send_arry[i]=0;    //数组复位
	}

}






