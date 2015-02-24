/****************************************************************************** 
SparkFun Spectrum Shield PWM Demo
Toni Klopfenstein @ SparkFun Electronics
February 2015
https://github.com/sparkfun/Spectrum_Shield

This sketch shows the basic functionality of the Spectrum Shield, working with a basic RGB LED Matrix.

The Spectrum Shield code is based off of the original demo sketch by Ben Moyes @Bliptronics.  
This sketch is available in the Spectrum Shield repository. 

Development environment specifics:
Developed in Arduino 1.6.

This code is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.
*********************************************************************************/

//Declare Spectrum Shield pin connections
#define STROBE 4
#define RESET 6
#define DC_One A0
#define DC_Two A1 


//Define LED connections on the shield
int LED[] = {7, 8, 9, 10, 11, 12, 13};

//Define spectrum variables
int freq_amp;
int Frequencies_One[7];
int Frequencies_Two[7]; 
int i;

/********************Setup Loop*************************/
void setup() {
  
  //Set LED pin configurations
  for(i=0; i<7; i++)
  {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }
  
  //Set Spectrum Shield pin configurations
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(DC_One, INPUT);
  pinMode(DC_Two, INPUT);  
  digitalWrite(STROBE, HIGH);
  digitalWrite(RESET, HIGH);
  
  //Initialize Spectrum Analyzers
  digitalWrite(STROBE, LOW);
  delay(1);
  digitalWrite(RESET, HIGH);
  delay(1);
  digitalWrite(STROBE, HIGH);
  delay(1);
  digitalWrite(STROBE, LOW);
  delay(1);
  digitalWrite(RESET, LOW);
}


/**************************Main Function Loop*****************************/
void loop() {
  
  Read_Frequencies();
  Graph_Frequencies();
  delay(50);
 
}


/*******************Pull frequencies from Spectrum Shield********************/
void Read_Frequencies(){
  //Read frequencies for each band
  for (freq_amp = 0; freq_amp<7; freq_amp++)
  {
    Frequencies_One[freq_amp] = analogRead(DC_One);
    Frequencies_Two[freq_amp] = analogRead(DC_Two); 
    digitalWrite(STROBE, HIGH);
    digitalWrite(STROBE, LOW);
  }
}

/*******************Light LEDs based on frequencies*****************************/
void Graph_Frequencies(){
   for( i= 0; i<7; i++)
   {
     if(Frequencies_Two[i] > Frequencies_One[i]){
        analogWrite(LED[i], Frequencies_Two[i]/4);
     }
     else{
        analogWrite(LED[i], Frequencies_One[i]/4);
     }
   }
}
