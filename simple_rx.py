import serial

ser = serial.Serial(
	'/dev/ttyACM0', baudrate=9600, bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE, timeout=None)

ser_bytes = ser.readline()
print(ser_bytes)