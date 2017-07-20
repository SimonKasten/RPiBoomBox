



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

