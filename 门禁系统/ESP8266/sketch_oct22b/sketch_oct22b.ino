#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#define BLINKER_PRINT Serial
#include <Blinker.h>

char auth[] = "ac05e0eb1789"; //在app中获取到的Secret Key
char ssid[] = "VLC-313"; //你的WiFi热点名称
char pswd[] = "147258369"; //你的WiFi密码

#define GPIO 16//ESP8266 D1口即为GPIO16，我们将通过D1电平输出控制UNO

#define BUTTON_1 "ButtonKey"


BlinkerButton Button1("key1");
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(GPIO,!digitalRead(GPIO));
    Blinker.vibrate(); 
}

void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ",state);

    if (state == BLINKER_CMD_OFF)
    {
        digitalWrite(GPIO,HIGH);  //关门,状态为高
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
    else if (state == BLINKER_CMD_ON) 
    {
        digitalWrite(GPIO,LOW);   //开门，状态为低

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    }
}


void setup() {
    Serial.begin(115200);
    pinMode(GPIO,OUTPUT);
    digitalWrite(GPIO,LOW);
    Blinker.begin(auth, ssid, pswd);
    Button1.attach(button1_callback);

    //回调函数
    BlinkerMIOT.attachPowerState(miotPowerState);


}

void loop()
{
    Blinker.run();
}
