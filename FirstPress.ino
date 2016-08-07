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


const int   wait = 500;
bool        debug = true;
const int   accumulator = 10000;
unsigned long time, runtime = 0;




void setup() {
  pinMode(BUTTON, INPUT_PULLUP);   
  pinMode(FLAME1, OUTPUT);
  pinMode(FLAME2, OUTPUT);
  pinMode(CAMERA, OUTPUT);
  pinMode(LED, OUTPUT);
  if (DEBUG) { Serial.begin(9600); }
}

void loop() {
  // Check for button press
  // If the button is pressed
  // and the accumulator is full enough
  if(runtime < accumulator && digitalRead(BUTTON) == LOW){
    debugMsg("Button press!");
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
  }else{
    runtime--;
    // Otherwise, make sure the fire is off
    if(digitalRead(FLAME1) == HIGH){
      digitalWrite(FLAME1, LOW);
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

void activatePoofer() {
  
}



