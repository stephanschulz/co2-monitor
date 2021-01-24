// Set the pins used
#define cardSelect 10 // 15 for huzzah
File logfile;

void setup_sd() {


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }


  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.println("Ready!");
}


void logTo_sd(float batV_value, float co2_value, float temp, float humidity) {

  DateTime now = rtc.now();
  
  String dataString = "";

  dataString += String(now.year(), DEC);
  dataString += '/';
  dataString += String(now.month(), DEC);
  dataString += '/';
  dataString += String(now.day(), DEC);
  dataString += ",";
  dataString += daysOfTheWeek[now.dayOfTheWeek()];
  dataString += ",";
  dataString += String(now.hour(), DEC);
  dataString += ':';
  dataString += String(now.minute(), DEC);
  dataString += ':';
  dataString += String(now.second(), DEC);
  //  dataString += '\n';

  dataString += ",";
  dataString += String(co2_value, 2);
  dataString += ",";
  dataString += String(temp, 2);
  dataString += ",";
  dataString += String(humidity, 2);
  dataString += ",";
  dataString += String(batV_value, 2);

  //  char filename[] = "20210122.txt"
  char filename[] = "00000000.txt"; // don't make the file name longer than 12 chars
  sprintf(filename, "%02d%02d%02d.txt", now.year(), now.month(), now.day());

  if (SD.exists(filename) == false) {
    Serial.print("exists == false ");
    Serial.println(filename);
  }

  File dataFile = SD.open(filename, FILE_WRITE);
  // File dataFile = SD.open("myData.txt", FILE_WRITE);


  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //    Serial.println(todayString);
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("error opening file: ");
    Serial.println(filename);
  }

  delay(100);
}

// blink out an error code
void error(uint8_t errno) {
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i = errno; i < 10; i++) {
      delay(200);
    }
  }
}
