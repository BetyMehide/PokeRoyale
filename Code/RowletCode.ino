#include <Adafruit_NeoPixel.h>
//Pins
const int ledPin = 3;
const int comEmmitPin = 1;
const int comReceivePin = 4;
const int diodePin = 0;
const int buttonPin = 2;
#ifdef __AVR__
#include <avr/power.h>
#endif
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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, ledPin, NEO_RGBW + NEO_KHZ800);
//harm variables
bool harm = false;
bool prevHarm = false;
unsigned long harmStartTime;
//fight logic variables
bool inFightMode = false;
bool attackSuccessful = false;
int attackFlaps = 5; //number of flaps performed for attack
int flapWaitTime = 500; //wait time between flap moves
long responseTime = 3000; //in milliseconds
long attackPeriod = 3000; //the emmition period of the attack mechanism
int maxValue = 2^15 - 1;

//SETUP
void setup() {
  //Trinket special code
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  //set up pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(comEmmitPin, OUTPUT);
  pinMode(comReceivePin, INPUT_PULLUP);
  pinMode(diodePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  //set up laser
  digitalWrite(comEmmitPin, HIGH);
  //set up neopixel
  strip.begin();
  strip.show(); //all pixel initialized to off
}

//MAIN LOOP
void loop() {
  if (inFightMode) {
    fightMode();
  }
  else {
    idleMode();
  }
}

//IDLE MODE
void idleMode() {
  //listen for connection of two objects
  if (digitalRead(comReceivePin) == HIGH) {
    inFightMode = true;
  }
  //Change body color
  bodyColorChangeF();
}

//BODY COLOR CHANGE
void bodyColorChangeF() {
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
    colorWipe(strip.Color(green, red, blue), colorChangeSpeed);
    buttonPressStartTime = maxValue;
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

//IMPLEMENT COLOR
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//FIGHT MODE
void fightMode() {
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
  
  //if attack period exceeded check if it's a disconnect or defeat
  if (millis() - harmStartTime > attackPeriod && prevHarm) {
    //if defeat perform lost response
    if (!harm) {
      harmStartTime = maxValue;
      colorWipe(strip.Color(0, 255, 0), colorChangeSpeed);
      delay(responseTime);
      colorWipe(strip.Color(green, red, blue), colorChangeSpeed);
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

//COMMRECEIVE RESPONSE
void comReceiveResponse(){
  if (digitalRead(comReceivePin) == HIGH) {
    harm = false;
  }
  else {
    harm = true;
  }
}

//ATTACK FUNCTION
void attack() {
  //send out attack signal
  digitalWrite(comEmmitPin, LOW);
  delay(0.5*attackPeriod);
  digitalWrite(comEmmitPin, HIGH);
  delay(0.5*attackPeriod);
  //perform attack action
  for (int numberOfFlaps = 0; numberOfFlaps <= attackFlaps; numberOfFlaps++) {
    digitalWrite(diodePin, HIGH);
    delay(flapWaitTime);
    digitalWrite(diodePin, LOW);
    delay(flapWaitTime);
  }
}
