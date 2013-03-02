arduino-lcd
===========

Display arbitrary messages from a network connection on an LCD display using an Arduino


To Do
--------
- [x] HelloWorld
- [x] Echo Server
- [x] Get server to recognize commands
- [x] Get server to store a persistent message across sessions
- [x] Enable some sort of authentication system (even if very simple)

- [ ] Enable display to show messages

- Possible alternative: Swap arduino w/ pi?

~~Backup plan~~
-------------
~~Just so we have something that works for next week...~~
I was able to get the initial plan to work. This seems unnecessary now, but it is something we can come back to if we're interested.

- [ ] Enable display to show messages
- [ ] Setup server to store current message
- [ ] Have arduino query server for message to display
- [ ] Step 1) Have arduino query every n seconds
- [ ] Step 2) Have arduino take in input and forward to server. Query server for changes.
- [ ] Step 3) Move server functionality back to arduino.
