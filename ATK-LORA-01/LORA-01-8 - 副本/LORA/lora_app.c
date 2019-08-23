#include "lora_app.h"
#include "lora_ui.h"
#include "lora_cfg.h"
#include "usart2.h"
#include "string.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "text.h"
#include "key.h"



#define GN_default 10
#define YL_default 3
#define RD_default 10



//********************************************************************************
//ATK-LORA-01模块功能驱动		  
//********************************************************************************


//设备参数初始化(具体设备参数见lora_cfg.h定义)
_LoRa_CFG LoRa_CFG=
{
	.addr = LORA_ADDR,       //设备地址
	.power = LORA_POWER,     //发射功率
	.chn = LORA_CHN,         //信道
	.wlrate = LORA_RATE,     //空中速率
	.wltime = LORA_WLTIME,   //睡眠时间
	.mode = LORA_MODE,       //工作模式
	.mode_sta = LORA_STA,    //发送状态
	.bps = LORA_TTLBPS ,     //波特率设置
	.parity = LORA_TTLPAR    //校验位设置
};


//全局参数
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

//设备工作模式(用于记录设备状态)
u8 Lora_mode=0;// 0:配置模式 1:接收模式 2:发送模式

//记录中断状态
static u8 Int_mode=0;//0：关闭 1:上升沿 2:下降沿

//AUX中断设置
//mode:配置的模式 0:关闭 1:上升沿 2:下降沿
void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//关闭
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿
	  else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
		
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//记录中断模式
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
    
}

//LORA_AUX中断服务函数
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//上升沿(发送:开始发送数据 接收:数据开始输出)     
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART2_RX_STA=0;//数据计数清0
		  }
		  Int_mode=2;//设置下降沿
		  LED0=0;//DS0亮
	   }
       else if(Int_mode==2)//下降沿(发送:数据已发送完 接收:数据输出结束)	
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART2_RX_STA|=1<<15;//数据计数标记完成
		  }else if(Lora_mode==2)//发送模式(串口数据发送完毕)
		  {
			 Lora_mode=1;//进入接收模式
		  }
		  Int_mode=1;//设置上升沿
          LED0=1;//DS0灭		   
	   }
       Aux_Int(Int_mode);//重新设置中断边沿
	   EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位  
	}	
}

//LoRa模块初始化
//返回值:0,检测成功
//       1,检测失败
u8 LoRa_Init(void)
{
	 u8 retry=0;
	 u8 temp=1;
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LORA_MD0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //下拉输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //中断线关闭
  	 EXTI_Init(&EXTI_InitStructure);//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2， 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//子优先级3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;		   //关闭外部中断通道
   	 NVIC_Init(&NVIC_InitStructure); 
	 
	 LORA_MD0=0;
	 LORA_AUX=0;
	
	 while(LORA_AUX)//确保LORA模块在空闲状态下(LORA_AUX=0)
	 {
		 Show_Str(40+30,50+20,200,16,"Module is busy. Wait!!",16,0); 	
		 delay_ms(500);
		 Show_Str(40+30,50+20,200,16,"                    ",16,0);
         delay_ms(100);		 
	 }
	 usart2_init(115200);	         //初始化串口2 
	 
	 LORA_MD0=1;//进入AT模式
	 delay_ms(40);
	 retry=3;
	 while(retry--)
	 {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//检测成功
			 USART2_RX_STA=0;
			 break;
		 }	
	 }
	 if(retry==0) temp=1;//检测失败
	 return temp;
}


//Lora模块参数配置
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	usart2_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//进入配置模式前设置通信波特率和校验位(115200 8位数据 1位停止 无数据校验）
	usart2_rx(1);//开启串口3接收
	
	while(LORA_AUX);//等待模块空闲
	LORA_MD0=1; //进入配置模式
	delay_ms(40);
	Lora_mode=0;//标记"配置模式"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	lora_send_cmd("AT","OK",70);
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//设置设备地址
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//设置信道和空中速率
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//设置发射功率
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//设置工作模式
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//设置发送状态
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//设置睡眠时间
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//设置串口波特率、数据校验位
	lora_send_cmd(sendbuf,"OK",100);

	LORA_MD0=0;//退出配置,进入通信
	delay_ms(40);
	while(LORA_AUX);//判断是否空闲(模块会重新配置参数)
	USART2_RX_STA=0;
	Lora_mode=1;//标记"接收模式"
	usart2_set(LoRa_CFG.bps,LoRa_CFG.parity);//返回通信,更新通信串口配置(波特率、数据校验位)
	Aux_Int(1);//设置LORA_AUX上升沿中断	
}

u8 Dire_Date[]={0x11,0x22,0x33,0x44,0x55};//定向传输数据
u8 date[30]={0};//定向数组
u8 Tran_Data[30]={0};//透传数组

#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
extern u32 obj_addr;//记录用户输入目标地址
extern u8 obj_chn;//记录用户输入目标信道

u8 wlcd_buff[10]={0}; //LCD显示字符串缓冲区
u8 rlcd_buff[10]={0}; //LCD显示字符串缓冲区




u8 GN_time = GN_default;
u8 YL_time = YL_default;
u8 RD_time = RD_default;
u8 GN_YL = GN_default + YL_default;
u8 GN_YL_RD = GN_default + YL_default + RD_default;
u16 time = 0;		// 也可设为u16或u32类型，根据时长需求255,65536,42亿9千万
u8 *ptr;
u8 str[10] = {0};
u8 gn = 0;
u8 yl = 0;
u8 rd = 0;
u8 str1[10] = {0};
u8 str2[10] = {0};
u8 str3[10] = {0};
u8 str4[10] = {0};




// 整数转字符串
void Int_Str(u16 num,u8* str)//10进制 
{
	//int i = 0;//指示填充str 
	//转换 
//	do{
//		str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
//		num /= 10;//去掉最低位	
//	}while(num);//num不为0继续循环
//	str[i] = 's';
//	str[++i] = '\0';
	
	str[0] = '\0';
	str[1] = 's';
	int i = 2;
	do{
		str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
		num /= 10;//去掉最低位	
	}while(num);//num不为0继续循环
	int j = 0;
	//对称交换 
	for(;j<i/2;j++)
	{
		//对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
		str[j] = str[j] + str[i-1-j];
		str[i-1-j] = str[j] - str[i-1-j];
		str[j] = str[j] - str[i-1-j];
	}
}


//Lora模块接收数据
void LoRa_ReceData(void)
{
	u16 i=0;
  u16 len=0;
   
	POINT_COLOR=BLUE;
	//有数据来了
	if(USART2_RX_STA&0x8000)
	{
		len = USART2_RX_STA&0X7FFF;
		USART2_RX_BUF[len]=0;//添加结束符
		USART2_RX_STA=0;

		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
			USART_SendData(USART1,USART2_RX_BUF[i]); 	
		}
    LCD_Fill(10,245,240,265,WHITE);
if(len < 10){		
		if(LoRa_CFG.mode_sta==LORA_STA_Tran)//透明传输
		{	
			Show_Str_Mid(10,245,USART2_RX_BUF,16,240);//显示接收到的数据


			
			
// 切分“：”前字符串的并转为整数，确定绿灯时间
ptr = (u8*)strtok((char*)USART2_RX_BUF, ":\\r\\n");
GN_time = atoi((char*)ptr);
// 切分“：”后字符串的并转为整数，确定红灯时间
ptr = (u8*)strtok(NULL, ":\\r\\n");
RD_time = atoi((char*)ptr);	

GN_YL = GN_time + YL_time;
GN_YL_RD = GN_time + YL_time + RD_time;
			
// 清除，归零，熄灭所有灯，重新开始
time = 0;
PCout(0) = 1;		
PCout(2) = 1;
PCout(3) = 1;
strcpy((char*)str1, "0s");
strcpy((char*)str2, "0s");
strcpy((char*)str3, "0s");		



			
		}else if(LoRa_CFG.mode_sta==LORA_STA_Dire)//定向传输
		{
			//将十六进制的数据转化为字符串打印在lcd_buff数组
			sprintf((char*)rlcd_buff,"%x %x %x %x %x",
			USART2_RX_BUF[0],USART2_RX_BUF[1],USART2_RX_BUF[2],USART2_RX_BUF[3],USART2_RX_BUF[4]);
				
			Show_Str_Mid(10,245,rlcd_buff,16,240);//显示接收到的数据	
		}
		memset((char*)USART2_RX_BUF,0x00,len);//串口接收缓冲区清0
}
	}
}

//Lora模块发送数据
void LoRa_SendData(void)
{      
	 static u8 num=0;
   u16 addr;
	 u8 chn;
	 u16 i=0;
	
	POINT_COLOR=BLUE;
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//透明传输
	{
		strcpy((char*)str4, (const char*)str1);
		strcat((char*)str4, ":");
		strcat((char*)str4, (const char*)str2);
		strcat((char*)str4, ":");
		strcat((char*)str4, (const char*)str3);
		//strcpy((char*)Tran_Data, (const char*)str4);
		sprintf((char*)Tran_Data, (const char*)str4, num);
		//sprintf((char*)Tran_Data,"LORA_0 TEST %d",num);
		u2_printf("%s\r\n",Tran_Data);
		LCD_Fill(0,290,240,310,WHITE); //清除显示
		Show_Str_Mid(10,290,Tran_Data,16,240);//显示发送的数据
		
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//定向传输
	{		
		addr = (u16)obj_addr;//目标地址
		chn = obj_chn;//目标信道
		
		date[i++] =(addr>>8)&0xff;//高位地址
		date[i++] = addr&0xff;//低位地址
		date[i] = chn;  //无线信道
		
		for(i=0;i<Dire_DateLen;i++)//数据写到发送BUFF
		{
			date[3+i] = Dire_Date[i];
		}
		for(i=0;i<(Dire_DateLen+3);i++)
		{
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
			USART_SendData(USART2,date[i]); 
		}	
		
    //将十六进制的数据转化为字符串打印在lcd_buff数组
		sprintf((char*)wlcd_buff,"%x %x %x %x %x %x %x %x",
		date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7]);
		
		LCD_Fill(0,290,240,310,WHITE);//清除显示
		Show_Str_Mid(10,290,wlcd_buff,16,240);//显示发送的数据
		
	  Dire_Date[4]++;//Dire_Date[4]数据更新
	}	
}

//接收和发送处理
void LoRa_Process(void)
{
	u8 key=0;
	static u8 t=0;

 DATA:
	Process_ui();//界面显示
	LoRa_Set();//LoRa配置(进入配置需设置串口波特率为115200) 
	while(1)
	{
		
		key = KEY_Scan(0);
		
		if(key==KEY0_PRES)
		{
			if(LoRa_CFG.mode_sta==LORA_STA_Dire)//若是定向传输,则进入配置目标地址和信道界面
			{
				usart2_rx(0);//关闭串口接收
				Aux_Int(0);//关闭中断
				Dire_Set();//进入设置目标地址和信道
				goto DATA;
			}
		}else if(key==WKUP_PRES)//返回主菜单页面
		{
			usart2_rx(0);//关闭串口接收
			Aux_Int(0);//关闭中断
			break;
		}
		else if(key==KEY1_PRES)//发送数据
		{
			  if(!LORA_AUX)//若空闲
			  {
				  Lora_mode=2;//标记"发送状态"
					//发送数据  
				  LoRa_SendData();  
			  }
		}
			
		//数据接收
		LoRa_ReceData();
		

		
		
		
if(time == 0){
	POINT_COLOR=GREEN;
	Int_Str(GN_time, str);
	Show_Str(100,100,200,24,"    ",24,0);
	Show_Str(100,100,200,24,str,24,0);			
	POINT_COLOR=YELLOW;
	Int_Str(YL_time, str);
	Show_Str(100,140,200,24,"    ",24,0);
	Show_Str(100,140,200,24,str,24,0);
	POINT_COLOR=RED;
	Int_Str(RD_time, str);
	Show_Str(100,180,200,24,"    ",24,0);
	Show_Str(100,180,200,24,str,24,0);
	// 下面这段代码不加的话会导致绿灯少1秒
	PCout(0) = 0;
	gn = GN_time - time / 20;
	POINT_COLOR=GREEN;
	Int_Str(gn, str);
	Show_Str(160,180,200,24,"    ",24,0);
	Show_Str(160,100,200,24,"    ",24,0);
	Show_Str(160,100,200,24,str,24,0);
	strcpy((char*)str1, "0s");
  strcpy((char*)str2, "0s");
  strcpy((char*)str3, "0s");
	strcpy((char*)str1, (const char*)str);
}
time++;
//sprintf((char*)str, "%s", time);
//Int_String(time, str);
//if(time == 1){
if(time >= 1 && time % 20 == 0 && time < GN_time*20){	// 进入次数太多会出现延时现象（语句执行也需要时间），所以加个判定再进入
	PCout(0) = 0;
	gn = GN_time - time / 20;
	POINT_COLOR=GREEN;
	Int_Str(gn, str);
	Show_Str(160,100,200,24,"    ",24,0);
	Show_Str(160,100,200,24,str,24,0);
	strcpy((char*)str1, (const char*)str);
}
//else if(time == GN_time*20){
else if(time >= GN_time*20 && time % 20 == 0 && time < GN_YL*20){
	strcpy((char*)str1, "0s");
	PCout(0) = 1;
	PCout(2) = 0;
	yl = YL_time - (time - GN_time*20) / 20;
	POINT_COLOR=YELLOW;
	Int_Str(yl, str);
	if(time == GN_time*20){	// 进入次数太多会出现闪屏现象，所以加个if语句
		Show_Str(160,100,200,24,"    ",24,0);
	}
	Show_Str(160,140,200,24,"    ",24,0);
	Show_Str(160,140,200,24,str,24,0);
	strcpy((char*)str2, (const char*)str);
}
//else if(time == GN_YL*20){
else if(time >= GN_YL*20 && time % 20 == 0 && time < GN_YL_RD*20){
	strcpy((char*)str2, "0s");
	PCout(2) = 1;
	PCout(3) = 0;
	rd = RD_time - (time - GN_YL*20) / 20;
	POINT_COLOR=RED;
	Int_Str(rd, str);
	if(time == GN_YL*20){
		Show_Str(160,140,200,24,"    ",24,0);
	}
	Show_Str(160,180,200,24,"    ",24,0);
	Show_Str(160,180,200,24,str,24,0);
	strcpy((char*)str3, (const char*)str);
}
else if(time == GN_YL_RD*20){
	strcpy((char*)str3, "0s");
	PCout(3) = 1;
	time = 0;
}		
		
	


		
		t++;
		if(t==10)
		{
			t=0;
			LED1=~LED1;	
		}			
		delay_ms(50);		
   }
	
}


//主测试函数
void Lora_Test(void)
{
	u8 t=0;
//	u8 key=0;
//	u8 netpro=0;
	// 进入主界面,检测到模块则通过
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
  Show_Str_Mid(0,40,"ATK-LORA-01",24,240); 
	Show_Str_Mid(0,80,"_^o^_",16,240);
	while(LoRa_Init())//初始化ATK-LORA-01模块
	{
		Show_Str(30,120+20,200,24,"Module Not OK !!",24,0); 	
		delay_ms(500);
		Show_Str(30,120+20,200,24,"                ",24,0);
	}
	Show_Str(20+30,120+20,200,24,"Module OK !!",24,0);
	delay_ms(1000);
	
	//菜单显示
	Menu_ui();
	
	while(1)
	{		
//		key = KEY_Scan(0);
//		if(key)
//		{
//			Show_Str(30+10,95+45+netpro*25,200,16,"  ",16,0);//清空之前的显示
//			
//			 if(key==KEY0_PRES)//KEY0按下
//			{
//				if(netpro<6)netpro++;
//				else netpro=0;
//			}else if(key==KEY1_PRES)//KEY1按下
//			{
//				if(netpro>0)netpro--;
//				else netpro=6; 
//			}else if(key==WKUP_PRES)//KEY_UP按下
//			{
//				if(netpro==0)//进入通信选项
//				{
//				  LoRa_Process();//开始数据测试
//				  netpro=0;//索引返回第0
//				  Menu_ui();
//				}else
//				{
//					Show_Str(30+40,95+45+netpro*25+2,200,16,"________",16,1);//显示下划线,表示选中
//					Show_Str(30+10,95+45+netpro*25,200,16,"->",16,0);//指向新条目
//					Menu_cfg(netpro);//参数配置
//					LCD_Fill(30+40,95+45+netpro*25+2+15,30+40+100,95+45+netpro*25+2+18,WHITE);//清除下划线显示
//				}
//			}
//			Show_Str(30+10,95+45+netpro*25,200,16,"->",16,0);//指向新条目
//		}
	
		LoRa_Process();//开始数据测试
		
		t++;
		if(t==30)
		{
			t=0;
			LED1=~LED1;		
		}
		delay_ms(10);
	}	
	
}
