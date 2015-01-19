#include <LEDPixels.h>

//Example to control RGB LED Modules with Spectrum Analyzer
//Bliptronics.com
//Ben Moyes 2010
//Use this as you wish, but please give credit, or at least buy some of my LEDs!
//


LEDPixels LP;  //Our LEDPixels library - see http://www.bliptronics.com/ArduinoCode/LEDPixels.zip
   
//For spectrum analyzer shield, these three pins are used.
//You can move pinds 4 and 5, but you must cut the trace on the shield and re-route from the 2 jumpers. 
int spectrumReset=5;
int spectrumStrobe=4;
int spectrumAnalog=0;  //0 for left channel, 1 for right.

//This holds the 15 bit RGB values for each LED.
//You'll need one for each LED, we're using 25 LEDs here.
//Note you've only got limited memory, so you can only control 
//Several hundred LEDs on a normal arduino. Double that on a Duemilanove.

int MyDisplay[25];

// Spectrum analyzer read values will be kept here.
int Spectrum[7];

void setup() {
  byte Counter;

  //Initialize the LEDPixels library.
  //            refresh delay, address of data, number of LEDs, clock pin, data pin.
  LP.initialize(25, &MyDisplay[0],25, 12, 11 );

  //Setup pins to drive the spectrum analyzer. 
  pinMode(spectrumReset, OUTPUT);
  pinMode(spectrumStrobe, OUTPUT);

  //Init spectrum analyzer
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,LOW);
    delay(5);
  // Reading the analyzer now will read the lowest frequency.
  
  // Turn all LEDs off.
  LP.setRange(0,24,LP.color(0,0,0));
  LP.show();                             //Write out display to LEDs

}

void loop() {

  int Counter, Counter2, Counter3;
    
  showSpectrum();
  delay(15);  //We wait here for a little while until all the values to the LEDs are written out.
              //This is being done in the background by an interrupt.
}

// Read 7 band equalizer.
void readSpectrum()
{
  // Band 0 = Lowest Frequencies.
  byte Band;
  for(Band=0;Band <7; Band++)
  {
    Spectrum[Band] = (analogRead(spectrumAnalog) + analogRead(spectrumAnalog) ) >>1; //Read twice and take the average by dividing by 2
    digitalWrite(spectrumStrobe,HIGH);
    digitalWrite(spectrumStrobe,LOW);     
  }
}


void showSpectrum()
{
  //Not I don;t use any floating point numbers - all integers to keep it zippy. 
   readSpectrum();
   byte Band, BarSize, MaxLevel;
   static unsigned int  Divisor = 80, ChangeTimer=0; //, ReminderDivisor,
   unsigned int works, Remainder;
  
  MaxLevel = 0; 
        
  for(Band=0;Band<5;Band++)//We only graph the lowest 5 bands here, there is 2 more unused!
  {
  //If value is 0, we don;t show anything on graph
     works = Spectrum[Band]/Divisor;	//Bands are read in as 10 bit values. Scale them down to be 0 - 5
     if(works > MaxLevel)  //Check if this value is the largest so far.
       MaxLevel = works;                       
     for(BarSize=1;BarSize <=5; BarSize++)  
        {
        if(	works > BarSize) LP.setLEDFast(	LP.Translate(Band,BarSize-1),BarSize*6,31-(BarSize*5),0);
          else if ( works == BarSize) LP.setLEDFast(	LP.Translate(Band,BarSize-1),BarSize*6,31-(BarSize*5),0); //Was remainder
            else LP.setLEDFast(	LP.Translate(Band,BarSize-1),5,0,5);
        }
  }
  LP.show();

 // Adjust the Divisor if levels are too high/low.
 // If  below 4 happens 20 times, then very slowly turn up.
  if (MaxLevel >= 5)
  {
    Divisor=Divisor+1;
    ChangeTimer=0;
  }
  else
    if(MaxLevel < 4)
    {
      if(Divisor > 65)
        if(ChangeTimer++ > 20)
        {
          Divisor--;
          ChangeTimer=0;
        }
    }
    else
    {
      ChangeTimer=0; 
    }
  }




 
    
