#define BLINKER_WIFI  // 定义使用Blinker库的Wi-Fi功能
#define BLINKER_MIOT_LIGHT  // 定义使用Blinker库的MIOT Light组件

#include <Blinker.h>
#include <Servo.h>

Servo myservo; // 定义舵机

#define rotation_angle 70 // 旋转角度，复位角度为90时最大为90，复位角度为0时最大180
#define default_angle 90 // 舵机复位角度
#define delay_time 1000 // 归位延迟时间

char auth[] = "90f3fd90ad2d"; // Blinker Key
char ssid[] = "Magic4"; // Wi-Fi名称
char pswd[] = "00000000"; // Wi-Fi密码

// 新建组件对象
BlinkerButton Button1("test");

int counter = 0;

// MIOT Light组件的电源状态操作回调函数
void miotPowerState(const String &state) {
    BLINKER_LOG("需要设置电源状态: ", state);

    if (state == BLINKER_CMD_ON) {
        myservo.write(default_angle + rotation_angle); // 舵机转到指定角度
        BlinkerMIOT.powerState("on"); // 反馈MIOT Light组件的电源状态为开启
        BlinkerMIOT.print(); // 将反馈信息发送给Blinker云平台
        delay(delay_time); // 延时1秒
        myservo.write(default_angle); // 舵机归位，回到垂直状态
    } else if (state == BLINKER_CMD_OFF) {
        myservo.write(default_angle - rotation_angle); // 舵机转到指定角度
        BlinkerMIOT.powerState("off"); // 反馈MIOT Light组件的电源状态为关闭
        BlinkerMIOT.print(); // 将反馈信息发送给Blinker云平台
        delay(delay_time); // 延时1秒
        myservo.write(default_angle); // 舵机归位，回到垂直状态
    }
}

// 按钮组件回调函数，按下按钮后执行
void button1_callback(const String &state) {
    BLINKER_LOG("获取按钮状态: ", state);
    if (state == "on") {
        myservo.write(default_angle + rotation_angle); // 舵机转到指定角度
        delay(delay_time); // 延时
        myservo.write(default_angle); // 舵机归位，回到垂直状态
    } else if (state == "press" || state == "tap") {
        myservo.write(default_angle - rotation_angle); // 舵机转到指定角度
        delay(delay_time); // 延时
        myservo.write(default_angle); // 舵机归位，回到垂直状态
    }
}

// 如果未绑定的组件被触发，则会执行其中的内容
void dataRead(const String &data) {
    BLINKER_LOG("Blinker读取字符串: ", data);
    counter++;
}

void setup() {
    // 初始化串口
    Serial.begin(9600);
    BLINKER

