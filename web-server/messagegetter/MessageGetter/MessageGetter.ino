#include <string.h>
#include <LiquidCrystal.h>



LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
char currentCharacter = 0;
String content = "";


void setup() {
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  /*pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  analogWrite(2, 0);
  analogWrite(3, 0);
  analogWrite(4, 0);*/
}

void loop() {
  

  if(Serial.available()) {
    content = "";
    while(Serial.available()) {
      currentCharacter = Serial.read();
      content.concat(currentCharacter);
      delay(10);
    }
  }
  
  if(!content.equals("")) {
    lcd.clear();
    lcd.print(content);  
  }
  
  
   content = "";
  
  
}
