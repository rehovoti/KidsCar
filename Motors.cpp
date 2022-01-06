#include "config.h"
#include <arduino.h>

void spinLeftMotor(short value);
void spinRightMotor(short value);

void motorsSetup()
{
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
}

void motorsShutDown()
{
  spinLeftMotor(0);
  spinRightMotor(0);
}

void motorsControlMovement(Joystick_reading &jr)
{
  short left_load, right_load;
  if ((jr.angle >= 0) && (jr.angle < 90))
  {
    left_load = round(255.0 * jr.power / 1000.0);
    right_load = round((jr.angle - 45.0) * 5.6 * jr.power / 1000.0);
  }
  if ((jr.angle >= 90) && (jr.angle < 180))
  {
    left_load = round((jr.angle - 135.0) * -5.6 * jr.power / 1000.0);
    right_load = round(255.0 * jr.power / 1000.0);
  }
  if ((jr.angle >= 180) && (jr.angle < 270))
  {
    left_load = round(-255.0 * jr.power / 1000.0);
    right_load = round((jr.angle - 225.0) * -5.6 * jr.power / 1000.0);
  }
  if ((jr.angle >= 270) && (jr.angle <= 360))
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
  if (value < 0) // reverse
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
  if (value != 0)
    value = map(value, 0, 255, 100, 255);
  Serial.print("Left value: ");
  Serial.println(value);
  ledcWrite(pwm_channel, value);
}

void spinRightMotor(short value)
{
  int pwm_channel;
  if (value < 0) // reverse
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
  if (value != 0)
    value = map(value, 0, 255, 100, 255);
  Serial.print("Right value: ");
  Serial.println(value);
  ledcWrite(pwm_channel, value);
}
