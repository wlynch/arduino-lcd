/*
 Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x8B, 0x2F };
IPAddress ip(172,16,6,72);
IPAddress gateway(172,16,6,65);
IPAddress subnet(255,255,255,224);


// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  char *buffer;
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client == true) {
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    buffer=getInput(client);
    server.write(buffer);
    
    free(buffer);
  }
}

char *getInput(EthernetClient client){
  char *buffer=(char *)malloc(sizeof(char)*256);
  char c = client.read();
  int i=0;
  
  while (c != -1){
    Serial.print(c);
    buffer[i]=c;
    i++;
    c=client.read();
  }
  buffer[i]='\0';
  strtok(buffer,"\n");
  Serial.print("[");
  Serial.print(buffer);
  Serial.print("]");
  return buffer;  
}
