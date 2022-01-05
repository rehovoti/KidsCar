#include "BluetoothSerial.h"
#include <stdlib.h>
#include <string.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#define LEFT_FORWARD LOW
#define LEFT_REVERSE HIGH
#define RIGHT_FORWARD LOW
#define RIGHT_REVERSE HIGH

//const unsigned char DIRECTION_PIN_MOTOR_LEFT = 19;
//const unsigned char PWM_PIN_MOTOR_LEFT = 21;
//const unsigned char DIRECTION_PIN_MOTOR_RIGHT = 22;
//const unsigned char PWM_PIN_MOTOR_RIGHT = 23;

//const unsigned char DIRECTION_PIN_MOTOR_LEFT = 22;
//const unsigned char PWM_PIN_MOTOR_LEFT = 23;
//const unsigned char DIRECTION_PIN_MOTOR_RIGHT = 19;
//const unsigned char PWM_PIN_MOTOR_RIGHT = 21;
//const unsigned char PWM_CHANNEL_LEFT = 0;
//const unsigned char PWM_CHANNEL_RIGHT = 1;


const unsigned char PIN_MOTOR_LEFT_1 = 22;
const unsigned char PIN_MOTOR_LEFT_2 = 23;
const unsigned char PIN_MOTOR_RIGHT_1 = 19;
const unsigned char PIN_MOTOR_RIGHT_2 = 21;
const unsigned char PWM_CHANNEL_LEFT_1 = 0;
const unsigned char PWM_CHANNEL_LEFT_2 = 1;
const unsigned char PWM_CHANNEL_RIGHT_1 = 2;
const unsigned char PWM_CHANNEL_RIGHT_2 = 3;

BluetoothSerial SerialBT;

typedef struct {
  unsigned short angle;
  unsigned short power;
} Joystick_reading;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("OrensCar"); //Bluetooth device name
  pinMode(PIN_MOTOR_LEFT_1, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_2, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_1, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_2, OUTPUT);
  // configure LED PWM functionalitites
  // ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(PWM_CHANNEL_LEFT_1, 30000, 8); 
  ledcSetup(PWM_CHANNEL_LEFT_2, 30000, 8); 
  ledcSetup(PWM_CHANNEL_RIGHT_1, 30000, 8); 
  ledcSetup(PWM_CHANNEL_RIGHT_2, 30000, 8); 
  ledcAttachPin(PIN_MOTOR_LEFT_1, PWM_CHANNEL_LEFT_1);
  ledcAttachPin(PIN_MOTOR_LEFT_2, PWM_CHANNEL_LEFT_2);
  ledcAttachPin(PIN_MOTOR_RIGHT_1, PWM_CHANNEL_RIGHT_1);
  ledcAttachPin(PIN_MOTOR_RIGHT_2, PWM_CHANNEL_RIGHT_2);
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop()
{
  Joystick_reading jr;
  if (SerialBT.available())
  {
//    Serial.write(SerialBT.read());
    if(readBT(jr)) {
      Serial.print("Angle: ");
      Serial.println(jr.angle);
      Serial.print("Power: ");
      Serial.println(jr.power);
      controlMovement(jr);
      //while(SerialBT.available()) SerialBT.read();
    }
    else
    {
      spinLeftMotor(0);
      spinRightMotor(0);
    }
  }
}

int controlMovement(Joystick_reading& jr)
{
  short left_load, right_load;
  if((jr.angle >= 0) && (jr.angle < 90)) 
  {
    left_load = round(255.0 * jr.power / 1000.0);
    right_load = round((jr.angle - 45.0) * 5.6 * jr.power / 1000.0);
  }
  if((jr.angle >= 90) && (jr.angle < 180)) 
  {
    left_load = round((jr.angle - 135.0) * -5.6 * jr.power / 1000.0);
    right_load = round(255.0 * jr.power / 1000.0);
  }
  if((jr.angle >= 180) && (jr.angle < 270)) 
  {
    left_load = round(-255.0 * jr.power / 1000.0);
    right_load = round((jr.angle - 225.0) * -5.6 * jr.power / 1000.0);
  }
  if((jr.angle >= 270) && (jr.angle <= 360)) 
  {
    left_load = round((jr.angle - 315.0) * 5.6 * jr.power / 1000.0);
    right_load = round(-255.0 * jr.power / 1000.0);
  }
  Serial.print(left_load);
  Serial.print(" : ");
  Serial.println(right_load);
  spinLeftMotor(left_load);
  spinRightMotor(right_load);
}

void spinLeftMotor(short value)
{
  int direction_pin, pwm_channel;
  if(value < 0) // reverse
  {
    ledcWrite(PWM_CHANNEL_LEFT_2, 0);
    pwm_channel = PWM_CHANNEL_LEFT_1;
    value = -value;
    Serial.print("Left Reverse, ");
  }
  else
  {
    ledcWrite(PWM_CHANNEL_LEFT_1, 0);
    pwm_channel = PWM_CHANNEL_LEFT_2;
  }
  if(value !=0 )
    value = map(value, 0, 255, 100, 255);
  Serial.print("Left value: ");
  Serial.println(value);
  ledcWrite(pwm_channel, value);
}

void spinRightMotor(short value)
{
  int pwm_channel;
  if(value < 0) // reverse
  {
    ledcWrite(PWM_CHANNEL_RIGHT_2, 0);
    pwm_channel = PWM_CHANNEL_RIGHT_1;
    value = -value;
    Serial.print("Right Reverse, ");
  }
  else
  {
    ledcWrite(PWM_CHANNEL_RIGHT_1, 0);
    pwm_channel = PWM_CHANNEL_RIGHT_2;
  }
  if(value !=0 )
    value = map(value, 0, 255, 100, 255);
  Serial.print("Right value: ");
  Serial.println(value);
  ledcWrite(pwm_channel, value);
}

char readBT (Joystick_reading& jr)
{
  static char readStr[10];
  static char idx;
  char sym = SerialBT.read();
  Serial.write(sym);
  if(sym == '#') {
    idx = 0;
    return 0;
  }
  readStr[idx] = sym;
  idx++;
  if(idx == 7) {
    char strAngle[4];
    char strPower[5];
    
    strncpy(strAngle,readStr,3);
    strncpy(strPower,readStr+3,4);

    jr.angle = atoi(strAngle);
    jr.power = atoi(strPower);
    
    return 1;
  }
}
