#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

#define I2C_SLAVE1_ADDRESS 11
#define PAYLOAD_SIZE 2


char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int n = 0;
int go = 3;
int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);


void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println(F("-------------------------------------I am the Master"));
  //delay(1000);

  //Request value of n to slave
  Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
  n = Wire.read();
  Serial.print(F("recieved value : "));
  Serial.println(n);

  //Send value 13 to slave
  Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
  Wire.write(13);
  Serial.print(F("sending value : "));
  Serial.println(13);
  Wire.endTransmission();
  Serial.print(" ");

  //Request value of n to slave after change
  Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
  n = Wire.read();
  Serial.print(F(" new recieved value : "));
  Serial.println(n);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 1 seconds for connection:
  delay(1000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}


void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    }
    else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }


  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            //client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Clica <a href=\"/H\">aqui</a> para o carro andar em FRENTE<br>");
            client.print("Clica <a href=\"/L\">aqui</a> para o carro andar para TRAS<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see the client request
        if (currentLine.indexOf("Y")>0 && currentLine.indexOf("H")==0){













          // different member versions of find in the same order as above:
          int y_pos = currentLine.indexOf("Y");
          int H_pos = currentLine.indexOf("H");
          String y_value = currentLine.substring (y_pos + 1,H_pos-1);

          int x_pos = currentLine.indexOf("X");

          String x_value = currentLine.substring (x_pos + 1, y_pos);

          Serial.println("Recebido: "+currentLine);
          Serial.println("A mandar X: "+x_value);
          Serial.println("A mandar Y: " + y_value);
          
          










          //go = 1;
          //while( go == 1){
          //Request value of n to slave
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);
          //Send value "X" + x_pos + "Y" + y_pos to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          String  msg = "X" + x_value + "Y" + y_value;
          char buffer[8];
          msg.toCharArray(buffer, 8);
          Wire.write(buffer);
          Serial.print(F("sending value : "));
          Serial.println(msg);
          //delay(10000);
          Wire.endTransmission();
          Serial.print(" ");
          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //delay(5000);
          //go=3;
        }


      }
    }


    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}



void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  //Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
