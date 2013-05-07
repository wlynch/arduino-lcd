import requests
import serial
import time

# website username linked with phone number 
username1 = 'robertf224'
username2 = 'katief818'

# last message
username1_last = ''
username2_last = ''

# listen to arduino over serial
ser = serial.Serial('/dev/tty.usbmodem1411', 9600)

ser.write('starting...')

while True:

	#username = ser.readline().partition('\r')[0]

	response1 = requests.get('http://door-secretary.herokuapp.com/' + username1)

	if username1_last != response1.text:
		ser.write(response1.text)
		print response1.text

	else:
		response2 = requests.get('http://door-secretary.herokuapp.com/' + username2)
		if username2_last != response2.text:
			ser.write(response2.text)

		username2_last = response2.text
		print response2.text

	username1_last = response1.text


	time.sleep(5)


