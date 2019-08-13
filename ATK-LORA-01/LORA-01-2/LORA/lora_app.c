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

//********************************************************************************
//ATK-LORA-01ģ�鹦������		  
//********************************************************************************


//�豸������ʼ��(�����豸������lora_cfg.h����)
_LoRa_CFG LoRa_CFG=
{
	.addr = LORA_ADDR,       //�豸��ַ
	.power = LORA_POWER,     //���书��
	.chn = LORA_CHN,         //�ŵ�
	.wlrate = LORA_RATE,     //��������
	.wltime = LORA_WLTIME,   //˯��ʱ��
	.mode = LORA_MODE,       //����ģʽ
	.mode_sta = LORA_STA,    //����״̬
	.bps = LORA_TTLBPS ,     //����������
	.parity = LORA_TTLPAR    //У��λ����
};


//ȫ�ֲ���
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

//�豸����ģʽ(���ڼ�¼�豸״̬)
u8 Lora_mode=0;// 0:����ģʽ 1:����ģʽ 2:����ģʽ

//��¼�ж�״̬
static u8 Int_mode=0;//0���ر� 1:������ 2:�½���

//AUX�ж�����
//mode:���õ�ģʽ 0:�ر� 1:������ 2:�½���
void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//�ر�
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //������
	  else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
		
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//��¼�ж�ģʽ
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
    
}

//LORA_AUX�жϷ�����
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//������(����:��ʼ�������� ����:���ݿ�ʼ���)     
	   {
		  if(Lora_mode==1)//����ģʽ
		  {
			 USART2_RX_STA=0;//���ݼ�����0
		  }
		  Int_mode=2;//�����½���
		  LED0=0;//DS0��
	   }
       else if(Int_mode==2)//�½���(����:�����ѷ����� ����:�����������)	
	   {
		  if(Lora_mode==1)//����ģʽ
		  {
			 USART2_RX_STA|=1<<15;//���ݼ���������
		  }else if(Lora_mode==2)//����ģʽ(�������ݷ������)
		  {
			 Lora_mode=1;//�������ģʽ
		  }
		  Int_mode=1;//����������
          LED0=1;//DS0��		   
	   }
       Aux_Int(Int_mode);//���������жϱ���
	   EXTI_ClearITPendingBit(EXTI_Line4); //���LINE4�ϵ��жϱ�־λ  
	}	
}

//LoRaģ���ʼ��
//����ֵ:0,���ɹ�
//       1,���ʧ��
u8 LoRa_Init(void)
{
	 u8 retry=0;
	 u8 temp=1;
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LORA_MD0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //��������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //�ж��߹ر�
  	 EXTI_Init(&EXTI_InitStructure);//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2�� 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//�����ȼ�3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;		   //�ر��ⲿ�ж�ͨ��
   	 NVIC_Init(&NVIC_InitStructure); 
	 
	 LORA_MD0=0;
	 LORA_AUX=0;
	
	 while(LORA_AUX)//ȷ��LORAģ���ڿ���״̬��(LORA_AUX=0)
	 {
		 Show_Str(40+30,50+20,200,16,"Module is busy. Wait!!",16,0); 	
		 delay_ms(500);
		 Show_Str(40+30,50+20,200,16,"                    ",16,0);
         delay_ms(100);		 
	 }
	 usart2_init(115200);	         //��ʼ������2 
	 
	 LORA_MD0=1;//����ATģʽ
	 delay_ms(40);
	 retry=3;
	 while(retry--)
	 {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//���ɹ�
			 USART2_RX_STA=0;
			 break;
		 }	
	 }
	 if(retry==0) temp=1;//���ʧ��
	 return temp;
}


//Loraģ���������
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	usart2_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//��������ģʽǰ����ͨ�Ų����ʺ�У��λ(115200 8λ���� 1λֹͣ ������У�飩
	usart2_rx(1);//��������3����
	
	while(LORA_AUX);//�ȴ�ģ�����
	LORA_MD0=1; //��������ģʽ
	delay_ms(40);
	Lora_mode=0;//���"����ģʽ"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	lora_send_cmd("AT","OK",70);
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//�����豸��ַ
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//�����ŵ��Ϳ�������
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//���÷��书��
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//���ù���ģʽ
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//���÷���״̬
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//����˯��ʱ��
	lora_send_cmd(sendbuf,"OK",100);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//���ô��ڲ����ʡ�����У��λ
	lora_send_cmd(sendbuf,"OK",100);

	LORA_MD0=0;//�˳�����,����ͨ��
	delay_ms(40);
	while(LORA_AUX);//�ж��Ƿ����(ģ����������ò���)
	USART2_RX_STA=0;
	Lora_mode=1;//���"����ģʽ"
	usart2_set(LoRa_CFG.bps,LoRa_CFG.parity);//����ͨ��,����ͨ�Ŵ�������(�����ʡ�����У��λ)
	Aux_Int(1);//����LORA_AUX�������ж�	
}

u8 Dire_Date[]={0x11,0x22,0x33,0x44,0x55};//����������
u8 date[30]={0};//��������
u8 Tran_Data[30]={0};//͸������

#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
extern u32 obj_addr;//��¼�û�����Ŀ���ַ
extern u8 obj_chn;//��¼�û�����Ŀ���ŵ�

u8 wlcd_buff[10]={0}; //LCD��ʾ�ַ���������

//Loraģ�鷢������
void LoRa_SendData(void)
{      
	 static u8 num=0;
   u16 addr;
	 u8 chn;
	 u16 i=0; 
		
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//͸������
	{
		sprintf((char*)Tran_Data,"ATK-LORA-01 TEST %d",num);
		u2_printf("%s\r\n",Tran_Data);
		LCD_Fill(0,195,240,220,WHITE); //�����ʾ
		Show_Str_Mid(10,195,Tran_Data,16,240);//��ʾ���͵�����
		
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//������
	{
		
		addr = (u16)obj_addr;//Ŀ���ַ
		chn = obj_chn;//Ŀ���ŵ�
		
		date[i++] =(addr>>8)&0xff;//��λ��ַ
		date[i++] = addr&0xff;//��λ��ַ
		date[i] = chn;  //�����ŵ�
		for(i=0;i<Dire_DateLen;i++)//����д������BUFF
		{
			date[3+i] = Dire_Date[i];
		}
		for(i=0;i<(Dire_DateLen+3);i++)
		{
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
			USART_SendData(USART2,date[i]); 
		}	
		
        //��ʮ�����Ƶ�����ת��Ϊ�ַ�����ӡ��lcd_buff����
		sprintf((char*)wlcd_buff,"%x %x %x %x %x %x %x %x",
				date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7]);
		
		LCD_Fill(0,200,240,230,WHITE);//�����ʾ
		Show_Str_Mid(10,200,wlcd_buff,16,240);//��ʾ���͵�����
		
	    Dire_Date[4]++;//Dire_Date[4]���ݸ���
		
	}
			
}

u8 rlcd_buff[10]={0}; //LCD��ʾ�ַ���������



u8 save = 0;
u8 DT = 0;
u8 d = 0;



//Loraģ���������
void LoRa_ReceData(void)
{
    u16 i=0;
    u16 len=0;
   
	//����������
	if(USART2_RX_STA&0x8000)
	{
		len = USART2_RX_STA&0X7FFF;
		USART2_RX_BUF[len]=0;//���ӽ�����
		USART2_RX_STA=0;

		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
			USART_SendData(USART1,USART2_RX_BUF[i]); 
		}
		 LCD_Fill(10,260,240,320,WHITE);

		if(LoRa_CFG.mode_sta==LORA_STA_Tran)//͸������
		{	
			Show_Str_Mid(10,270,USART2_RX_BUF,16,240);//��ʾ���յ�������
		
		
		
save = atoi((char*)USART2_RX_BUF);
DT = save;	
		
		
		}else if(LoRa_CFG.mode_sta==LORA_STA_Dire)//������
		{
			//��ʮ�����Ƶ�����ת��Ϊ�ַ�����ӡ��lcd_buff����
			sprintf((char*)rlcd_buff,"%x %x %x %x %x",
			USART2_RX_BUF[0],USART2_RX_BUF[1],USART2_RX_BUF[2],USART2_RX_BUF[3],USART2_RX_BUF[4]);
				
			Show_Str_Mid(10,270,rlcd_buff,16,240);//��ʾ���յ�������	
		}
		memset((char*)USART2_RX_BUF,0x00,len);//���ڽ��ջ�������0

	}

}


//���ͺͽ��մ���
void LoRa_Process(void)
{
	u8 key=0;
	static u8 t=0;

 DATA:
	Process_ui();//������ʾ
	LoRa_Set();//LoRa����(�������������ô��ڲ�����Ϊ115200) 
	while(1)
	{
		
		key = KEY_Scan(0);
		
		if(key==KEY0_PRES)
		{
			if(LoRa_CFG.mode_sta==LORA_STA_Dire)//���Ƕ�����,���������Ŀ���ַ���ŵ�����
			{
				usart2_rx(0);//�رմ��ڽ���
				Aux_Int(0);//�ر��ж�
				Dire_Set();//��������Ŀ���ַ���ŵ�
				goto DATA;
			}
		}else if(key==WKUP_PRES)//�������˵�ҳ��
		{
			usart2_rx(0);//�رմ��ڽ���
			Aux_Int(0);//�ر��ж�
			break;
		}
		else if(key==KEY1_PRES)//��������
		{
			  if(!LORA_AUX)//������
			  {
				  Lora_mode=2;//���"����״̬"
				  LoRa_SendData();//��������    
			  }
		}
			
		//���ݽ���
		LoRa_ReceData();
		
		t++;
		if(t==20)
		{
			t=0;
			LED1=~LED1;


			
//PCout(0) = ~PCout(0);
d++;
if(d==1){
	d=0;
	DT--;
}
if(DT==0){
	DT = save;
	PCout(0) = ~PCout(0);
}

			
			
		}			
		delay_ms(10);		
   }
	
}


//�����Ժ���
void Lora_Test(void)
{
	u8 t=0;
	u8 key=0;
	u8 netpro=0;
	
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
  Show_Str_Mid(0,30,"ATK-LORA-01 _wangze",16,240); 
	
	while(LoRa_Init())//��ʼ��ATK-LORA-01ģ��
	{
		Show_Str(40+30,50+20,200,16,"No module detected!!!",16,0); 	
		delay_ms(300);
		Show_Str(40+30,50+20,200,16,"                ",16,0);
	}
	Show_Str(40+30,50+20,200,16,"Module OK !!!",16,0);
    delay_ms(500); 	
	Menu_ui();//�˵�
	
	while(1)
	{
		
		key = KEY_Scan(0);
		if(key)
		{
			Show_Str(30+10,95+45+netpro*25,200,16,"  ",16,0);//���֮ǰ����ʾ
			
			 if(key==KEY0_PRES)//KEY0����
			{
				if(netpro<6)netpro++;
				else netpro=0;
			}else if(key==KEY1_PRES)//KEY1����
			{
				if(netpro>0)netpro--;
				else netpro=6; 
			}else if(key==WKUP_PRES)//KEY_UP����
			{
				if(netpro==0)//����ͨ��ѡ��
				{
				  LoRa_Process();//��ʼ���ݲ���
				  netpro=0;//�������ص�0
				  Menu_ui();

				}else
				{
					Show_Str(30+40,95+45+netpro*25+2,200,16,"________",16,1);//��ʾ�»���,��ʾѡ��
					Show_Str(30+10,95+45+netpro*25,200,16,"->",16,0);//ָ������Ŀ
					Menu_cfg(netpro);//��������
					LCD_Fill(30+40,95+45+netpro*25+2+15,30+40+100,95+45+netpro*25+2+18,WHITE);//����»�����ʾ
				}
			}
			Show_Str(30+10,95+45+netpro*25,200,16,"->",16,0);//ָ������Ŀ
		}
	
		t++;
		if(t==30)
		{
			t=0;
			LED1=~LED1;		
		}
		delay_ms(10);
	}	
	
}