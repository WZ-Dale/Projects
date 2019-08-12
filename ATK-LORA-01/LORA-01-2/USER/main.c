#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"		
#include "touch.h"	
#include "usart2.h"	
#include "lora_app.h"
/************************************************
 ALIENTEK Mini STM32开发板
 ATK-LORA-01模块测试实验 
************************************************/

 //主函数
int main(void)
{
	u8 key,fontok=0; 
  NVIC_Configuration();	 
	delay_init();	    	         //延时函数初始化	  
	uart_init(115200);	 	         //串口初始化
  usmart_dev.init(72);		     //初始化USMART		 
	LCD_Init();				         //初始化液晶 
	LED_Init();         	         //LED初始化	 
	KEY_Init();				         //按键初始化	  													    
//	tp_dev.init();			         //触摸屏初始化
 	mem_init();				         //初始化内存池	    
// 	exfuns_init();			         //为fatfs相关变量申请内存  
//  f_mount(fs[0],"0:",1); 	         //挂载SD卡 
// 	f_mount(fs[1],"1:",1); 	         //挂载FLASH.
//	key=KEY_Scan(0);  
//	if(key==KEY0_PRES)		         //强制校准
//	{
//		LCD_Clear(WHITE);	         //清屏
//	  tp_dev.adjust();  	         //屏幕校准  
//	  LCD_Clear(WHITE);	         //清屏
//	}
//	fontok=font_init();		         //检查字库是否OK
//	if(fontok||key==KEY1_PRES)       //需要更新字库（字库不存在/KEY1按下）			 
//	{
//		LCD_Clear(WHITE);		   	 //清屏
// 		POINT_COLOR=RED;			 //设置字体为红色	   	   	  
//		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
//		while(SD_Initialize())		 //检测SD卡
//		{
//			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
//			delay_ms(200);
//			LCD_Fill(60,70,200+60,70+16,WHITE);
//			delay_ms(200);		    
//		}								 						    
//		LCD_ShowString(60,70,200,16,16,"SD Card OK");
//		LCD_ShowString(60,90,200,16,16,"Font Updating...");
//		key=update_font(20,110,16); //更新字库
//		while(key)                  //更新失败		
//		{			 		  
//			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
//			delay_ms(200);
//			LCD_Fill(20,110,200+20,110+16,WHITE);
//			delay_ms(200);		       
//		} 		  
//		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
//		delay_ms(1500);	
//		LCD_Clear(WHITE);           //清屏	       
//	}  
	Lora_Test();//主测试
}

