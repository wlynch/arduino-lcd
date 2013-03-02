/*
 * Message Server
 *
 * Allows for reading and setting of a message over a network through telnet.
 *
 */

#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <ctype.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x8B, 0x2F };
IPAddress ip(192,168,1,21);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

// globals
char message[80];

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
  message[0]='\0';
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
    
    if (strcmp(buffer,"get")==0){
      server.println(message);
    } else {
      char *tok=strtok(buffer," \n\0");
      if (strcmp(tok,"set")==0){
        tok=strtok(NULL,"\0\n");
        strcpy(message,tok);
        strcat(message,"\0");
        server.println("message set");
      } else {
        if (strcmp(buffer,"")!=0){
          server.print("Unknown Command: [");
          server.print(buffer);
          server.print("]\n");
        }
      } 
    }
    free(buffer);
    server.print("$> ");
  }
}

char *getInput(EthernetClient client){
  char *buffer=(char *)calloc(256,sizeof(char));
  char c = client.read();
  int i=0;
  
  while ((c > 0) && (c != '\n')) {
    buffer[i]=c;
    i++;
    c=client.read();
  }
  /* Remove trailing \n and flush the rest of the client */
  buffer[i-1]='\0';
  client.flush();
  return buffer;  
}
