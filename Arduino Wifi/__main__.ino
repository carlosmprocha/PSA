#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

#define I2C_SLAVE1_ADDRESS 11
#define PAYLOAD_SIZE 2

#define Pino_Ponta 1 // Pino do Servo do Manipulador que está na ponta
#define Pino_Meio 2 // Pino do Servo do Manipulador que está no meio
#define Pino_Base 3 // Pino do Servo do Manipulador que está na base
#define Pino_Rele 4 // Pino do Eletroíman 

int n = 0;
int go = 3;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

Servo Ponta;
Servo Meio;
Servo Base;

int posvel = 100; // velocidade positiva
int negvel = 80; // velocidade negativa
int zerovel = 90; // velocidade nula


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

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

  Ponta.attach(Pino_Ponta);
  Meio.attach(Pino_Meio);
  Base.attach(Pino_Base);

  pinMode(Pino_Ponta, OUTPUT);
  pinMode(Pino_Meio, OUTPUT);
  pinMode(Pino_Base, OUTPUT);
  pinMode(Pino_Rele, OUTPUT);

}

void loop() {
  //delay(100);



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
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Clica <a href=\"/H\">aqui</a> para o carro andar em FRENTE<br>");
            client.print("Clica <a href=\"/L\">aqui</a> para o carro andar para TRAS<br>");
            client.print("Clica <a href=\"/S\">aqui</a> para o carro andar para LADO NENHUM<br>");
            client.print("Clica <a href=\"/E\">aqui</a> para o carro andar para ESQUERDA<br>");
            client.print("Clica <a href=\"/D\">aqui</a> para o carro andar para DIREITA<br>");

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

        ///////// PONTA //////////
        if (currentLine.endsWith("GET /CE")) {
          Ponta.write(posvel);
        }
        elseif(currentLine.endsWith("GET /CD")) {
          Ponta.write(negvel);
        }
        elseif(currentLine.endsWith("GET /CS")) {
          Ponta.write(zerovel);
        }

        ////////// MEIO /////////
        if (currentLine.endsWith("GET /ME")) {
          Meio.write(posvel);
        }
        elseif (currentLine.endsWith("GET /MD")) {
          Meio.write(negvel);
        }
        elseif (currentLine.endsWith("GET /MS")) {
          Meio.write(zerovel);
        }

        ////////// Base /////////
        if (currentLine.endsWith("GET /BE")) {
          Base.write(posvel);
        }
        elseif (currentLine.endsWith("GET /BD")) {
          Base.write(negvel);
        }
        elseif (currentLine.endsWith("GET /BS")) {
          Base.write(zerovel);
        }

        //////// Eletroiman /////////
        if (currentLine.endsWith("GET /IU")) {
          digitalWrite(Pino_Rele, 1);
        }
        elseif(currentLine.endsWith("GET /IZ")) {
          digitalWrite(Pino_Rele, 0);
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 1 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(1);
          Serial.print(F("sending value : "));
          Serial.println(1);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }

        else if (currentLine.endsWith("GET /NE")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 2 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(2);
          Serial.print(F("sending value : "));
          Serial.println(2);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /D")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 3 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(3);
          Serial.print(F("sending value : "));
          Serial.println(3);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /SE")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 4 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(4);
          Serial.print(F("sending value : "));
          Serial.println(4);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /L")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 5 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(5);
          Serial.print(F("sending value : "));
          Serial.println(5);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /SO")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 6 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(6);
          Serial.print(F("sending value : "));
          Serial.println(6);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /E")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 7 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(7);
          Serial.print(F("sending value : "));
          Serial.println(7);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /NO")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 8 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(8);
          Serial.print(F("sending value : "));
          Serial.println(8);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /P")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 9 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(9);
          Serial.print(F("sending value : "));
          Serial.println(9);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }
        else if (currentLine.endsWith("GET /AUTO")) {
          //go = 1;
          //while( go == 1){
          //Request value of n to slave

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F("recieved value : "));
          Serial.println(n);

          //Send value 10 to slave
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          Wire.write(10);
          Serial.print(F("sending value : "));
          Serial.println(9);
          Wire.endTransmission();
          Serial.print(" ");

          //Request value of n to slave after change
          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Serial.print(F(" new recieved value : "));
          Serial.println(n);
          //go=3;
        }

        else if (currentLine.indexOf("HTTP/1.1") > 0 ) { // && currentLine.indexOf("H")==0

          // different member versions of find in the same order as above:
          int y_pos = currentLine.indexOf("Y");
          int H_pos = currentLine.indexOf("H");
          String y_value = currentLine.substring (y_pos + 1, H_pos - 1);

          int x_pos = currentLine.indexOf("X");

          String x_value = currentLine.substring (x_pos + 1, y_pos);

          Serial.println("Recebido: " + currentLine);
          Serial.println("A mandar X: " + x_value);
          Serial.println("A mandar Y: " + y_value);


          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          Wire.beginTransmission(I2C_SLAVE1_ADDRESS);
          String  msg = "X" + x_value + "Y" + y_value;
          char buffer[9];
          msg.toCharArray(buffer, 9);
          Wire.write(buffer, sizeof(buffer));
          Serial.print(F("sending value : "));
          Serial.println(msg);
          Wire.endTransmission();
          Serial.print(" ");

          Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
          n = Wire.read();
          // Serial.print(F(" new recieved value : "));
          // Serial.println(n);
          //go=3;
          client.stop();
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
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
