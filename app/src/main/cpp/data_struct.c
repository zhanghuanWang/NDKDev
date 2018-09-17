/************************************************



*****************************************************/

#include "data_struct.h"
//#include "driver_lis3dsh.h"


void uart_cmd_Init(UART_CMD_STRUCT *cmd_struct, uint8_t * data_array,uint8_t len)
{
	cmd_struct->data_cnt=0;
	cmd_struct->head_fla=0;
	cmd_struct->tail_fla=0;
	cmd_struct->p_data=data_array;
	cmd_struct->data_len=len;
}

/*-----------------------------------------------
环形数组初始化
缓存数组：    队列头……先存放的数据……后存放的数据……队列尾

--------------------------------------------------*/
uint8_t Ringbuf8_Init(RINGBUF8 *RB, uint8_t array[], uint16_t len )
{
	RB->databuf=array;   //缓存数组
	RB->length=len;      //数组总长度
	RB->fill_cnt=0;      //缓存记数
	RB->head=0;          //缓存队列头
	RB->tail=0;          //缓存数据尾

	return 0;
}
/*----------------------------------------------
//获取一个unsigned short型环形队列的元素存放到elem所指的地址中去
从环形数组q中取出一个数据存elem中
---------------------------------------------*/
uint8_t RINGBUF8_GetOneElement(RINGBUF8 *rbuf,uint8_t *p_data)
{
	if(rbuf->head!=rbuf->tail)	//如果队列为非空
	{
		*p_data=rbuf->databuf[rbuf->head];   //取出一个元素
		rbuf->head=(rbuf->head+1)%rbuf->length;  // 队列上标加1，如果超界，则从头开始

		return 1;
	}
	else
	{
		return 0;
	}
}

/*----------------------------------------------

添加元素

---------------------------------------------------*/
uint8_t RINGBUF8_Fill(RINGBUF8 *rbuf,uint8_t data)
{
	uint8_t aa;
	if((rbuf->tail+ 1)%(rbuf->length )!= rbuf->head)  //队列未满，同时考虑了head<tail，和head>tail的这两种情况。
	{
		rbuf->databuf[rbuf->tail]=data;         //队列尾增加一个元素
		rbuf->tail=(rbuf->tail+1)%(rbuf->length); //尾下标加1，如果下标超出数组界限，则指向开始。
		//队列未满
		return 1;
	}
	else
	{

		RINGBUF8_GetOneElement(rbuf,&aa);   //如果队列满，则先删去一个
		rbuf->databuf[rbuf->tail]=data;         //队列尾增加一个元素
		rbuf->tail=(rbuf->tail+1)%(rbuf->length); //尾下标加1，如果下标超出数组界限，则指向开始。

		return 0;
	}

}

//获取unsigned short型环形队列长度
/*---------------*/
uint16_t RINGBUF8_GetBufLength( RINGBUF8 *rbuf )
{
	uint16_t buf_len;
	buf_len=0;
	buf_len=(rbuf->tail+rbuf->length - rbuf->head) % rbuf->length;
	return buf_len;

}

//获取unsigned short型环形队列长度
/*---------------*/
uint8_t RINGBUF8_CleanBuf( RINGBUF8 *rbuf )
{
	rbuf->fill_cnt=0;      //缓存记数
	rbuf->head=0;          //缓存队列头
	rbuf->tail=0;          //缓存数据尾

	return 0;
}


void RXCMD_Init(RXCMD_STRUCT *p_rxcmd_aa)
{
	p_rxcmd_aa->packet_head_fla =0x00;
	p_rxcmd_aa->data_ary_cnt=0x00;
	p_rxcmd_aa->packet_length =0x00;
	p_rxcmd_aa->packet_RxOK=0x00;
	p_rxcmd_aa->packet_serial_num =0;
}


void SendStruct_Init(SEND_STRUCT *p_sendsruct_aa)
{
	uint8_t i;

	for(i=0;i<200;i++)
	{
		p_sendsruct_aa->send_arry[i]=0x00;
	}
	p_sendsruct_aa->packet_ready=0;
	p_sendsruct_aa->packet_Txbussy=0;
	p_sendsruct_aa->send_arry[0]=0xFA;   //包头
	p_sendsruct_aa->send_arry[1]=0xFA;
	p_sendsruct_aa->packet_serial=0x00;
	p_sendsruct_aa->packet_length=0x00;
	p_sendsruct_aa->CMD =0x00;
	p_sendsruct_aa->CRC16_value = 0x00;


}

///*---------------------------------------------------
//初始化设备参数，后续需要从flash中读取保存的信息

//------------------------------------------------------*/
//void Device_Para_Init(DEVICE_PARA *dvicepara)
//{
//	dvicepara->DRIVE_RATIO=0x01;
//	dvicepara->acc_scale_mode=0x04;
//	dvicepara->acc_speed_mode=LIS3DSH_SEEPD_MODE_50HZ;
//	dvicepara->oparation_mode=0x01;
//	dvicepara->send_mode= 0x01;
//	
//}


