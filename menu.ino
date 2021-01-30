
//boolean bSetupMode = false;
unsigned long setupTimer;
int setupStage = -1;

bool checkMenu() {

  //    if (setupStage == -1) {
  bool somethingPressed = false;
  if (digitalRead(BUTTON_C) == false) {
    somethingPressed = true;
    if (millis() - setupTimer > 3000) {
      setupTimer = millis();
      setupStage = 1;
      scd30.forceRecalibrationWithReference(400);
      //          display.clearDisplay();
      //          display.setTextSize(1);
      //          display.setCursor(0, 0);
      //          display.print("setup #");
      //          display.setCursor(500, 0);
      //          display.print(setupStage);
      //          display.display();
      //        }
    }
    //

  }



  if (setupStage == 1) {
    unsigned long temp_diff = millis() - setupTimer;
    if (temp_diff > 3000) {
      somethingPressed = false;
      setupStage = -1;
    } else {
      somethingPressed = true;
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.print("setting CO2");
      display.setCursor(0, 10);
      display.print("to 400 ppm");

      //      display.setCursor(60, 0);
      //      display.print(setupStage);
      //      display.setCursor(0, 0);
      //      display.print("setup #");
      //      display.setCursor(60, 0);
      //      display.print(setupStage);
      display.setCursor(0, 20);
      display.print( (3000 - temp_diff) );
      display.display();
    }
  }

  if (somethingPressed == false) {
    setupTimer = millis();
  }

  if (setupStage != -1 || somethingPressed == true) {
    return true;
  } else {
    return false;
  }
  //  display.display();
}
