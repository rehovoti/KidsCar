#include "Motors.h"
#include "BluetoothCmd.h"

void setup()
{
  initBluetooth();
  motorsSetup();
  Serial.begin(115200);
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop()
{
  Joystick_reading jr;
  char bluetoothCommand;

  bluetoothCommand = getBluetoothCommand(jr);
  if((bluetoothCommand != INCOMPLETE_COMMAND) && (bluetoothCommand != NO_COMMAND))
  {
    Serial.print("Command received: ");
    Serial.write(bluetoothCommand);
    Serial.println("!");
  }
//  if (bluetoothCommand == END_COMMAND)
//  {
//    motorsShutDown();
//  }
  if (bluetoothCommand == JOYSTICK_COMMAND)
  {
    motorsControlMovement(jr);
  }
}
