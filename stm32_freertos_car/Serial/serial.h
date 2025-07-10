#ifndef	__SERIAL_H
#define	__SERIAL_H

#define reBiggestSize 13
#define SendBiggestSize  13

//左右轮速共用体
typedef union 
{
	short d;
	unsigned char data[2];
}sendData;

typedef union
{
	short d;
	unsigned char data[2];
}receiveData;

//控制车体结构体
typedef struct{
	int leftSpeedSet;
	int rightSpeedSet;
	unsigned char crtlFlag;
	char left_move;					//为1时方向为正
	char right_move;
}Ctrl_information;
	
//封装数据，将数据发送给linux
void usartSendData(short leftVel, short rightVel,short angle,unsigned char ctrlFlag); 
void JudgeBuffReceive(unsigned char ReceiveBuffer[]);

//计算八位循环冗余校验，得到校验值，一定程度上验证数据的正确性
unsigned char getCrc8(unsigned char *ptr, unsigned short len); 

#endif
