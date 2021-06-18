import serial
from time import sleep


ser = serial.Serial(
	'/dev/ttyACM0', baudrate=9600, bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE, timeout=None)
ser.timeout = 0.1

while True:
	ser_bytes = ser.readline()
	print(ser_bytes)





