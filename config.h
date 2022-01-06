extern const unsigned char PIN_MOTOR_LEFT_1;
extern const unsigned char PIN_MOTOR_LEFT_2;
extern const unsigned char PIN_MOTOR_RIGHT_1;
extern const unsigned char PIN_MOTOR_RIGHT_2;
extern const unsigned char PWM_CHANNEL_LEFT_1;
extern const unsigned char PWM_CHANNEL_LEFT_2;
extern const unsigned char PWM_CHANNEL_RIGHT_1;
extern const unsigned char PWM_CHANNEL_RIGHT_2;

extern const unsigned char NO_COMMAND;
extern const unsigned char JOYSTICK_COMMAND;
extern const unsigned char HEADLIGHTS_COMMAND;
extern const unsigned char HORN_COMMAND;
extern const unsigned char INCOMPLETE_COMMAND;
extern const unsigned char UNKNOWN_COMMAND;

#ifndef CONF_H
#define CONF_H

typedef struct
{
  unsigned short angle;
  unsigned short power;
} Joystick_reading;

#endif
