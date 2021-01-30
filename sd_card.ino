// Set the pins used
#define cardSelect 10 // 15 for huzzah
File logfile;

bool setup_sd() {


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    return false;
  }


  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.println("Ready!");
  return true;
}


void logTo_sd(float batV_value, float co2_value, float temp, float humidity) {

  DateTime now = rtc.now();

  //  String todayString = "DATA-";
  //  todayString += String(now.year(), DEC);
  //  todayString += '-';
  //  todayString += String(now.month(), DEC);
  //  todayString += '-';
  //  todayString += String(now.day(), DEC);

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

  //  Serial.println(dataString);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //  File dataFile = SD.open("datalog.TXT", FILE_WRITE);

  //  todayString += ".TXT";


  //  char filename[] = "DATA-2021-01-22.TXT";
  //  char filename[] = "20210122.txt"
  char filename[] = "00000000.txt";;
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
