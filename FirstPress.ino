//      The Photobomb v1.0       //
// Software by James Villalpando //

// use NodeMCU 1.0 chip for ESP
// use generic Arduino Uno for Metro

// Set up the button and solenoid
// Define wait time and camera button
#define BUTTON  6
#define FIREFLASH  NULL
#define PIXELFLASH  3
#define PIXELSHOW  3
#define CAMERASHUTTER  2
#define CAMERAFOCUS 1
#define LED     D4
#define DEBUG   1
#define OVERRIDE D7
#define FIREPOT 0



bool        debug = true;
const int   accumulator = 10000; // total accumulator
unsigned long time, runtime = 0; // Accumulator timing
int         leds = 12;


// comment out if not using neopixels
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
Adafruit_NeoPixel strip = Adafruit_NeoPixel(leds, FIREFLASH, NEO_GRB + NEO_KHZ800);


void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(OVERRIDE, INPUT_PULLUP);
  pinMode(FIREPOT, INPUT);
  pinMode(CAMERASHUTTER, OUTPUT);
  pinMode(CAMERAFOCUS, OUTPUT);
  pinMode(FIREFLASH, OUTPUT);
  pinMode(PIXELFLASH, OUTPUT);
  pinMode(CAMERA, OUTPUT);
  pinMode(LED, OUTPUT);
  if(PIXELSHOW && PIXELSHOW != PIXELFLASH){
    pinMode(PIXELFLASH, OUTPUT);
  }

  if (PIXELFLASH != NULL) {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
      if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif


    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
  }


  if (DEBUG) { Serial.begin(9600); }
  debugMsg("Photobomb Operational!");
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
           /* MAIN LOOP */
//////////////////////////////////////////////////
//////////////////////////////////////////////////

void loop() {
	const int   wait = 500;
  //const int   wait = analogRead(FIREPOT);
  // If the button is pressed
  // and the accumulator is full enough
  if (FIREFLASH != NULL) {
    fireCamera();
  }else if (PIXELFLASH != NULL){
    pixelCamera();
  }if (PIXELSHOW != NULL){
    pixelShow();
  }
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
           /* END MAIN LOOP */
//////////////////////////////////////////////////
//////////////////////////////////////////////////



//////////////////////////////////////////////////
           /* CAMERA FUNCTIONS */
//////////////////////////////////////////////////

void takePhoto() {
	digitalWrite(CAMERASHUTTER, HIGH);       // snap a photo
	delay(5);
	digitalWrite(CAMERASHUTTER, LOW);        // turn off camera snapper
}

void focus() {
	digitalWrite(CAMERAFOCUS, HIGH);       // focus the lens
	delay(500);
	digitalWrite(CAMERAFOCUS, LOW);        // release focus
}


//////////////////////////////////////////////////
           /* FIRE FUNCTIONS */
//////////////////////////////////////////////////

void fireCamera(){
  if(runtime < accumulator && digitalRead(BUTTON) == LOW  && digitalRead(OVERRIDE) == HIGH ){
      debugMsg("Button press!");
      debugMsg(String(digitalRead(OVERRIDE)));
      takeFirePhoto();
    }else if(digitalRead(BUTTON) == LOW && digitalRead(OVERRIDE) == LOW){
      digitalWrite(FIREFLASH, HIGH);
      digitalWrite(LED, HIGH);
      debugMsg("Overide poofing!");
    }else{
      if(runtime > 0){ // refill timer
        runtime--;
      }
      if(runtime > accumulator) { // if the accumulator must fill, flash LED
        for(int i = 0;i <= 10 ;i++){
          digitalWrite(LED, HIGH);
          delay(100);
          digitalWrite(LED, LOW);
          delay(100);
        }
      }
      // Otherwise, make sure the fire is off
      if(digitalRead(FIREFLASH) == HIGH){
        digitalWrite(FIREFLASH, LOW);
        digitalWrite(LED, LOW);
      }
    }
}
void puffUp(int times, bool finish){
  // flash `times` at a `rate` and `finish` in which state
  for(int i = 0;i <= times ;i++){
    digitalWrite(FIREFLASH, finish);
    time = millis();
    digitalWrite(LED, finish);
    delay(100);
    digitalWrite(FIREFLASH, !finish);
    runtime += millis() - time;
    debugMsg("Run time: " + String(runtime));
    digitalWrite(LED, !finish);
    delay(500);
  }
}

void takeFirePhoto() {
  // Puff for attention
  puffUp(3,LOW);                    // three puffs
  delay(1500);                      // pause for dramatic effect
  // turn on the flame
  digitalWrite(FIREFLASH, HIGH);       // FIRE!
  time = millis(); // start timer
  focus();
  if(DEBUG){digitalWrite(LED, HIGH);} // indicate on board
  delay((int)wait*.667);
  takePhoto();
  delay((int)wait*.333);
  // turn off flame
  digitalWrite(FIREFLASH, LOW);        // turn off flame
  runtime += millis() - time;       // calculate accumulator depletion
  if(DEBUG){digitalWrite(LED, LOW);} // indicate on board
  debugMsg("Run time: " + String(runtime)); // read out poofer on time
}

//////////////////////////////////////////////////
         /* Neopixel Camera Functions */
//////////////////////////////////////////////////


void pixelShow() {
  int randomShow = random(0,3);
  if(randomShow == 0) {
    theaterChaseRainbow(50);
  }else if(randomShow == 1) {
    colorWipe(strip.Color(random(0,255), random(0,255), random(0,255)), random(10,50));
  }else if(randomShow == 2) {
     rainbow(random(10,50));
  }else if(randomShow == 3) {
    rainbowCycle(random(10,50));
  }
}

void pixelCamera() {
  if(digitalRead(BUTTON) == LOW ){
	colorWipe(strip.Color(255, 255, 255), 0); // Flash white
	focus();
	if(DEBUG){digitalWrite(LED, HIGH);} // indicate on board
	delay((int)wait*.667);
	takePhoto();
	delay((int)wait*.333);
    colorWipe(strip.Color(255, 0, 0), 10); // Red
  }
}

//////////////////////////////////////////////////
         /* Neopixel Light Functions */
//////////////////////////////////////////////////

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/* DEBUG FUNCTIONS */
void debugMsg(String message){
  if (DEBUG) { Serial.println(message); }
}

void debugPiece(String message){
  if (DEBUG) { Serial.print(message); }
}





