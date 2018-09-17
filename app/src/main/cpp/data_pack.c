#include "data_pack.h"
#include "data_struct.h"
#include "data_aes.h"

 RXCMD_STRUCT  rxcmd_data_packet;     //引用main.c中的全局变量
 RINGBUF8 rx_data_rbuf;                //缓存蓝牙接收到的数据
// extern uint8_t data_array[20];
/*---------------------------------------------
p_rawdata : 解密后的原始数据

-----------------------------------------------*/
void GetDataPacket(void)
{
	uint8_t i;
	uint8_t datalen;     //一个蓝牙包中的数据长度

	uint8_t cmd_rawdata_ary[20];      //接收到的原始加密数据
	uint8_t cmd_decrypt_ary[20];      //解密后的数据
	if(rxcmd_data_packet.packet_head_fla )    //如果收到了包头
	{
		if(rxcmd_data_packet.packet_RxOK)
		{
			//旧的命令没处理完，放弃本次操作
		}
		else    //如果收到包头但是通信包没有接收完成，说明通信包不止一包，继续接收包
		{
			for(i=0;i<20;i++)
			{
				RINGBUF8_GetOneElement(&rx_data_rbuf,&cmd_rawdata_ary[i]);      //从缓存中取出20字节数据
//				data_array[i]=cmd_rawdata_ary[i];    //测试接收到的数据
			}
			ble_pack_decrypt(cmd_rawdata_ary,cmd_decrypt_ary);      //数据解密

			datalen=rxcmd_data_packet.packet_length - rxcmd_data_packet.data_ary_cnt;
			if(datalen>19)
			{
				datalen=19;      //如果剩下长度超过19字节，则本次仅接收19字节
			}
			for(i=0;i<datalen;i++)
			{
				rxcmd_data_packet.data_ary[rxcmd_data_packet.data_ary_cnt]=cmd_decrypt_ary[i+1];     //从byte1开始接收
				rxcmd_data_packet.data_ary_cnt++;
			}
			if(rxcmd_data_packet.data_ary_cnt >= (rxcmd_data_packet.packet_length))
			{
				Struct_Remove_Mark(&rxcmd_data_packet);
				rxcmd_data_packet.packet_RxOK=1;
			}
		}
	}
	else  //如果没有收到包头
	{
		for(i=0;i<20;i++)
		{
			RINGBUF8_GetOneElement(&rx_data_rbuf,&cmd_rawdata_ary[i]);      //从缓存中取出20字节数据
//			data_array[i]=cmd_rawdata_ary[i];    //测试接收到的数据
		}
		ble_pack_decrypt(cmd_rawdata_ary,cmd_decrypt_ary);      //数据解密

		if(0==cmd_decrypt_ary[0])    //如果收到蓝牙起始包，说明是包头
		{
			if(0xfa==cmd_decrypt_ary[1] && 0xfa==cmd_decrypt_ary[2])
			{
				rxcmd_data_packet.packet_head_fla = 1;   //收到包头，置高标志
				rxcmd_data_packet.packet_serial_num=cmd_decrypt_ary[3];   //包序列
				rxcmd_data_packet.packet_length =cmd_decrypt_ary[4];     //包长度
				rxcmd_data_packet.CMD =cmd_decrypt_ary[5];     //命令码
				rxcmd_data_packet.data_ary_cnt=0;      //数组下标复位

				//对包长进行判断
				datalen=rxcmd_data_packet.packet_length;   //如果包长小于等于15，则只用通信包只有一个蓝牙包
				if(rxcmd_data_packet.packet_length>14)
				{
					datalen = 14;   //如果通信包长 大于14,则通信包使用了不止一个蓝牙包，第1包中只有14个数据
				}

				for(i=0;i<datalen;i++)
				{
					rxcmd_data_packet.data_ary[rxcmd_data_packet.data_ary_cnt]=cmd_decrypt_ary[i+6];       //从byte6开始接收
					rxcmd_data_packet.data_ary_cnt++;
				}
				if(rxcmd_data_packet.data_ary_cnt>=(rxcmd_data_packet.packet_length))
				{
					Struct_Remove_Mark(&rxcmd_data_packet);
					rxcmd_data_packet.packet_RxOK=1;
				}
			}
		}
		else
		{
			//如果没有收到包头的时候收到中间包，则跳过
		}
	}
}

/*
**=================================================
**	接收数据去除插入符函数
**	原型：u8 Struct_Remove_Mark
数据长度为数据区+CRC校验区
**=================================================
*/
void Struct_Remove_Mark(RXCMD_STRUCT *source)
{
	uint8_t i,j;
	uint8_t ajust_array[160];
	uint8_t len;
	len = source->packet_length ;

	for(i=0;i<len;i++)
	{
		ajust_array[i]=source->data_ary[i];     //复制原始数据
	}

	//将调整好的数据填回结构体
	j=0;
	for(i=0;i<len;i++)
	{
		source->data_ary[j]=ajust_array[i];
		j++;
		if(0xFA==ajust_array[i])
		{
			i++;     //跳过0xfa后面的一个数据
		}
	}
	source->packet_length=j;     //更新数据长度

}

//在将要发送的数据中自动在0xaa和0xbb后插入0x00
/*
 **=================================================
**	发送数据插0函数
**	原型：
**	版本：V1(库函数版本)
**	修改日期：2018-07-05
**	参数说明： raw_arry：需要插0的数组
**	          arry_Len：插零的数据长度

**=================================================
*/
void ARRAY_Insert_Mark(uint8_t*raw_arry,uint8_t arry_Len, SEND_STRUCT *Tx_struct)
{
	uint8_t i,j ;

	j=5;   //开始写入的数组下标
	for(i=0;i<arry_Len;i++)
	{
		Tx_struct->send_arry[j] =	raw_arry[i];
		j++;
		if(0xFA==raw_arry[i])
		{
			Tx_struct->send_arry[j] =0x00;
			j++;
		}
	}
	Tx_struct->packet_length=j-5;    //更新长度   减掉包头包序列包长度值 命令码等的长度，只包括数据区和CRC的长度


}





