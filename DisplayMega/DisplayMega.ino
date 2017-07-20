#include <UTFT.h>  // Arduino LCD library


UTFT myGLCD(ILI9481, 38, 39, 40, 41);

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];


// Variablen
byte MessungZ[500];
byte MessungV[500];

void setup() {

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);
  
  // put your setup code here, to run once:
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Warte auf Trigger-Signal", CENTER, 1);
  //Serial.println("waiting for trigger");
}

void loop() {
  // put your main code here, to run repeatedly:

}

//  MessungV[1] = voltage * 100;;
//  for (int x = 1; x < 478; x++)
//  {
//    MessungZ[x] = MessungV[x];
//    //MessungZ[x+1] = MessungZ[x];
//    MessungV[x + 1] = MessungV[x];
//
//    myGLCD.setColor(0, 0, 0);
//    myGLCD.drawLine(x, MessungZ[x], x + 1, MessungZ[x + 1]);
//    myGLCD.setColor(255, 255, 255);
//    myGLCD.drawLine(x, MessungV[x], x + 1, MessungV[x + 1]);
//  }

void Darstellung() {

  //############ zeichne Raster #########################################################
  myGLCD.setColor(0, 220, 0);

  //############ vertical Lines #########################################################
  myGLCD.drawLine(0, 0, 0, 319);
  myGLCD.drawLine(60, 0, 60, 319);
  myGLCD.drawLine(120, 0, 120, 319);
  myGLCD.drawLine(180, 0, 180, 319);
  myGLCD.drawLine(240, 0, 240, 319);
  myGLCD.drawLine(300, 0, 300, 319);
  myGLCD.drawLine(360, 0, 360, 319);
  myGLCD.drawLine(479, 0, 479, 319);


  //############ horizontal Lines #######################################################
  myGLCD.drawLine(0, 0, 479, 0);
  myGLCD.drawLine(0, 60, 479, 60);
  myGLCD.drawLine(0, 120, 479, 120);
  myGLCD.drawLine(0, 180, 479, 180);
  myGLCD.drawLine(0, 240, 479, 240);
  myGLCD.drawLine(0, 300, 479, 300);
}

