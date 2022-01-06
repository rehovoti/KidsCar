#include "BluetoothSerial.h"
#include "config.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void initBluetooth()
{
  SerialBT.begin("OrensCar"); //Bluetooth device name
}

char getBluetoothCommand(Joystick_reading &jr)
{
  if (SerialBT.available())
  {
    return readBT(jr);
  }
  return NO_COMMAND;
}

char readBT(Joystick_reading &jr)
{
  static char readStr[10];
  static char idx;
  unsigned short mostSig;
  unsigned short leastSig;
  char sym = SerialBT.read();
  Serial.write(sym);
  if (sym == '#')
  {
    Serial.println("<==>");
    idx = 0;
    char strAngle[4];
    char strPower[5];

    strncpy(strAngle, readStr, 3);
    strncpy(strPower, readStr + 3, 4);

    mostSig = atoi(strAngle);
    leastSig = atoi(strPower);

    if((mostSig == 0) && (leastSig == 2))
      return HORN_COMMAND;
    if((mostSig == 0) && (leastSig == 3))
      return HEADLIGHTS_COMMAND;
      
    jr.angle = mostSig;
    jr.power = leastSig;

    return JOYSTICK_COMMAND;
  }
  readStr[idx++] = sym;
  return INCOMPLETE_COMMAND;
}
