#include <Stepper.h>
#include <EEPROM.h>

#define STEPS 400

int num = 0;
int counter = 0;
bool check = false;
long start = 0;

int max_time = 20; // Half of a day (12h) 43200

Stepper stepper(STEPS, 11, 8, 10, 9);

void setup() {
  stepper.setSpeed(220);
  
  Serial.begin(9600);
  Serial.println("Serial communication started.");
  counter = readIntFromEEPROM(45);
  Serial.print("Moves number is: ");
  Serial.println(counter);

  start = millis() / 1000;
}

void loop() {
  stepper.step((!check)?-1:1);
  num += (!check)?1:-1;

  if((num >= 220 && !check) || (num <= 0 && check)){
    check = !check;
    counter++;
    delay(3000);
  }

  if(millis() / 1000 - start > max_time){
    writeIntIntoEEPROM(45, counter);
    start = millis() / 1000;
    Serial.println("Writing data to the EEPROM...");
  }
}

void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}