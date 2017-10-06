/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include "DVDPinDefs.h"
#include "mcpDac.h"


// These constants won't change.  They're used to give names
// to the pins used:
const int potTopLeft = A8;  // Analog input pin that the potentiometer is attached to
const int potTopRight = A9;  // Analog input pin that the potentiometer is attached to
const int potBottomLeft = A10;  // Analog input pin that the potentiometer is attached to
const int potBottomRight = A11;  // Analog input pin that the potentiometer is attached to

const int LEDPin = 7; // Analog output pin that the LED is attached to
const int buttonPin = 11;     // the number of the pushbutton pin


const int SEL2 =  A0;      // the number of the LED pin
const int SEL1 =  A1;      // the number of the LED pin
const int OSC =  2;      // the number of the LED pin


int potValue1,potValue2,potValue3,potValue4 = 0;        // value read from the pot
int potValueRaw1,potValueRaw2,potValueRaw3,potValueRaw4 = 0;        // value read from the pot

int outputValue = 1900;        // value output to the PWM (analog out)

int x=0;
int y=0;

 int sizeX=200;
 int sizeY=200;
 int posX=2170;
 int posY=2050;
 int fact=3;
 int chanel=5;

const int mode=0; //0: scanning xy  1:scanning xz
 int focus_shift=0;
const int focus_step=0;
const int csize=10;
float bounce_fact;

int control_data_in[csize*2];
int control_data_out[csize];


int intensitiy;
int c,val;
int j=0; 

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
       mcpDacInit();

pinMode(A1, INPUT);           // set pin to input
//digitalWrite(A1, HIGH);       // turn on pullup resistors
  pinMode(LEDPin, OUTPUT);

  pinMode(OSC, OUTPUT);
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
digitalWrite(OSC, HIGH);       // ?
digitalWrite(SEL1, HIGH);       // SEL1/SEL2 HIGH= UV SEL1/SEL2 LOW = OFF
digitalWrite(SEL2, HIGH);       //  SEL1 HIGH / SEL2 LOW = RED, SEL1 LOW / SEL2 HIGH = IR

x=sizeX;

mcpDac3BSend (1675);//Set Amp Offset

}

void loop() {
  
  
  if (Serial.available()>0) 
{
    control_data_in[j] =Serial.read();
    j++;


}

 
if (j==(csize*2))
{
  j=0;

  control_data_out[0]=potValue1<<2;   //Laser Intensity
  control_data_out[1]=(potValue2<<2)+(potValue4>>2)+focus_shift;  //Drive reading head focus
  control_data_out[2]=potValue3<<2;   //Drive stage Z
  control_data_out[4]=fact;
  control_data_out[5]=focus_step;
  control_data_out[6]=focus_shift;
 control_data_out[7]=control_data_in[0]*256+control_data_in[1];
 control_data_out[8]=control_data_in[2]*256+control_data_in[3];

  sizeX=control_data_in[0]*256+control_data_in[1];
  sizeY=control_data_in[2]*256+control_data_in[3];
  posX=control_data_in[4]*256+control_data_in[5];
  posY=control_data_in[6]*256+control_data_in[7];
  fact=control_data_in[8]*256+control_data_in[9];
  chanel=control_data_in[10]*256+control_data_in[11];
  if (control_data_in[13]==1)  y=0;
  
  if (control_data_in[15]>1) digitalWrite(SEL1, HIGH);  else digitalWrite(SEL1, LOW);
  if ((control_data_in[15]%2) == 0) digitalWrite(SEL2, LOW); else digitalWrite(SEL2, HIGH);

  
  
  for (int i=0; i < csize; i++){
Serial.write(control_data_out[i]&0xff);
Serial.write(control_data_out[i]>>8); //analogRead(A0)
  }
  
  bounce_fact=sizeX/10000;
  
for (int i=0; i < sizeX; i++)
  {
    x=(100-i)*(100-i)*bounce_fact;
     mcpDac2BSend(posX+((-(sizeX/2)+x)*fact));        //Drive reading head X
     delayMicroseconds(400);
  };
     delayMicroseconds(1000);


  x=0;
  y++;
    if (y>sizeY) {y=0;  focus_shift+=focus_step;}

 
}

  
  
if(x<sizeX)
{
  
  intensitiy = analogRead(A0+chanel);
  if (intensitiy<350) digitalWrite(LEDPin, HIGH); else digitalWrite(LEDPin, LOW);
     
   mcpDac2BSend(posX+((-(sizeX/2)+x)*fact));        //Drive reading head X

  if (mode==0) mcpDac1BSend(posY+((-(sizeY/2)+y)*fact));        //Drive stage Y
  if (mode==1) mcpDac2ASend(4096-y*20);  
   
x++;

Serial.write(intensitiy&0xff);
Serial.write(intensitiy>>8); //analogRead(A0)
 // digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
 // digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
 // delay(5);              // wait for a second
}


  
  
  
  // read the analog in value:
  potValueRaw1 = analogRead(potTopRight);
  potValueRaw2 = analogRead(potTopLeft);
  potValueRaw3 = analogRead(potBottomLeft);
  potValueRaw4 = analogRead(potBottomRight);

  if ((potValueRaw1-potValue1)>2) {potValue1=potValueRaw1-2;} // add hysteresis
  if ((potValue1-potValueRaw1)>2) {potValue1=potValueRaw1+2;}

  if ((potValueRaw2-potValue2)>2) {potValue2=potValueRaw2-2;}
  if ((potValue2-potValueRaw2)>2) {potValue2=potValueRaw2+2;}
  
  if ((potValueRaw3-potValue3)>2) {potValue3=potValueRaw3-2;}
  if ((potValue3-potValueRaw3)>2) {potValue3=potValueRaw3+2;}
  
  if ((potValueRaw4-potValue4)>2) {potValue4=potValueRaw4-2;}
  if ((potValue4-potValueRaw4)>2) {potValue4=potValueRaw4+2;}

  // map it to the range of the analog out:
 // outputValue = map(potValue, 0, 1023, 0, 4095);
  // change the analog out value:

  // print the results to the serial monitor:



  
  if (mode==0)  mcpDac2ASend((potValue2<<2)+(potValue4>>2)+focus_shift);        //Drive reading head focus
   //  mcpDac2ASend(outputValue);        //Drive reading head focus

   mcpDac1ASend(potValue3<<2);        //Drive stage Z


   mcpDac3ASend(potValue1<<2);        //Current for Laser Diode



  // wait+ 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
     delayMicroseconds(1000);
}
