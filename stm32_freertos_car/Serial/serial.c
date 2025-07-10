#include "stm32f4xx_hal.h"
#include "serial.h"   
#include <string.h> 
#include "usart.h"

unsigned char JudgeReceiveBuffer[reBiggestSize*2];//接受最大内存
unsigned char JudgeSend[SendBiggestSize];//发送最大内存

uint8_t num_buff[3];

//通信协议常量
const unsigned char header[2]  = {0x55, 0xaa};
const unsigned char ender[2]   = {0x0d, 0x0a};


unsigned char SaveBuffer[100];//接受双缓存区

Ctrl_information chassis_ctrl ={0,0,0,1,1}; //接受上位机控制信息

//定义发送数据（左轮速、右轮速、角度）共用体
sendData leftVelNow, rightVelNow, angleNow;

//定义左右轮速控制速度共用体
receiveData leftVelSet, rightVelSet;

void JudgeBuffReceive(unsigned char ReceiveBuffer[])
{
	short k=0;
	short PackPoint;
	memcpy(&SaveBuffer[reBiggestSize],&ReceiveBuffer[0],reBiggestSize);		
	//把ReceiveBuffer[0]地址拷贝到SaveBuffer[13], 依次拷贝13个, 把这一次接收的存到数组后方
	for(PackPoint=0;PackPoint<reBiggestSize;PackPoint++)	//先处理前半段数据(在上一周期已接收完成)
	{
		if(SaveBuffer[PackPoint]==header[0] && SaveBuffer[PackPoint + 1]== header[1]) //包头检测
		{	
			short dataLength  = SaveBuffer[PackPoint + 2]    ;
			unsigned char checkSum = getCrc8(&SaveBuffer[PackPoint], 3 + dataLength);
				// 检查信息校验值
			if (checkSum == SaveBuffer[PackPoint +3 + dataLength]) //SaveBuffer[PackPoint开始的校验位]
			{
				//说明数据核对成功，开始提取数据
				for(k = 0; k < 2; k++)
				{
					leftVelSet.data[k]  = SaveBuffer[PackPoint + k + 3]; //SaveBuffer[3]  SaveBuffer[4]
					rightVelSet.data[k] = SaveBuffer[PackPoint + k + 5]; //SaveBuffer[5]  SaveBuffer[6]
				}				
				//速度赋值操作
				chassis_ctrl.leftSpeedSet  = (int)leftVelSet.d;
				chassis_ctrl.rightSpeedSet = (int)rightVelSet.d;
				
				//ctrlFlag
				chassis_ctrl.crtlFlag = SaveBuffer[PackPoint + 7];     //SaveBuffer[7]
			}
		}	
	memcpy(&SaveBuffer[0],&SaveBuffer[reBiggestSize],reBiggestSize);		
	//把SaveBuffer[13]地址拷贝到SaveBuffer[0], 依次拷贝13个，把之前存到后面的数据提到前面，准备处理
	}
}

void usartSendData(short leftVel, short rightVel,short angle,unsigned char ctrlFlag)
{
	int i, length = 0;

	// 计算左右轮期望速度
	leftVelNow.d  = leftVel;
	rightVelNow.d = rightVel;
	angleNow.d    = angle;
	
	// 设置消息头
	for(i = 0; i < 2; i++)
	{
		JudgeSend[i] = header[i];             // buf[0] buf[1] 
	}
	// 设置机器人左右轮速度、角度
	length = 7;
	JudgeSend[2] = length;                 // buf[2]
	for(i = 0; i < 2; i++)
	{
		JudgeSend[i + 3] = leftVelNow.data[i];         // buf[3] buf[4]
		JudgeSend[i + 5] = rightVelNow.data[i];        // buf[5] buf[6]
		JudgeSend[i + 7] = angleNow.data[i];           // buf[7] buf[8]
	}
	// 预留控制指令
	JudgeSend[3 + length - 1] = ctrlFlag;              // buf[9]
	
	// 设置校验值、消息尾
	JudgeSend[3 + length] = getCrc8(JudgeSend, 3 + length);  // buf[10]
	JudgeSend[3 + length + 1] = ender[0];              // buf[11]
	JudgeSend[3 + length + 2] = ender[1];              // buf[12]
	HAL_UART_Transmit_DMA(&huart1, JudgeSend, 3 + length + 3);
}

unsigned char getCrc8(unsigned char *ptr, unsigned short len)
{
	unsigned char crc;
	unsigned char i;
	crc = 0;
	while(len--)
	{
		crc ^= *ptr++;
		for(i = 0; i < 8; i++)
		{
			if(crc&0x01)
			{
				crc=(crc>>1)^0x8C;
			}
      else
			{
				crc >>= 1;
			}          
		}
	}
	return crc;
}
