#include "derivative.h"      /* derivative-specific definitions */
#include "18B20.h"      


/*************************************************************/
/*                         ��ʱ����1                         */
/*************************************************************/
void delay8us(unsigned int n) 
{
    unsigned int i;
    for(i=0;i<n;i++) 
    {
        ECT_TFLG1_C1F = 1;              //�����־λ
        ECT_TC1 = ECT_TCNT + 2;             //��������Ƚ�ʱ��Ϊ8us
        while(ECT_TFLG1_C1F == 0);      //�ȴ���ֱ����������Ƚ��¼�
    }
}

/*************************************************************/
/*                      ��ʼ��18B20                          */
/*************************************************************/
void init18b20(void)
{
 DSDDR=1;
 DSO = 1; 
 delay8us(1);
 DSO = 0;          //���������ߣ���λ���ߣ�
 delay8us(63);     //��ʱ504us 
 DSO = 1;         //���������ߣ�
 delay8us(4);     //��ʱ32us��
 DSDDR=0;
 while(DSI)       //�ȴ�����������Ӧ���źţ�
 {asm("nop");}
 DSDDR=1;
 delay8us(16);     //��ʱ128us�� 
 DSO = 1;          //���������ߣ�׼�����ݴ��䣻
}

/*************************************************************/
/*                      ��18B20д������                      */
/*************************************************************/
void WR18b20(byte cmd)
{
    unsigned char k;
    for(k=0;k<8;k++)
    {
        if(cmd & 0x01)  	    //��λ��ǰ��
        {
            DSO = 0;    
            delay8us(1); 
            DSO = 1;          //��������;
        }                 	
        else 
        {
            DSO = 0;    	       
            delay8us(1);  
        }
        delay8us(8);  		 //��ʱ64us�ȴ�������������
        DSO = 1;      	   //��������
        delay8us(1);      	 
        cmd >>= 1;
    }
}

/*************************************************************/
/*                      ��18B20��ȡ����                      */
/*************************************************************/
unsigned char RD18b20(void)
{
    unsigned char k;
    unsigned char tmp=0;
    DSO = 1;  
    delay8us(1);          //׼������
    for(k=0;k<8;k++)
    {
      tmp >>= 1;    		 //�ȶ�ȡ��λ
      DSO = 0;     	     //Read init��
      delay8us(1);     		
      DSO = 1;      		//����д1������������Ľ��ǲ�Ԥ�ڵ����ݣ�
      asm("nop");asm("nop");asm("nop");			//��ʱ9us�  
      DSDDR=0;
      asm("nop");
      if(DSI)      	//��12us����ȡ���ݣ�
      tmp |= 0x80;
      delay8us(8);
      DSDDR=1;
      DSO = 1;  
      delay8us(1);
        //�ָ�One Wire Bus��
    }
    return tmp; 
}

/*************************************************************/
/*                      ��18B20��ȡ�¶�                      */
/*************************************************************/
unsigned int read_T(void)
{  
	 unsigned int t;
	 unsigned char temp[2];
   
   init18b20();
   WR18b20(0xcc);  
   WR18b20(0x44); 
   init18b20();
   WR18b20(0xcc);
   WR18b20(0xbe);
   temp[0]=RD18b20();
   temp[1]=RD18b20();
   init18b20();
   t=(temp[1]<<8)|temp[0];
   return(t); 
 } 

