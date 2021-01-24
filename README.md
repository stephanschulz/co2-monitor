# CO2 monitor

Version 2.0 of my homemade CO2 monitor uses the SCD-30 sensor and multiple @adafruit's feather boards. Every 30 seconds data is logged on to an SD-Card. 
With classrooms being a source of COVID-19 outbreaks I wanted to make use of C02 level readings to gauge how well a classroom with 27 kids might be ventilated.

![One full school day](https://github.com/stephanschulz/co2-monitor/blob/main/co2_graph_2021-01-22.png)
![Front](https://github.com/stephanschulz/co2-monitor/blob/main/IMG_19241.jpg)
![Back](https://github.com/stephanschulz/co2-monitor/blob/main/IMG_19251.jpg)

The Arduino for the feather M4 express reads and records the CO2 levels, temperature, relative humidity, current time, and battery voltage.

# Libraries used
- Adafruit_SCD30.h
- Adafruit_SSD1306.h
- SPI.h
- SD.h
- RTClib.h

# Bill Of Materials
1. [Adafruit SCD-30 - NDIR CO2](https://www.adafruit.com/product/4867)
2. [Feather M4 Express](https://www.adafruit.com/product/3857)
3. [Battery - 3.7v 2000mAh](https://www.adafruit.com/product/2011)
4. [FeatherWing 128x32 OLED](https://www.adafruit.com/product/2900)
5. [Qwiic JST SH 4-Pin Cable](https://www.adafruit.com/product/4399)
6. [Adalogger FeatherWing - RTC + SD Add-on](https://www.adafruit.com/product/2922)
7. [8 GB sd card](https://www.adafruit.com/product/1294)
8. [CR1220 3V Coin Cell Battery](https://www.adafruit.com/product/380)
9. [Quad 2x2 FeatherWing Kit](https://www.adafruit.com/product/4253)

# Tutorials used
- https://learn.adafruit.com/adafruit-scd30
- https://learn.adafruit.com/adafruit-oled-featherwing/download
- https://learn.adafruit.com/adafruit-adalogger-featherwing
