  /*
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
  
  */
  
  #include <OneWire.h>
  #include <DallasTemperature.h>
  
  //keys for data.sparkfun.com api (dont use quotes here)
  #define PUBLIC_KEY your_sparkfun_public_key
  #define PRIVATE_KEY your_sparkfun_private_key
  
  //wifi connection parameters (dont use quotes here)
  #define WIFI_NETWORK your_wifi_network_name
  #define WIFI_PASSWORD your_wifi_password
  
  //period between posts, set at 60 seconds
  #define DELAY_PERIOD 60000
  
  //pin for the DS18B20 temperature module
  #define TEMPERATURE_PIN 6
  
  
  OneWire oneWire(TEMPERATURE_PIN);
  DallasTemperature sensors(&oneWire);
  
  // use http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html 
  // to find the address of your DS18B20, it will be different then this
  DeviceAddress thermometer = { 0x28, 0xB0, 0x0C, 0x37, 0x04, 0x00, 0x00, 0x68 };
  
  
  char serialbuffer[100];//serial buffer for request command
  long nextTime;//next time in millis that the temperature read will fire
  
  void setup()
  {
    Serial1.begin(115200);//connection to ESP8266
    Serial.begin(115200); //serial debug
    
    //set mode needed for new boards
    Serial1.println("AT+RST");
    Serial1.println("AT+CWMODE=1");
    delay(500);//delay after mode change
    Serial1.println("AT+RST");
    
    //connect to wifi network
    Serial1.println("AT+CWJAP=\"WIFI_NETWORK\",\"WIFI_PASSWORD\"");
    
    sensors.begin();
    
    nextTime = millis();//reset the timer
  }
  
  void loop()
  {
    //output everything from ESP8266 to the Arduino Micro Serial output
    while (Serial1.available() > 0) {
      Serial.write(Serial1.read());
    }
    
    //to send commands to the ESP8266 from serial monitor (ex. check IP addr)
    if (Serial.available() > 0) {
       //read from serial monitor until terminating character
       int len = Serial.readBytesUntil('\n', serialbuffer, sizeof(serialbuffer));
    
       //trim buffer to length of the actual message
       String message = String(serialbuffer).substring(0,len-1);
       
       Serial.println("message: " + message);
   
       //check to see if the incoming serial message is an AT command
       if(message.substring(0,2)=="AT"){
         //make command request
         Serial.println("COMMAND REQUEST");
         Serial1.println(message); 
       }//if not AT command, ignore
    }
    
    //wait for timer to expire
    if(nextTime<millis()){
      Serial.print("timer reset: ");
      Serial.println(nextTime);
  
      //reset timer
      nextTime = millis() + DELAY_PERIOD;
      
      //get temp
      sensors.requestTemperatures();
      //send temp
      SendTempData(sensors.getTempF(thermometer));
  
    }
  }
  
  
  //web request needs to be sent without the http for now, https still needs some working
  void SendTempData(float temperature){
     char temp[10];
    
     Serial.print("temp: ");     
     Serial.println(temperature);
    
     dtostrf(temperature,1,2,temp);
   
     //create start command
     String startcommand = "AT+CIPSTART=\"TCP\",\"data.sparkfun.com\", 80";
     
     Serial1.println(startcommand);
     Serial.println(startcommand);
     
     //test for a start error
     if(Serial1.find("Error")){
       Serial.println("error on start");
       return;
     }
     
     //create the request command
     String sendcommand = "GET /input/PUBLIC_KEY?private_key=PRIVATE_KEY&temp="; 
     sendcommand.concat(String(temp));
     sendcommand.concat(" HTTP/1.0\r\n\r\n");
     
     
     //send to ESP8266
     Serial1.print("AT+CIPSEND=");
     Serial1.println(sendcommand.length());
     
     //display command in serial monitor
     Serial.print("AT+CIPSEND=");
     Serial.println(sendcommand.length());
     
     if(Serial1.find(">"))
     {
       Serial.println(">");
     }else
     {
       Serial1.println("AT+CIPCLOSE");
       Serial.println("connect timeout");
       delay(1000);
       return;
     }
     
     //Serial.print(sendcommand);
     Serial1.print(sendcommand); 
  }
  









