
/***************************  Arduino code for Robodyn keypad 4x4 + NodeMcu v3 over MQTT   ************************************/

/*Conections:
 * Device   Physical port   Logic 
 * buttons  a0          
 * buzzer d0
 * 
 */
 
 
 
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h> //
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


ESP8266WiFiMulti wifiMulti;


 /*************************** EDIT THIS AREA START************************************/

//device name
#define DEVICE_NAME     "XXXX_yourdevicename_XXXX"

//mqtt details
#define AIO_SERVER      "XXXX_mqttserver_XXXX"
#define AIO_SERVERPORT  1883                
#define AIO_USERNAME    "XXXX_mqttuser_XXXX"
#define AIO_KEY         "XXXX_mqttpasswd_XXXX"


WiFiClient client;    //do not edit
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); //do not edit


//mqtt publish every 60 seconds

Adafruit_MQTT_Publish mqtt60_uptime = Adafruit_MQTT_Publish(&mqtt,"iot60/uptime/"DEVICE_NAME);
Adafruit_MQTT_Publish mqtt60_rssi = Adafruit_MQTT_Publish(&mqtt,"iot60/rssi/"DEVICE_NAME);

//mqtt publish button from 0 to 16

Adafruit_MQTT_Publish mqtt_button = Adafruit_MQTT_Publish(&mqtt,"iot/button/"DEVICE_NAME);


//NODE MCU PINS

const int buzzerPin = 5; //buzzer to arduino pin d5 is d0 at board
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0


//WIFI NAME AND PASSWORD (ALLOW MULTIPLE WIFIS)

void setup_wifi() {
 
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(DEVICE_NAME);
  wifiMulti.addAP("XXXX_wifi1name_XXXX", "XXXX_wifi1password_XXXX");
  //wifiMulti.addAP("XXXX_wifi2name_XXXX", "XXXX_wifi2password_XXXX.");  //coment this line if you have just one wifi or add more if multiple.

}




 
  /*************************** EDIT THIS AREA END************************************/



/****************************** Sys variables ***************************************/
long previousMillis60 = 0;        // will store last time LED was updated
long interval60 = 60000;           // interval at which to blink (milliseconds)

//DEVICE UPTIME
int uptime = 0;
int dBm = 0;


int sensorValue = 0;  // value read from the pot


String location = DEVICE_NAME;
boolean connectioWasAlive = true;
bool dataok = false;



/*************************** Sketch Code ************************************/

/*************************** MQTT functions ************************************/

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       monitorWiFi();
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}






void monitorWiFi()
{
  if (wifiMulti.run() != WL_CONNECTED)
  {
    if (connectioWasAlive == true)
    {
      connectioWasAlive = false;
      Serial.print("Looking for WiFi ");
    }
    Serial.print(".");
    delay(500);
  }
  else if (connectioWasAlive == false)
  {
    connectioWasAlive = true;
    Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
  }
   printConnectionInfo();
}



void printConnectionInfo(){
    Serial.print("\n\n\n\n"  );  
    Serial.print("IP address"  );  
    Serial.println(WiFi.localIP());
    Serial.print("Wireless status =" + (String)WiFi.status());
    Serial.print("\n\n\n\n"  );
}








int keypadNum ()
{
      sensorValue = analogRead(analogInPin);
      if ( sensorValue>=0 && sensorValue<=100  )      {
            return 0;
      }
      else
      {
            if ( sensorValue>100 && sensorValue<=260  )
            {
                  return 16;
            }
            else
            {
                  if ( sensorValue>260 && sensorValue<=300  ){
                        return 15;
                  } 
                  else
                  {
                        if ( sensorValue>300 && sensorValue<=380  )
                        {
                              return 14;      
                        }
                        else
                        {
                              if ( sensorValue>380 && sensorValue<=430  )
                              {
                                    return 13;      
                              }
                              else
                              {
                                    if ( sensorValue>430 && sensorValue<=470  )
                                    {
                                          return 12;      
                                    }
                                    else
                                    {
                                          if ( sensorValue>470 && sensorValue<=485  )
                                          {
                                          return 11;      
                                          }
                                          else
                                          {
                                                if ( sensorValue>485 && sensorValue<=510  )
                                                {
                                                      return 10;      
                                                }
                                                else
                                                {
                                                      if ( sensorValue>510 && sensorValue<=560  )
                                                      {
                                                            return 9;
                                                      }
                                                      else
                                                      {
                                                            if ( sensorValue>560 && sensorValue<=600  )
                                                            {
                                                                 return 8;      
                                                            }
                                                            else
                                                            {
                                                                  if ( sensorValue>600 && sensorValue<=640  )
                                                                  {
                                                                        return 7;      
                                                                  }
                                                                  else
                                                                  {
                                                                        if ( sensorValue>640 && sensorValue<=680  )
                                                                        {
                                                                              return 6;      
                                                                        }
                                                                        else
                                                                        {
                                                                              if ( sensorValue>680 && sensorValue<=750  )
                                                                              {
                                                                                    return 5;
                                                                              }
                                                                              else
                                                                              {
                                                                                    if ( sensorValue>750 && sensorValue<=850  )
                                                                                    {
                                                                                          return 4;   
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                          if ( sensorValue>850 && sensorValue<=950  )
                                                                                          {
                                                                                                return 3;   
                                                                                          }
                                                                                          else
                                                                                          {
                                                                                                if ( sensorValue>950 && sensorValue<=990  )
                                                                                                {
                                                                                                      return 2;   
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                      if ( sensorValue>990)
                                                                                                      {
                                                                                                            return 1;   
                                                                                                      }
                                                                                                } 
                                                                                          } 
                                                                                    }                                                                                
                                                                              }                                                                          
                                                                        }                                                                    
                                                                  }                                                              
                                                            }                                                        
                                                      }                                                  
                                                }                                            
                                          }                                      
                                    }                                
                              }                          
                        }
                  }
            }      
      }
}


void myButton ( int bstate){
    if (mqtt_button.publish(bstate))
    {
      Serial.print("Data ok:");
      goodSound();
    }
    else
    {
     Serial.print("Data NOT ok:");
      badSound();
    }
    delay(100);
 
    
}

void goodSound ()
{
    tone(buzzerPin, 4000); // Send  sound signal...
    delay(25);        // ...for 1 sec
    noTone(buzzerPin);     // Stop sound...  
    delay(100);
 //   tone(buzzerPin, 4000); // Send  sound signal...
 //   delay(25);        // ...for 1 sec
 //   noTone(buzzerPin);     // Stop sound...  
}

void badSound ()
{
  for (int i = 1000; i>500; i--)
  {
  tone(buzzerPin, i); // Send  sound signal...
  delay(1);        // ...for 1 sec
  }
  noTone(buzzerPin);     // Stop sound...   
}

void setup() {
  // initialize serial communication at 115200
  Serial.begin(115200);


  Serial.print("\n\nSETUP WIFI \r\n" );
  delay(5000);
  setup_wifi();
  monitorWiFi();
  goodSound();
  delay(5000);
  MQTT_connect(); 
 
}




void loop() {

int statusButton = keypadNum();

if ( statusButton == 0 )
{
    delay(100);
}
else
{
  myButton(statusButton);
}



  // print the readings in the Serial Monitor






unsigned long currentMillis60 = millis();

  if(currentMillis60 - previousMillis60 > interval60) {
      previousMillis60 = currentMillis60;   
      //actions60

    MQTT_connect();    


    //piUPTIME
    uptime=uptime+1;
    if (mqtt60_uptime.publish(uptime))
    {
      Serial.println(F("mqtt60_uptime OK"));
    }
    else
    {
      Serial.println(F("mqtt60_uptime FAILED"));
    }

    dBm = WiFi.RSSI();
    uptime=uptime+1;
    //publish RSSI
    if (mqtt60_rssi.publish(dBm))
    {
      Serial.println(F("mqtt60_rssi OK"));
    }
    else
    {
      Serial.println(F("mqtt60_rsssi FAILED"));
    }    

    
  } 

  



}
