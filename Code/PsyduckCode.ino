//Psyduck
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Constants
const int buzzPin = 0;
const int comEmmitPin = 1;
const int buttonPin = 2;
const int ledPin = 3;
const int comReceivePin = 4; //2

//button variables
long pressPeriod = 3000; //in milliseconds
unsigned long buttonPressStartTime;
bool pressed = false;
bool prevPressed = false;

//LED variables
unsigned int red;
unsigned int green;
unsigned int blue;
long colorChangeSpeed = 200;

//Communication variables
bool harm = false;
bool prevHarm = false;
unsigned long harmStartTime;
bool initializationDone = false;

//fight logic variables
bool inFightMode = false;
bool attackSuccessful = false;
long responseTime = 3000; //in milliseconds
long attackPeriod = 3000; //the emmition period of the attack mechanism
int maxValue = 2^15 - 1;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, ledPin, NEO_RGBW + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {     

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  
  //Set up pinMode()
  pinMode(buzzPin,OUTPUT); // Set buzzer-pin as output
  pinMode(comEmmitPin, OUTPUT); // Set lasser-pin as output
  pinMode(buttonPin, INPUT_PULLUP); // initialize the pushbutton pin as an input:
  pinMode(comReceivePin, INPUT_PULLUP); // Set receiver-pin as output
  digitalWrite(comEmmitPin, HIGH);

  // Set up neopixel
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {

  if(inFightMode){
    //if(initializationDone){
      fightMode();
    //}
  }
  else{
    idleMode();
  }

  // For testing all the functionalities like button, neopixel, buzzer, lasser
  /*
  if (!digitalRead(buttonPin)) {  // if the button is pressed
    colorWipe(strip.Color(138, 43, 226), 50); // blueviolet
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(0, 0, 0), 50); // Off
    strip.show();
    //digitalWrite(ledPin, HIGH);    // light up the LED
    digitalWrite(laserPin, HIGH);
    // For Buzzer
    beep(0, 2000, 1000);
    //delay(1000);
  } else {
    digitalWrite(ledPin, LOW);     // otherwise, turn it off
    digitalWrite(laserPin, LOW);
    //delay(1000);
  }
 */
}

//FIGHT MODE
void fightMode(){
  //fight sensor data gathering
  comReceiveResponse();
  buttonResponse();

  //if register harm then start the clock to check whether it's disconnect or attack.
  if (harm) {
    if (!prevHarm) {
        harmStartTime = millis();
        prevHarm = true;
    }
  } 
  //if no harm registered attack
  else if (pressed && !prevHarm) {
    attack();
  }

  //if attack period exceeded check if it's a disconnect or defeated
  if (millis() - harmStartTime > attackPeriod && prevHarm) {
    //if defeat perform response
    if (!harm) {
      harmStartTime = maxValue;
      // if psyduck is defeated make a noise and change the color to red
      beep(0, 2000, 800);
      colorWipe(strip.Color(0, 255, 0), colorChangeSpeed); // red
      //delay(responseTime);
      colorWipe(strip.Color(green, red, blue), colorChangeSpeed); // Off
      prevHarm = false;
    }
    //if disconnect exit fightmode
    else {
      harmStartTime = maxValue;
      inFightMode = false;
      prevHarm = false;
    }
  }
}

//BUTTON RESPONSE FUNCTION
void buttonResponse(){
  if (digitalRead(buttonPin) == HIGH) {
    pressed = false;
  }
  else {
    pressed = true;
  }
}

//COMMRECEIVE RESPONSE
void comReceiveResponse(){
  if (digitalRead(comReceivePin) == HIGH) {
    //initializationDone = true;
    harm = false;
  }
  else {
    //initializationDone = false;
    harm = true;
  }
}

//ATTACK FUNCTION
void attack(){
  //send out attack signal
  digitalWrite(comEmmitPin, LOW);
  delay(0.5*attackPeriod);
  digitalWrite(comEmmitPin, HIGH);
  delay(0.5*attackPeriod);
  //perform attack action
  // When attack is succesfull turn lights to purple
   colorWipe(strip.Color(4, 99, 146), colorChangeSpeed); // blueviolet
   //delay(responseTime);
   colorWipe(strip.Color(green, red, blue), colorChangeSpeed); // Off
}

//IDLE MODE
void idleMode() {
  //listen for connection of two objects
  if (digitalRead(comReceivePin) == HIGH) {
    //initializationDone = true;
    inFightMode = true;
  }
  //Change Neopixel color
  eyeColorChangeF();
}

//EYE COLOR CHANGE
void eyeColorChangeF() {
  //exit from long press recording when button unpressed
  buttonResponse();
    
  if (!pressed) {
    prevPressed = false;
  }
  //record first button press start time
  if (pressed && !prevPressed) {
    prevPressed = true;
    buttonPressStartTime = millis();
  }
  //react to a long button press
  if (millis() - buttonPressStartTime > pressPeriod && prevPressed) {
    red = random(255);
    green = random(255);
    blue = random(255);
    if (red > 245 && green < 10 && blue < 10) {
      green = 255;
      blue = 255;
    }
    //initializationDone = false;
    colorWipe(strip.Color(green, red, blue), colorChangeSpeed);
    buttonPressStartTime = maxValue;    
  }
  //Turn of the light
  delay(responseTime);
  colorWipe(strip.Color(0, 0, 0), colorChangeSpeed); // Off
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// The sound-producing function
void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{ // http://web.media.mit.edu/~leah/LilyPad/07_sound_code.html
  int  x;
  long delayAmount = (long)(1000000 / frequencyInHertz);
  long loopTime = (long)((timeInMilliseconds * 1000) / (delayAmount * 2));
  for (x = 0; x < loopTime; x++) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(delayAmount);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(delayAmount);
  }
}
