#include <Arduino.h>
#include <Servo.h>


Servo servo1, servo2;

void setup(){
	
	Serial.begin(115200);
	pinMode(13,OUTPUT);
	servo1.attach(10);
	servo2.attach(11);
 

	
	//Serial.write(buffer_ar,2);
//	delay(10);

}

void loop(){
  const int NMAX = 64;
  char buffer_ar[NMAX];
  float ms1, ms2;
  int n, len = 8;
  static int i = 0;

  float *q;
  char * p;
  

  //check if bytes are available at port
  while(Serial.available() == 0) {
    i++;
  }
  
  // using memory buffer/chunks
  p = buffer_ar;
  q = (float* )p; 
  
	// readbytes reads the buffer till length specified and returns the number
	// of bytes read
 

	n = Serial.readBytes(buffer_ar,len);
	if(n < len){
		Serial.print("\nerror, not enough data read");
	}
	
	ms1 = q[0];
	ms2 = q[1];
	servo1.writeMicroseconds(ms1);
	servo2.writeMicroseconds(ms2);
  q[0] = ms2;
	Serial.write(buffer_ar,4  );

}
