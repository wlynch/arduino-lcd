/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 modified 9 Apr 2012
 by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x8B, 0x2F };
IPAddress ip(192,168,1,23);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

IPAddress server(71,19,144,28); // vverma.net
//IPAddress server(165,230,205,70); //wlyn.ch

LiquidCrystal lcd(7,8,3,4,5,6);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

int row=0;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  lcd.begin(20,4);
  
  Ethernet.begin(mac,ip);

  // give the Ethernet shield a second to initialize:
  Serial.println("connecting...");
  
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /nextbus/nextbus.php?s=hill&android=1 HTTP/1.0");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{
  //   if there are incoming bytes available 
  // from the server, read them and print them:
  char message[512]="\0";
  int i=0, offset=10;
  if (client.available()) {
    char c=client.read();
    Serial.print("[");
    Serial.print(c);
    Serial.print("] ");
    Serial.print((int)c);
    Serial.print(" ");
    Serial.print(row);
    Serial.print(" ");
    Serial.println(offset);
    if (c == 10){
      row++;
      if (row>=offset) {
        lcd.setCursor(0,row-offset);
      }
    } else {
      if (row >= offset){
        lcd.print(c);
      }
    }
    
    message[i]=c;
    i++;
  }
  message[i]='\0';
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("Message:");
    Serial.println(message);
    Serial.println("disconnecting.");
    client.stop();
    // do nothing forevermore:
    for(;;)
      ;
  }
}

char *getInput(EthernetClient client) {
  char *buffer=(char *)calloc(256,sizeof(char));
  char c = client.read();
  int i=0;
  
  while ((c > 0)) {
    buffer[i]=c;
    i++;
    c=client.read();
  }
  /* Remove trailing \n and flush the rest of the client */
  if (i > 0)
    buffer[i-1]='\0';
  return buffer;  
}
