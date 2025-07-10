#ifndef	__SERIAL_H
#define	__SERIAL_H

#define reBiggestSize 13
#define SendBiggestSize  13

//�������ٹ�����
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

//���Ƴ���ṹ��
typedef struct{
	int leftSpeedSet;
	int rightSpeedSet;
	unsigned char crtlFlag;
	char left_move;					//Ϊ1ʱ����Ϊ��
	char right_move;
}Ctrl_information;
	
//��װ���ݣ������ݷ��͸�linux
void usartSendData(short leftVel, short rightVel,short angle,unsigned char ctrlFlag); 
void JudgeBuffReceive(unsigned char ReceiveBuffer[]);

//�����λѭ������У�飬�õ�У��ֵ��һ���̶�����֤���ݵ���ȷ��
unsigned char getCrc8(unsigned char *ptr, unsigned short len); 

#endif
