arduino-server
==============

In this implementation, the arduino acts as a server for users to connect to. Users can communicate with the device by sending bytes to the device. The easiest way to do this is to use a simplistic client such as telnet.

The arduino also acts as a client to connect to services such as nextbus, twitter, and the Rutgers Schedule of Classes.

The server supports the following commands:
- get : get current message on the screen
- refresh : force an update of the last set command
- help : print help documentation
- password : set password to set messages
- set : set message on display
  - !nextbus : Display nextbus information for Hill Center
  - !rusoc : Display Rutgers Schedule of Classes information for CS440
  - !twitter : Display latest tweet from wlynch92
- exit : disconnect from server

This was done with heavy use of the Arduino [Ethernet Library](http://arduino.cc/en/Reference/Ethernet), as well as a supplementary [script](https://github.com/wlynch92/rusoc) to scrape the Rutgers Schedule of Classes.

To Do
--------
- [x] HelloWorld
- [x] Echo Server
- [x] Get server to recognize commands
- [x] Get server to store a persistent message across sessions
- [x] Enable some sort of authentication system (even if very simple)
- [x] Enable display to show messages
- [x] Integrate NextBus
- [ ] Add additional services (twitter, email, etc)?
- [ ] Support arbitrary inputs for services (e.g. select your own bus stop/class/twitter feed)
- [ ] WiFi
- Possible alternative: Swap arduino w/ pi?
