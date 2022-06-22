/*
  Modified from:  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
  go nuts if you want to use this
*/

//map directions to pins on the teensy
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define BUTTON 6

//number of milliseconds to divide for 59.97 fps and 60 fps respectively
#define DROP_FRAME 1001000
#define NO_DROP_FRAME 1000000

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9960);
  // make the pins inputs:
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(BUTTON, INPUT);
}

//global variable to track how many loops have happened
uint64_t x = 0;

// the loop routine runs over and over again forever:
void loop() {
  //find starting time of this loop in millis() and wait until that time happens
  uint32_t st = [](uint64_t y){return y * 1000000 * 228 * 262 / 3579545;}(x++);
  while(st - micros() < 50000); //because unsigned, once micros() passes st, it'll be a billion or whatever; greatest the difference should be is ~16688 so 50k just to be safe
 
  // read the input pins:
  char buttonState = 0x1F & ~(digitalRead(UP) + 2 * digitalRead(DOWN) + 4 * digitalRead(LEFT) + 8 * digitalRead(RIGHT) + 16 * digitalRead(BUTTON));

  // print out the state of the button:
  Serial.print(buttonState);
}
