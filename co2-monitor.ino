// A simple eCO2 meter using the Adafruit SCD30 breakout and the Adafruit 128x32 OLEDs
#include <Adafruit_SCD30.h>
#include <Adafruit_SSD1306.h>

#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
RTC_PCF8523 rtc;

#include <Adafruit_NeoPixel.h>
#define NEO_PIN 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, NEO_PIN, NEO_GRB + NEO_KHZ800);

float verNumber = 2.2;
int bad_co2 = 1100;

Adafruit_SCD30  scd30;

//---OLED display
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5


#define VBATPIN A6 //A13 //14 

boolean initDone;
unsigned long initTimer;



const int ledPin =  13;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

unsigned long writeInterval = 30000;
unsigned long writeTimer;

unsigned long checkInterval = 2000;
unsigned long checkTimer;

float cur_co2 = 0;
float cur_humidity = 0;
float cur_temperature = 0;
float cur_measuredvbat = 0;
boolean vbat_toggleShow;

unsigned long sdCard_checkTimer;
bool sdCard_found;

void setup(void) {
  Serial.begin(115200);
  //  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  delay(500);



  Serial.println("SCD30 OLED eCO2 meter!");
  // Try to initialize!
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("SCD30 Found!");


  //---OLED display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);



  if (!scd30.setMeasurementInterval(2)) {
    Serial.println("Failed to set measurement interval");
    while (1) {
      delay(10);
    }
  }


  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'




  display.display();
  delay(500); // Pause for half second

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);

  sdCard_found = setup_sd();
  setup_rtc();

  initDone = false;
  initTimer = millis();
  checkTimer = millis();
  writeTimer = millis();

  Serial.print("setup done");
}


void loop() {

  //------print initial info
  if ( initDone == false) {

    if (millis() - initTimer > 5000) {
      initDone = true;

      Serial.print("version ");
      Serial.println(verNumber);


      if (scd30.selfCalibrationEnabled()) {
        Serial.print("Self calibration enabled");
      } else {
        Serial.print("Self calibration disabled");
      }
      Serial.println();

      Serial.print("Measurement Interval: ");
      Serial.print(scd30.getMeasurementInterval());
      Serial.println(" seconds");

      Serial.print("Forced Recalibration reference: ");
      Serial.print(scd30.getForcedCalibrationReference());
      Serial.println(" ppm");

      Serial.print("Altitude offset: ");
      Serial.print(scd30.getAltitudeOffset());
      Serial.println(" meters");

      Serial.print("Temperature offset: ");
      Serial.print((float)scd30.getTemperatureOffset() / 100.0);
      Serial.println(" degrees C");

    } else {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.print("version:");
      display.setCursor(0, 15);
      display.println(verNumber, 1);
      display.display();

      strip.setPixelColor(0, strip.Color(0, 0, 255));
      strip.show();
    }
  } else {

    //---check every 5 seconds if SD card is present
    if (millis() - sdCard_checkTimer > 5000) {
      sdCard_checkTimer = millis();
      sdCard_found = setup_sd();
    }

    //--check SCD30 sensor every n seconds
    if (millis() - checkTimer > checkInterval) {
      checkTimer = millis();

      //        loop_rtc();
      //    loop_sd();

      display.clearDisplay();
      //    display.setCursor(0, 0);

      display.setTextSize(1);
      //    display.setCursor(0, 0);
      //    display.print("v:");
      //    display.setCursor(10, 0);
      //    display.println(verNumber, 1);

      DateTime now = rtc.now();
      display.setCursor(0, 0);
      display.print(now.timestamp(DateTime::TIMESTAMP_TIME));

      if (scd30.dataReady()) {

        Serial.println("Data available!");

        if (!scd30.read()) {
          Serial.println("Error reading sensor data");
          display.println("READ ERR");
          display.display();
          return;
        }

        cur_temperature = scd30.temperature;
        cur_co2 = scd30.eCO2;
        cur_humidity = scd30.relative_humidity;


        Serial.print("eCO2: ");
        Serial.print(cur_co2, 3);
        Serial.println(" ppm");
        Serial.println("");


        cur_measuredvbat = analogRead(VBATPIN);
        cur_measuredvbat *= 2;    // we divided by 2, so multiply back
        cur_measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
        cur_measuredvbat /= 1024; // convert to voltage

        if (cur_co2 > 0 && millis() - writeTimer > writeInterval) {
          writeTimer = millis();
          logTo_sd(cur_measuredvbat, cur_co2, cur_temperature, cur_humidity);
        }

      } //end if (scd30.dataReady())


      if (sdCard_found == true) {

        if (cur_measuredvbat <= 3.3 ) {
          if (vbat_toggleShow == true) {
            display.setCursor(55, 0);
            display.println(cur_measuredvbat);
            display.setCursor(80, 0);
            display.print("V");
          }
          vbat_toggleShow = !vbat_toggleShow;
        } else {
          display.setCursor(55, 0);
          display.println(cur_humidity, 1);
          display.setCursor(80, 0);
          display.print("%");
        }

        display.setCursor(95, 0);
        display.print(cur_temperature, 1);
        display.setCursor(120, 0);
        display.print("C");

      } else {
        display.setCursor(55, 0);
        display.println("NO SD-CARD!");
      }

      display.setCursor(100, 10);
      display.println(" CO2");
      display.setCursor(100, 20);
      display.println(" ppm");

      display.setTextSize(2);
      display.setCursor(15, 15);
      display.print(cur_co2, 1);
      display.display();

    } //end if (millis() - writeTimer > writeInterval)

    if (cur_co2 > bad_co2) {
      fadeNeoPixel(true);
    } else {
      fadeNeoPixel(false);
    }

  }//else end if initDone == false
  blink_it();

  //  delay(100);
}

void blink_it()
{

  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }

}

void fadeNeoPixel(bool _doIt) {
  if (_doIt) {
    int temp_percent = map(millis() % 10000, 0, 10000, -255, 255);
    //    Serial.print("temp_percent ");
    //    Serial.print(temp_percent);
    //    Serial.println();

    strip.setPixelColor(0, strip.Color(abs(temp_percent), 0, 0));
    strip.show();
  } else {
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
  }

}
