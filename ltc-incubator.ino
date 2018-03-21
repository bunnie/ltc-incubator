// Love to Code
#include "ChibiOS.h"

int tempcode = 0;  // this is the value we want to display on the screen
float temp = 0.0;
int temperature = 0;

void control_code(void *config) {
  // control to the higher side of 37C
  // literature shows e-coli peak growth ~40C, with rapid decline over 42C
  while(1) {
    if( temp > 39.0 ) {
      off(0);
    } else if( temp < 37.0 ) {
      on(0);
    }
    delay(1000);
   // off(0);
   // pause(1000);
  }
}

void setup(void) {
  outputMode(0);
  off(0);
  
  outputMode(1);
  off(1);
  
  Serial.begin(9600);     // setup the TXT port
  analogReadResolution(12);
  
  createThreadFromHeap(THD_WORKING_AREA_SIZE(128), 20,
                   control_code, NULL);
}

void render() {
  Serial.println("#LCK"); // put the screen into "lock" mode
  
  Serial.print("Temperature: "); // print an explanation
  Serial.print( temperature / 10 );
  Serial.print( "." );
  Serial.print( temperature % 10 );

  // spaces are mandatory for a "blank" line. Successive blank lines without spaces in them are ignored.
  // try removing the line below and see what happens!
  Serial.println(" "); // add a blank line to the bottom of the screen
  Serial.println(" "); // add a second blank line on the bottom of the screen
  
  Serial.println("#SYN"); // synchronize the screen contents
  delay(30); // a delay of 30ms is necessary to allow the screen to update, or else the contents are garbled
  
}


void loop(void) {
  tempcode = analogRead(A6); // A6 is the temperature sensor
  float tempV = ((float) tempcode) * (3300.0 / 4095.0);

  temp = 25.0 - ((tempV - 716.0) / 1.62); // 716mV and 1.62mV/C are from the KL02 datasheet
  // now we have the nominal temperature, but it needs a calibration factor
  // this is calibrated using a K-type thermocouple on a digital thermometer type 342
  
  temp = temp * 0.725 + 9.365;

  temperature = temp * 10; // bring to an integer, mult by 10 to preserve some precision
  
  delay(250);
  
  render(); // this jumps to the render code above, keeping the loop() nice and tidy
}
