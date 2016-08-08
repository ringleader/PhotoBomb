//      The Photobomb v1.0       //
// Software by James Villalpando //

// use NodeMCU 1.0 chip for ESP
// use generic Arduino Uno for Metro

// Set up the button and solenoid
// Define wait time and camera button
#define BUTTON  D6
#define FLAME1  D5
#define FLAME2  0
#define CAMERA  2
#define LED     D4
#define DEBUG   1
#define OVERRIDE D7


const int   wait = 500;
bool        debug = true;
const int   accumulator = 10000;
unsigned long time, runtime = 0;





void setup() {
  pinMode(BUTTON, INPUT_PULLUP);   
  pinMode(OVERRIDE, INPUT_PULLUP);
  pinMode(FLAME1, OUTPUT);
  pinMode(FLAME2, OUTPUT);
  pinMode(CAMERA, OUTPUT);
  pinMode(LED, OUTPUT);
  if (DEBUG) { Serial.begin(9600); }
  debugMsg("Photobomb Operational!");
}

void loop() {
  // If the button is pressed
  // and the accumulator is full enough
  if(runtime < accumulator && digitalRead(BUTTON) == LOW  && digitalRead(OVERRIDE) == HIGH ){
    debugMsg("Button press!");
    debugMsg(String(digitalRead(OVERRIDE)));
    takePhoto();
  }else if(digitalRead(BUTTON) == LOW && digitalRead(OVERRIDE) == LOW){
    digitalWrite(FLAME1, HIGH);
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
    if(digitalRead(FLAME1) == HIGH){
      digitalWrite(FLAME1, LOW);
      digitalWrite(LED, LOW);
    }
  }
}

/* FUNCTIONS */
void puffUp(int times, bool finish){
  // flash `times` at a `rate` and `finish` in which state
  for(int i = 0;i <= times ;i++){
    digitalWrite(FLAME1, finish);
    time = millis();
    digitalWrite(LED, finish);
    delay(100);
    digitalWrite(FLAME1, !finish);
    runtime += millis() - time;
    debugMsg("Run time: " + String(runtime));
    digitalWrite(LED, !finish);
    delay(500);
  }
}

/* DEBUG FUNCTIONS */
void debugMsg(String message){
  if (DEBUG) { Serial.println(message); }
}

void debugPiece(String message){
  if (DEBUG) { Serial.print(message); }
}

void takePhoto() {
  // Puff for attention
    puffUp(3,LOW);                    // three puffs
    delay(1500);                      // pause for dramatic effect
    // turn on the flame
    digitalWrite(FLAME1, HIGH);       // FIRE!
    time = millis(); // start timer
    if(DEBUG){digitalWrite(LED, HIGH);} // indicate on board
    delay(wait);
    digitalWrite(CAMERA, HIGH);       // snap a photo
    delay(5);     
    digitalWrite(CAMERA, LOW);        // turn off camera snapper
    // turn off flame
    digitalWrite(FLAME1, LOW);        // turn off flame
    runtime += millis() - time;       // calculate accumulator depletion
    if(DEBUG){digitalWrite(LED, LOW);} // indicate on board
    debugMsg("Run time: " + String(runtime)); // read out poofer on time
}



