#include <Arduino.h>

void setup()
{
	Serial.begin(9600);

}

void loop()
{
   char n;
   int i;

    // wait until new data is available
	// -> force blocking
    while( Serial.available() == 0 ) i++;

	
	// n is the number of bytes received from the read function
    n = Serial.read();

	// wait until 6 bytes are received -> block
	// partial block with a "timeout" after some time
	// -> communiation fault
	// -> stop your robot -> turn off or try again later
    
    Serial.println( (char)n);

}


	