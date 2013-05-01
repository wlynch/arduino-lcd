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
IPAddress ip(192,168,1,42);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// telnet defaults to port 23
EthernetServer server(23);

// lcd init
LiquidCrystal lcd(7,8,3,4,5,6);

// globals
char message[80]="Welcome!\0";
char password[64]="\0";
unsigned long timeout=10000, currtime=0, oldtime=0;

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
  
  /* Refresh display every timeout/1000 secs */
  refresh_display(0);
  
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client == true) { 
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    buffer=getInput(client);
    
    if (strcmp(buffer,"get") == 0) {
      client.println(message);
    } else if (strcmp(buffer,"exit") == 0) {
      client.println("goodbye");
      client.stop();
    } else if (strcmp(buffer,"refresh") == 0) {
      refresh_display(1);
    } else if (strcmp(buffer,"password") == 0) {
      set_password(client);
    } else if (strcmp(buffer,"help") == 0) {
      help(client);    
    } else {
      char *tok=strtok(buffer," \n\0");
      if (strcmp(tok,"set") == 0) {
        /* Set message */
        if (check_password(client) == 0) {
          tok=strtok(NULL,"\0\n");
          strcpy(message,tok);
          strcat(message,"\0");
          if (refresh_display(1) == 0) {
            client.println("Message set.");
          } else {
            client.println("Error setting message. Please try again.");
          }
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

/* Get complete line of input from the client */
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

/* Refresh LCD display based on message */
int refresh_display(int force) {
  Serial.print(oldtime);
  Serial.print(" ");
  Serial.println(currtime);
  currtime=millis();
  /* Refresh display if timeout is met or millis timer reset */
  if ( (force != 0) ||(currtime-oldtime) >= timeout || (currtime < oldtime)) {
    Serial.print("refreshing! ");
    Serial.print(force);
    Serial.print(" ");
    Serial.print(oldtime);
    Serial.print(" ");
    Serial.print(currtime);
    Serial.print(" ");
    Serial.print(timeout);
    lcd.clear();
    oldtime=currtime;
    if (strcmp(message,"!nextbus") == 0) {
      return nextbus();
    } if (strcmp(message,"!twitter") == 0) {
      return twitter();
    } else {
      lcd.print(message); 
    }
  }
  return 0;
}

/* Simple password check. 
 * If password length is 0, it is treated like there is no password.
 */
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

/* Function to set password. Requires user to input 2x to set */
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

/* Prints nextbus information for Hill Center bus stop to the LCD */
int nextbus(){
  EthernetClient client;
  IPAddress nb_server(165,230,205,70); //wlyn.ch
  // give the Ethernet shield a second to initialize:
  Serial.println("connecting...");
  
  // if you get a connection, report back via serial:
  if (client.connect(nb_server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /nextbus/nextbus-simple.php?s=hill&android=1 HTTP/1.0");
    client.println();
    while (!client.available()) {}
  } else {
    Serial.println("connection failed");
    return 1;
  }
  
  //   if there are incoming bytes available 
  // from the server, read them and print them:
  int i=0, offset=10, row=0, col=0;
  Serial.println(client.available());
  if (client.available()) {
    while (client.connected()) {
      char c=client.read();
      
      if (c == 10){
        if (row >= offset) {
          if (col > 10) {
            row++;
            col=0;
            lcd.setCursor(col,row-offset);
          } else {
            lcd.print(" ");
            col++;
          }
        } else {
          row++;
        }
      } else {
        if (row >= offset){
          if ((col > 19) && (col != ' ')) {
            row++;
            col=0;
            lcd.setCursor(col,row-offset);
          }
          lcd.print(c);
          col++;
        }
      }
      
      i++;
    }
    Serial.println("disconnected");
  }
  client.stop();
  Serial.println("done");
  return 0;
}

int twitter() {
  EthernetClient client;
  String tweet="", currentLine="";
  currentLine.reserve(256);
  tweet.reserve(150);
  boolean readingTweet = false, finishedTweet = false;
  Serial.println("connecting to server...");
  IPAddress t_server(199,16,156,40); 
  int i=0;
  
  if (client.connect(t_server, 80)) {
    Serial.println("making HTTP request...");
    // make HTTP GET request to twitter:
    client.println("GET /1/statuses/user_timeline.xml?screen_name=wlynch92&count=1 HTTP/1.1");
    client.println("HOST: api.twitter.com");
    client.println();
    client.flush();
  }
  while (true) {
    if (client.connected()) {
      if (client.available()) {
        // read incoming bytes:
        char inChar = client.read();
        //Serial.print(inChar);
        //Serial.print((int)inChar);
        
        // add incoming byte to end of line:
        currentLine += inChar; 
  
        // if you get a newline, clear the line:
        if (inChar == '\n') {
          currentLine = "";
        }
        //Serial.println(currentLine); 
        //Serial.println(currentLine.endsWith("<text>"));
        
        // if the current line ends with <text>, it will
        // be followed by the tweet:
        if (currentLine.equals("    <text>")) {
          // tweet is beginning. Clear the tweet string:
          readingTweet = true; 
          tweet = "";
          Serial.println("READING TWEET");
        }
        // if you're currently reading the bytes of a tweet,
        // add them to the tweet String:
        if (readingTweet) {
          if (inChar != '<') {
            if ((inChar != '>') || (i != 0)) {
            tweet += inChar;
            Serial.print(inChar);
            if (i < 80) {
              lcd.setCursor(i%20,i/20);
              lcd.print(inChar);
              i++;
            }
            Serial.print(" [");
            Serial.print(tweet);
            Serial.println("]");
            }
          } 
          else {
            // if you got a "<" char<acter,
            // you've reached the end of the tweet:
            readingTweet = false;
            Serial.println("FINISHED READING TWEET");
            Serial.println(tweet);   
            // close the connection to the server:
            client.stop(); 
            break;
          }
        }
      }
    }  
  }
  // close the connection to the server:
  Serial.println("\n");
  Serial.println("Tweet:");
  Serial.println(tweet);
  return 0;   
}
  
void help(EthernetClient client) {
  client.println("Commands:");
  client.println("\nget");
  client.println("  Get the current message/command");
  client.println("\nset <message>/<command>");
  client.println("  Set a message or command to the display. Commands supported:");
  client.println("    !nextbus : Get nextbus information for the Hill Center bus stop.");
  client.println("\nrefresh");
  client.println("  Manually refresh the display on the lcd. Helpful for updating live information.");
  client.println("\npassword");
  client.println("  Set a password for setting messages on the device. By default, there is no password set.");
}
  
