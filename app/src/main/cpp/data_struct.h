#ifndef  _DATA_STRUCT_H_
#define  _DATA_STRUCT_H_

#include <stdint.h>

typedef struct
{
	uint8_t  head_fla;
	uint8_t  tail_fla;
	uint8_t  *p_data;
	uint8_t data_len;
	uint8_t  data_cnt;
}UART_CMD_STRUCT;

//从设备参数
typedef struct
{
	uint8_t status;    //是否已经连接上，
	uint8_t connect;    //是否需要连接，  =1 连接，=0 不需要连接
	uint8_t disconnect;    //是否需要断开蓝牙连接，=1 需要断开蓝牙连接
	uint8_t isfull;    // =1，有计步器地址，=0 地址为空，可以写入计步器地址
	uint8_t run;       // 是否运行，
	uint8_t runmode;
	uint8_t scale;     //是否需要扫描并发送扫描结果
	uint8_t mac[6];   //mac地址
	int8_t  rssi;     //信号强度
	uint16_t handle;
//	uint8_t reconnect;  //是否重连接

}slave_struct;


//从设备参数
typedef struct
{
	uint8_t status;    //是否已经连接上，
	uint8_t connect;    //是否需要连接，  =1 连接，=0 不需要连接
	uint8_t disconnect;    //是否需要断开蓝牙连接，=1 需要断开蓝牙连接
	uint8_t isfull;    // =1，有计步器地址，=0 地址为空，可以写入计步器地址
	uint8_t run;       // 是否运行，
	uint8_t runmode;
	uint8_t scale;     //是否需要扫描并发送扫描结果
	uint8_t mac[6];   //mac地址
	int8_t  rssi;     //信号强度
	uint16_t handle;
	uint8_t conn_index;   //在连接数组中的下标
	uint8_t timeout;    //扫描时间 超时则删除一个设备

}dev_scale_struct;      //设备扫描结构体

//设备信息结构体定义
typedef struct
{
	unsigned char device_version_sw[4];     //设备软件版本号
	unsigned char device_version_hw[4];     //设备电路版本号
	unsigned char device_serial_num[4];     //生产序列号

}DEV_INF_STRUCT;

typedef struct
{
	uint8_t DRIVE_RATIO;    //传动比，命令收发的时候传动比乘以10，收到后除以10，以保证一位小数的情况下传输整型数据
	uint8_t oparation_mode;  //工作模式，0x00=测试模式（不管有无传感器，秒发一包自增数据），01=三轴模式，02=霍尔模式
	uint8_t send_mode;       //发送模式，发送计步结果/发送三轴原始数据/发送X/Y/Z轴原始数据
	uint8_t acc_speed_mode;  //三轴采样速度
	uint8_t acc_scale_mode;  //三轴量程


}DEVICE_PARA;
//存uint8的环形数组结构体
typedef struct
{
	uint8_t* databuf;  //指向队列数组的指针
	uint16_t length;    //数组长度
	uint16_t head;     // 队列头下标
	uint16_t tail;     //队列尾下标
	uint16_t fill_cnt; //队列计数

}RINGBUF8 ;			//存uint8_t型的环形队列


//接收数据结构体
typedef struct
{
	uint8_t packet_RxOK;         //接收完成标志
	uint8_t packet_head_fla;     //通信包头标志，
	uint8_t packet_serial_num;   //包序列
	uint8_t packet_length;       //包长度
	uint8_t CMD;                 //命令码
	uint8_t data_ary[140];         //存放数据区和CRC16校验数据
	uint8_t data_ary_cnt;        //接收记数
	uint8_t  CRC16_fla;         //CRC校验结果，真/假

}RXCMD_STRUCT;

//发送数据结构体
typedef struct
{
	uint8_t packet_ready;         //可以发送标志
	uint8_t packet_Txbussy;         //发送完成标志，如果包长大于19字节，分开多个蓝牙包发送时，会存在可以发送，但没有发送完成的情况
	uint8_t packet_serial;	    //包序列
	uint8_t packet_length;       //包长度，仅包括数据区和校验码的长度，总的包长值还要再加5
	uint8_t CMD;                 //命令码
	uint8_t send_arry[200];      //通信包，包含包头、包序列、包长度、命令码、数据区和CRC校验码
	uint16_t  CRC16_value;         //CRC校验值
} SEND_STRUCT;

typedef struct
{
	uint8_t send_mode;        //  模式，发送计步值，原始数据等
}DEVICE_MODE;
//
typedef struct
{
	uint8_t samplerate;       //  采样率
	uint8_t scale;            //  量程
}LIS_CONFIG_STRUCT;


typedef struct
{
	uint8_t length;       //
	uint8_t fillcnt;      //填充计数
	uint8_t *databuf;     //
}LIS_BUF_STRUCT;

void uart_cmd_Init(UART_CMD_STRUCT * cmd_struct, uint8_t * data_array,uint8_t len);
uint8_t Ringbuf8_Init(RINGBUF8 *RB,uint8_t array[], uint16_t len );
uint8_t RINGBUF8_GetOneElement(RINGBUF8 *rbuf,uint8_t *p_data);
uint8_t RINGBUF8_Fill(RINGBUF8 *rbuf,uint8_t data);
uint16_t RINGBUF8_GetBufLength( RINGBUF8 *rbuf );
uint8_t RINGBUF8_CleanBuf( RINGBUF8 *rbuf );

void RXCMD_Init(RXCMD_STRUCT *p_rxcmd_aa);
void SendStruct_Init(SEND_STRUCT *p_sendsruct_aa);
//void Device_Para_Init(DEVICE_PARA *dvicepara);

#endif






