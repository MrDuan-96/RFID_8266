//Arduino  uno                                    RC522 
//D9                  <------------->             RST    
//D10                 <------------->             SDA   
//D11                 <------------->             MOSI
//D12                 <------------->             MISO
//D13                 <------------->             SCK
#include "RC522.h"
#include <SPI.h>
#include <Servo.h>

Servo myservo;  // 定义Servo对象来控制

#define Stgr 6  //定义舵机引脚

#define Relay 2 

#define key 8     //定义8266引脚8

int keypin = 0;   //定义输入状态
int pos = 0;    // 角度存储变量
int key_status = 1;

//前面4个字节为卡片序列号，第五个字节是校验字节
unsigned char serNum[5];

void setup()  
{ 
    Serial.begin(9600); 
    SPI.begin();      //rc522 启动spi通信
    myservo.attach(9);  // 控制线连接数字9
    //Serial.print("NCU Door System!\n");
    //pinMode(Relay,OUTPUT);
    pinMode(key,INPUT);         //定义8266为输入模式
    pinMode(Stgr,OUTPUT);         //定义舵机引脚为输出模式
    pinMode(NRSTPD,OUTPUT);    // Set digital Reset , Not Reset and Power-down
        
    pinMode(chipSelectPin,OUTPUT);    //将数字引脚10设置为OUTPUT以进行连接
    digitalWrite(chipSelectPin, LOW); // 激活RFID阅读器
    digitalWrite(key,LOW);        //默认key为低，门锁关闭
    
    MFRC522_Init();						//初始化RFID
    Stgr_Init();             //初始化舵机
}

void loop()
{
    unsigned char status;
    unsigned char str[MAX_LEN];

    //读取keypin的状态
    keypin = digitalRead(key);
    
    // Search card, return card types
    status = MFRC522_Request(PICC_REQIDL, str); 
//    Rotation_Right();
    if (status == MI_OK)			//读取到ID卡时候
    {   
  		// Show card type;
  		//ShowCardType(str);
      
  		//Prevent conflict, return the 4 bytes Serial number of the card
  		status = MFRC522_Anticoll(str);
      
  		// str[0..3]: serial number of the card
  		// str[4]: XOR checksum of the SN.
  		if (status == MI_OK)
  		{
  			memcpy(serNum, str, 5);
  			Serial.print("ID:");
  			ShowCardID(serNum);
      
  			// Check people associated with card ID
  			unsigned char* id = serNum;
  			if(  id[0]==0xf0 && id[1]==0x5d && id[2]==0xfa && id[3]==0xc6 || 
  			     id[0]==0x6b && id[1]==0xc3 && id[2]==0xa4 && id[3]==0x0d || 
             id[0]==0x4b && id[1]==0x21 && id[2]==0xe6 && id[3]==0x0d || 
             id[0]==0xe3 && id[1]==0x57 && id[2]==0x6f && id[3]==0x02 || 
             id[0]==0x6b && id[1]==0x21 && id[2]==0xf0 && id[3]==0x0d
           ) 
             
  			{
          Door_Open();
  				//digitalWrite(Relay,HIGH);		//打开继电器
          Serial.println("Open the door!!");
  			} 
  			else if(id[0]==0x4C && id[1]==0xB3 && id[2]==0x74 && id[3]==0x19) 
  			{
  				//digitalWrite(Relay,LOW);		//继电器
          Serial.println("The Host 2!");
  			}
        else
        {
           Serial.println("Stranger!");
        }
  			
  		}
		
    }
   else if(keypin == HIGH && (key_status == 1))      //读取到8266输入低电平状态（开锁指令）
   {
        Door_Open1();
        //digitalWrite(keypin,LOW);
        key_status = 0;
       
   }
   else if(keypin == LOW &&(key_status == 0))
   {
    Door_Close1();
    key_status = 1;
    }
    MFRC522_Halt(); //command the card into sleep mode 
    delay(100);
}

void Stgr_Init()
{
  int i=0;
  for(i=0;i<100;i++)
  { 
    digitalWrite(Stgr,HIGH);  //led 引脚输出高电平，点亮led
    delayMicroseconds(2300);            //延时2.3ms
    digitalWrite(Stgr,LOW); //led引脚输出低电平，熄灭led
    delayMicroseconds(17700);         //延时17.7ms      //左转
  }
}

/*void Door_Open1()
{
    for (; pos >= 0; pos--)
    {
      myservo.write(pos);              // 舵机角度写入
      delay(4);                       // 等待转动到指定角度
    }
    delay(500);
}*/
void Door_Open()
{
  int i = 0;
  delay(500);
  for(i=0;i<100;i++)
  {
    digitalWrite(Stgr,HIGH);  //led引脚输出高电平，点亮led
    delayMicroseconds(500);            //延时0.5ms
    digitalWrite(Stgr,LOW); //led引脚输出低电平，熄灭led
    delayMicroseconds(19500);         //延时19.5ms      //右转
  }
  delay(3000);
  for(i=0;i<100;i++)
  { 
    digitalWrite(Stgr,HIGH);  //led 引脚输出高电平，点亮led
    delayMicroseconds(2300);            //延时2.3ms
    digitalWrite(Stgr,LOW); //led引脚输出低电平，熄灭led
    delayMicroseconds(17700);         //延时17.7ms      //左转
  }
}

void Door_Open1()
{
  int i = 0;
  delay(500);
  for(i=0;i<100;i++)
  {
    digitalWrite(Stgr,HIGH);  //led引脚输出高电平，点亮led
    delayMicroseconds(500);            //延时0.5ms
    digitalWrite(Stgr,LOW); //led引脚输出低电平，熄灭led
    delayMicroseconds(19500);         //延时19.5ms      //右转
  }
  delay(3000);
  
}

void Door_Close1()
{
  int i = 0;
  delay(500);
  for(i=0;i<100;i++)
  { 
    digitalWrite(Stgr,HIGH);  //led 引脚输出高电平，点亮led
    delayMicroseconds(2300);            //延时2.3ms
    digitalWrite(Stgr,LOW); //led引脚输出低电平，熄灭led
    delayMicroseconds(17700);         //延时17.7ms      //左转
  }
  
}
