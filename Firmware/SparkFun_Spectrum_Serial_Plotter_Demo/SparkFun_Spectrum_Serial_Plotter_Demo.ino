/******************************************************************************
  SparkFun Spectrum Shield Demo
  Toni Klopfenstein @ SparkFun Electronics
  December 2015
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
#define RESET 5
#define DC_One A0
#define DC_Two A1

//Define spectrum variables
int freq_amp;
int Frequencies_One[7];
int Frequencies_Two[7];
int i;

/********************Setup Loop*************************/
void setup() {
  //Set spectrum Shield pin configurations
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(DC_One, INPUT);
  pinMode(DC_Two, INPUT);

  //Initialize Spectrum Analyzers
  digitalWrite(STROBE, LOW);
  digitalWrite(RESET, LOW);
  delay(5);

  Serial.begin(9600);
}


/**************************Main Function Loop*****************************/
void loop() {
  Read_Frequencies();
  Graph_Frequencies();

}


/*******************Pull frquencies from Spectrum Shield********************/
void Read_Frequencies() {
  digitalWrite(RESET, HIGH);
  delayMicroseconds(200);
  digitalWrite(RESET, LOW);
  delayMicroseconds(200);

  //Read frequencies for each band
  for (freq_amp = 0; freq_amp < 7; freq_amp++)
  {
    digitalWrite(STROBE, HIGH);
    delayMicroseconds(50);
    digitalWrite(STROBE, LOW);
    delayMicroseconds(50);

    Frequencies_One[freq_amp] = analogRead(DC_One);
    Frequencies_Two[freq_amp] = analogRead(DC_Two);
  }
}

/*******************Light LEDs based on frequencies*****************************/
void Graph_Frequencies() {
  for (i = 0; i < 7; i++)
  {
//    Serial.print(Frequencies_One[i]);
//    Serial.print(" ");
//    Serial.print(Frequencies_Two[i]);
//    Serial.print(" ");
    Serial.print( (Frequencies_One[i] + Frequencies_Two[i]) / 2 );
    Serial.print("    ");
  }
  Serial.println();
}


