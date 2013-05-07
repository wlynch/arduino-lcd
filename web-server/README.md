Server as Middle Man

====================
Software/Hardware Resources: 
----------------------------
- node.js/express server hosted on heroku (trial acount)
- twilio api for text message interface (trial account)
- Arduino Uno, 16x2 LCD

Final Design: 
-------------
The final design involved an Arduino Uno updating an LCD with messages passed over USB. The displayed messages were pulled from a URL associated with a specific username on an external server, which was associated with a phone number. The retrieved message was the message most recently sent via SMS to a specific phone number (twilio trial account number). The messages were pulled using a simple python program on the client computer as to not have to deal with the complications of network permissions on Rutgers' networks. The same results could easily be achieved with an ethernet/wifi shield on a network with less complicated security (i.e. a home network). The tradeoff of hosting the service on an external server rather than the Arduino board itself allows for interesting user interfaces involving third-party api's such as the one used
