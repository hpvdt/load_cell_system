#include "HX711.h"
const int DOUT12 = 2;
const int CLK12 = 3;
const int DOUT34 = 4;
const int CLK34 = 5;
const int DOUT56 = 6;
const int CLK56 = 7;

HX711 scale12;
HX711 scale34;
HX711 scale56;

const char ending[] = "\n";
const char separator = ';';

long readings[6];

//#define DEV_MODE 
// Used to toggle the dev mode, sending random numbers rather than reading actual values.
// Uncomment it if you want to enter dev mode, leave commented out for normal operation.

#ifdef DEV_MODE
int i = 0; // Used for generating data in dev mode
#endif

void setup() {
  Serial.begin(115200);
  scale12.begin(DOUT12, CLK12);
  scale34.begin(DOUT34, CLK34);
  scale56.begin(DOUT56, CLK56);
  delay(1000); // Delay for chips to start
}

void loop() {
#ifdef DEV_MODE
  // Development mode for interface testing
  // Just spits out numbers to be shown
  Serial.print(start);
  Serial.print(i * 2);
  Serial.print(separator);
  Serial.print(i);
  Serial.print(separator);
  Serial.print(i / 2);
  Serial.print(separator);
  Serial.print(i / 3.0);
  Serial.print(separator);
  Serial.print(i % 10);
  Serial.print(separator);
  Serial.print(0);
  Serial.print(ending);
  i++;  
  delay(100);
#else
  /* Normal operation
    Reads all possible values for the system.

    NOTE: Due to the operation of the HX711 chips the gain setting is not 
    as expected (e.g. setting it to 32 before reading for channel B). This 
    is due to us waiting for data being ready before setting the gain and 
    then reading.
    
    The data readied for us to read is from the previous gain setting since 
    the gain change has not had the time to come into effect on the data 
    prepared. So what we are effectively doing is setting the gain for the 
    next reading not the one that immediately follows a gain change.

    ALSO NOTE: For a given HX711 channel A is the even numbered header, 
    channel B is used for the odd numbered header.

    To make reads faster all chips are read for one channel then swapped to 
    the other channel so we only have to wait for one refresh period across 
    all the chips rather then reading A and B for each chip sequentially.
  */

  
  // Read all the odd load cell ports (channel Bs)
  scale12.wait_ready(1); // Wait in 1ms increments until ready to read  
  // With a gain factor of 64 or 128, channel A is selected for the next read
  scale12.set_gain(128); 
  readings[0] = scale12.read();

  scale34.wait_ready(1);
  scale34.set_gain(128);
  readings[2] = scale34.read();

  scale56.wait_ready(1);
  scale56.set_gain(128);
  readings[4] = scale56.read();


  // Read all the even load cell ports (channel As)
  scale12.wait_ready(1);
  // With a gain factor of 32, channel B is selected for the next read
  scale12.set_gain(32); 
  readings[1] = scale12.read();

  scale34.wait_ready(1);
  scale34.set_gain(32);
  readings[3] = scale34.read();

  scale56.wait_ready(1);
  scale56.set_gain(32);
  readings[5] = scale56.read();


  String data = "";
  for (int i = 0; i < 6; i++) {
    data.concat(readings[i]);
    data.concat(separator);
  }
  data.concat(ending);

  Serial.print(data);
#endif
}
