ESP8266MicroTemperature
=======================

  Temperature Monitor using ESP8266 and Arduino Micro with DS18B20

  ESP8366 Micro Temperature
  
  This will connects the Arduino Micro to the ESP8266 Wifi module.  It allows a DS18B20 temperature module to
  post temperature readings to the Sparkfun Data API (http://data.sparkfun.com).  The data is then converted 
  into a chart using Google Chart API.
  
  Build instructions can be found here:
  http://contractorwolf.com/esp8266-wifi-arduino-micro/
  
  Example using the DS18B20:
  http://contractorwolf.com/arduino-temperature-probe/
  (NOTE: above link uses data pin 3, this code uses pin 6)
  
  YOU MUST USE THIS SKETCH TO FIND THE ADDRESS OF YOUR DS18B20
  http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html
  
  Posting to data.sparkfun.com
  https://data.sparkfun.com/
