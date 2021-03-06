Arduino Display
===============
###Billy Lynch & Robert Fidler

Overview
--------
This project uses an arduino and an LCD character display in order to display messages specified by a remote client.

Components
----------
- [Arduino UNO](http://arduino.cc/en/Main/arduinoBoardUno)
- [Arduino Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield)
- [LCD Character Display](http://adafruit.com/products/499)

Method
------
For this project, we decided to take 2 different approaches to address how the arduino should interact with a user. In one approach, the arduino acts as an interactive server, reading from and writting to a client. In the other, we allow a supplementary web server to handle user interaction and then fowarded parsed requests to the arduino. You can find these different implementations here:
- [Arduino server](https://github.com/wlynch92/arduino-lcd/tree/master/arduino-server)
- [Web server](https://github.com/wlynch92/arduino-lcd/tree/master/web-server)

Results
-------
![Demo](https://pbs.twimg.com/media/BEZQvwJCUAA54Cu.jpg)

Final Thoughts
--------------
Future Work, Extensions: 
- more use of third-party api's
- casing design
- WiFi
- Power arduino with a battery

Pitfalls and Snags: 
- Rutgers' network problems

Recommendations: 
- test at home
