#ifndef  _APP_COMM_SEND_H__
#define _APP_COMM_SEND_H__


//#include "includes.h"
#include "data_struct.h"








void PackingAccResult(uint16_t stepcnt, uint16_t speed);
void PackingAccRawData(RINGBUF8 *p_databuf,uint8_t axisnum);
//void PackingCalRawData(RINGBUF8 *p_databuf,uint8_t axisnum);







#endif




