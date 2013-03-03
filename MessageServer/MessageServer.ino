/*
 * Message Server
 *
 * Allows for reading and setting of a message over a network through telnet.
 *
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <ctype.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x8B, 0x2F };
IPAddress ip(192,168,1,33);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

// lcd init
LiquidCrystal lcd(7,8,3,4,5,6);

// globals
char message[80]="test123\0";
char password[64]="\0";

void setup() {  
  // start lcd 
  lcd.begin(20,4);
  lcd.print(message);
    
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

void loop() {
  char *buffer;
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client == true) {
    if (alreadyConnected == false) {
      client.println("hello there!");
      alreadyConnected=true;
    }
    
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    buffer=getInput(client);
    
    if (strcmp(buffer,"get") == 0) {
      client.println(message);
    } else if (strcmp(buffer,"exit") == 0) {
      client.println("goodbye");
      client.stop();
    } else if (strcmp(buffer,"nextbus") == 0) {
      nextbus();
    } else if (strcmp(buffer,"password") == 0) {
      set_password(client);
    } else {
      char *tok=strtok(buffer," \n\0");
      if (strcmp(tok,"set") == 0) {
        /* Set message */
        if (check_password(client) == 0) {
          tok=strtok(NULL,"\0\n");
          strcpy(message,tok);
          strcat(message,"\0");
          client.println("message set");
          lcd.begin(20,4);
          lcd.clear();
          lcd.print(message);
        } 
      } else {
        if (strcmp(buffer,"") != 0) {
          client.print("Unknown Command: [");
          client.print(buffer);
          client.print("]\n");
        } else {
          client.println();
        }
      } 
    }
    free(buffer);
    client.print("$> ");
  }
}

char *getInput(EthernetClient client) {
  char *buffer=(char *)calloc(256,sizeof(char));
  char c = client.read();
  int i=0;
  
  while ((c > 0) && (c != '\n') && (i <= 84)) {
    buffer[i]=c;
    i++;
    c=client.read();
  }
  /* Remove trailing \n and flush the rest of the client */
  if (i > 0)
    buffer[i-1]='\0';
  client.flush();
  return buffer;  
}

int check_password(EthernetClient client) {
  int i,retval;
  char *attempt;
  /* See if password is set */
  if (strlen(password) == 0) {
    return 0;
  }

  /* Password check */
  for(i=0; i<3; i++){
    client.print("Password: ");
    while(!client.available()) {} // Wait for user to input password
    attempt=getInput(client);
    retval=strcmp(attempt,password);
    free(attempt);
    if (retval == 0) {
      return 0;
    } else {
      client.println("Authentication failed");
    }
  }
  return 1;
}

void set_password(EthernetClient client) {
  char *p1,*p2;
  
  if (check_password(client) == 0) {
    client.print("Enter new password: ");
    while (!client.available()) {}
    p1=getInput(client);
    client.print("Enter new password again: ");
    while (!client.available()) {}
    p2=getInput(client);
    if (strcmp(p1,p2) == 0) {
      strcpy(password,p1);
      strcat(password,"\0");
      client.println("Password successfully set");
    } else {
      client.println("Passwords do not match");
    }
    free(p1);
    free(p2);
  } 
}

int nextbus(EthernetClient main){
  EthernetClient client;
  IPAddress nb_server(71,19,144,28); // vverma.net
  
  // give the Ethernet shield a second to initialize:
  Serial.println("connecting...");
  
  // if you get a connection, report back via serial:
  if (client.connect(nb_server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /nextbus/nextbus.php?s=hill&android=1 HTTP/1.0");
    client.println();
    while (!client.available()) {}
  } else {
    Serial.println("connection failed");
    return 1;
  }
  
  //   if there are incoming bytes available 
  // from the server, read them and print them:
  char message[512]="\0";
  int i=0, offset=10, row=0;
  Serial.println(client.available());
  if (client.available()) {
    while (client.connected()) {
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
    Serial.println("disconnected");
  }
  client.stop();
  Serial.println("done");
  return 0;
}
  
