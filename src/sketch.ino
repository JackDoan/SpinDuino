#include <EEPROM.h>

  int statusLEDRed = 12;
  int statusLEDGreen = 13;
  int pwmPin = 10;
  int commandCapturePin = 11;
  int calibrationJumper = 9;
  int calibrationSettings[2] = {0};  //0 = brake/float; 1 = full throttle
  int EEPROMAddresses[2] = {0};
  int timeout = 20; //TODO get a value for this

void setup() {
  EEPROMAddresses[0] = 0;
  EEPROMAddresses[1] = 1;
  //eeprom_busy_wait();
  calibrationSettings[0] = EEPROM.read(EEPROMAddresses[0]);
  //eeprom_busy_wait();
  calibrationSettings[1] = EEPROM.read(EEPROMAddresses[1]);
}

void loop() {
  int currentCommand = 0;
  if (digitalRead(calibrationJumper) == 0) {
    digitalWrite(statusLEDRed, 1);
    digitalWrite(statusLEDGreen, 0);
    calibrationSettings[0] = pulseIn(commandCapturePin, 1, timeout); //gets time signal is high in us
    digitalWrite(statusLEDRed, 1);
    digitalWrite(statusLEDGreen, 1);
	//delay here
    calibrationSettings[1] = pulseIn(commandCapturePin, 1, timeout); //gets time signal is high in us
    digitalWrite(statusLEDRed, 0);
    digitalWrite(statusLEDGreen, 1);
    EEPROM.write(EEPROMAddresses[0], calibrationSettings[0]);
    EEPROM.write(EEPROMAddresses[1], calibrationSettings[1]);
  } 

  int holder = 0;
  int command = 0;
  holder = pulseIn(commandCapturePin, 1, timeout); //gets time signal is high in us
  if (holder >= calibrationSettings[1]) {
    command = 255;
    digitalWrite(statusLEDGreen, 1);
    digitalWrite(statusLEDRed, 0);
  }
  else if (holder <= calibrationSettings[0]) {
    digitalWrite(statusLEDGreen, 0);
    digitalWrite(statusLEDRed, 1);
    command = 0;
  }
  else {
	holder = holder - calibrationSettings[0];
	int range = calibrationSettings[1] - calibrationSettings[0];
	float scalar = (float)(255/range);
    command = (int)(command*scalar);
    digitalWrite(statusLEDGreen, 1);
    digitalWrite(statusLEDRed, 1);
  }
  
  currentCommand = command;
  analogWrite(pwmPin, currentCommand);

}
