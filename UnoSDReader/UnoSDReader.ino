#include <PCF8523.h>
#include <Wire.h>
#include <SPI.h>
#include "SdFat.h"
#include <string.h>

SdFat sd;
SdFile file;

PCF8523 RTC; // define the Real Time Clock object

// Default SD chip select is SS pin
const uint8_t chipSelect = 10;

// create Serial stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------

String str;
String dataString = "";
unsigned long StartSample = 0;
unsigned long EndSample = 0;
int Trigger = 0;
unsigned long SampleTime = 0;
float voltage = 0.0;
int sensor = 0;
int mapping = 0;
unsigned long Dauer = 0;
int BondCounter = 1;
int ChipCounter = 1;

int Merk = 0;
int Merkalt = 0;

//------------------------------------------------------------------------------
void setup(void) {
  pinMode(3, INPUT);
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);

  attachInterrupt(digitalPinToInterrupt(3), Switches, RISING);

  Serial.begin(250000);
  // Wait for USB Serial
  while (!Serial) {
    SysCall::yield();
  }

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  // connect to RTC
  Wire.begin();
  if (!RTC.begin()) {
    Serial.println("RTC failed");
  }

  DateTime nowName;
  nowName = RTC.now();

  String tag = String(nowName.day());
  String monat = String(nowName.month());
  String jahr = String(nowName.year());
  String stunde = String(nowName.hour());
  String minute = String(nowName.minute());
  String sekunde = String(nowName.second());
  String filestring;
  filestring.concat("Messung_");
  filestring.concat(tag);
  filestring.concat("_");
  filestring.concat(monat);
  filestring.concat("_");
  filestring.concat(jahr);
  filestring.concat("_");
  filestring.concat(stunde);
  filestring.concat("_");
  filestring.concat(minute);
  filestring.concat("_");
  filestring.concat(sekunde);
  //filestring.concat(".CSV");

  char filename[filestring.length() + 1];
  filestring.toCharArray(filename, sizeof(filename));

  sd.mkdir(filename);
  sd.chdir(filename);

  cout << F("\nList of files on the SD.\n");
  sd.ls("/", LS_R);
  cout << F("\n1 geschreibert vong ordner Order \n");
}



void loop() {

  //Darstellung();

  if (Merkalt == 0) {
    Merkalt = 1;
    
    DateTime nowName;
    nowName = RTC.now();
    String stunde = String(nowName.hour());
    String minute = String(nowName.minute());
    String sekunde = String(nowName.second());
    String filestring;
    filestring.concat("Messung");
    filestring.concat("_");
    filestring.concat(stunde);
    filestring.concat("h_");
    filestring.concat(minute);
    filestring.concat("m_");
    filestring.concat(sekunde);
    filestring.concat("s.CSV");

    char filename[filestring.length() + 1];
    filestring.toCharArray(filename, sizeof(filename));
    Serial.print("Writing to:  ");
    Serial.println(filename);
    // remove file if it exists
    sd.remove(filename);
    // create a new file with default timestamps
    if (!file.open(filename, O_CREAT | O_WRITE)) {
      error("open of file failed");
    }

    // set creation date time
    if (!file.timestamp(T_CREATE, nowName.year(), nowName.month(), nowName.day(), nowName.hour(), nowName.minute(), nowName.second())) {
      error("set create time failed");
    }
    // set write/modification date time
    if (!file.timestamp(T_WRITE, nowName.year(), nowName.month(), nowName.day(), nowName.hour(), nowName.minute(), nowName.second())) {
      error("set write time failed");
    }
    // set access date
    if (!file.timestamp(T_ACCESS, nowName.year(), nowName.month(), nowName.day(), nowName.hour(), nowName.minute(), nowName.second())) {
      error("set access time failed");
    }

    file.println("This is the first cell in the first column.\n");
    file.println("Chip;Bond;Voltage;Time");
  }

  String stringUTFT = "Chip: " + String(ChipCounter) + " Bond: " + String(BondCounter);
  while (digitalRead(3) == LOW)
  {
    dataString = "AUS ; 0 ; 0 ; " + String(micros() - EndSample);
  }
  Serial.println(dataString);
  file.println(dataString);

  StartSample = micros();
  while (digitalRead(3) == HIGH)
  {
    sensor = analogRead(A0);
    voltage = sensor * (5 / 1023.0);
    dataString = String(ChipCounter) + " ; " + String(BondCounter) + " ; " + String(voltage) + " ; " + String(micros() - StartSample);

    Serial.println(dataString);
    file.println(dataString);
    EndSample = micros();
    Merk = 1;
  }


  if (Merk == 1) {
    BondCounter++;
    if (BondCounter > 7) {
      BondCounter = 1;
      ChipCounter++;

      if (ChipCounter > 2) {
        ChipCounter = 1;
        file.close();
        Merkalt = 0;
      }
    }
    Merk = 0;
  }
}




void Switches() { }
//
//  static unsigned long last_interrupt_time = 0;
//  unsigned long interrupt_time = micros();
//  if (interrupt_time - last_interrupt_time > 200)
//  {
//  }
//  last_interrupt_time = interrupt_time;




