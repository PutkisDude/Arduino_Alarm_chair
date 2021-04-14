// HÄLYTTÄVÄ TOIMISTOTUOLI - TEAM UNICORN
// Version 0.2

const int RED_LED = 6; // Red led in digital port 6
const int BUZZER = 7; // Buzz in digital port 7
const int SWITCHER = 9; // Switch in digital port 9
const int GREEN_LED = 11; // Led in digital port 11
const double minute = 60; // constant value to turn seconds to minutes.

int switchState = 0; // State of Switch - Don't change value manually

// Values for timers
double timeToStand =   minute * 60; // Minutes to sit before alarm turn on - Change only number value
double timeToSit =  minute * 10; // Minutes to stand before alarm turns off - Change only number value

// Variables to count times.
double sitTime = 0; // How long user has sat on chair - Don't change this manually.
double standTime = 0; // How long user has stood when alarm turned on. Don't change this manually.
double afkTime = 0; // Reset timer if person is away from keyboard long enough 

// Setup I/O devices when arduino starts
void setup(){
  Serial.begin(9600); // Print to serial monitor while arduino plugged to pc
  pinMode(GREEN_LED, OUTPUT);  // Green LED to show that device is working and can sit
  pinMode(RED_LED, OUTPUT); // Red lED to show that alarm mode is activated
  pinMode(BUZZER, OUTPUT); // Device to make annoying noise
  pinMode(SWITCHER, INPUT); // Sensor to detect if person is sitting
}

// Alarm sound
void buzz(){
   tone(BUZZER, 1000); // Send 1000hz sound signal
  delay(1000);        // for 1 sec
  noTone(BUZZER);     // Stop sound
}

// The Alarm
void alarm(){
   digitalWrite(GREEN_LED, LOW);           // Turn Green LED off  
  digitalWrite(RED_LED, HIGH);            // Turn Red LED ON
 standTime = 0;                              // Set stand time to 0 
   while(standTime <= timeToSit){            // Loop until person afk long enough
     switchState = digitalRead(SWITCHER); // Check if person sit or not
       if(switchState == LOW) {            // If person sits: stop counting and keep beeping
          Serial.println("alarm beeping");  // Print nonsense to pc's monitor
          buzz();                          // Beep
          delay(1000);                     // wait 1sec
       }else{                              // If user afk
        delay(1000);                         // wait 1sec
         Serial.println("Afk time: ");    // Print nonsense to pc's monitor
         Serial.println(standTime);      // Print nonsense to pc's monitor
         standTime++;                      // add sec to stand time
   }
 }
  sitTime = 0;                          // reset sit time
  afkTime = 0;
  standTime = 0;
  digitalWrite(RED_LED, LOW);            // Turn Red LED OFF when loop turns off
}


// If user afk more than half of sitting time, timers reset
void afk(){
   while(switchState == HIGH && sitTime > 0){    // if user not sit or sit time is 0, start loop
     delay(1000);                                // wait 1sec
     switchState = digitalRead(SWITCHER);        // check if user sit
      Serial.println("AFK time:");                    // print crap to pc screen
     Serial.println(afkTime);                    // print crap to pc screen
     afkTime++;                                 // increase afk time by 1
       if(afkTime > (sitTime / 2)){            // if afk time is more than half of sit time:
         sitTime = 0;                          // reset sit time
         afkTime = 0;                          // reset afk time
         break;                                // break loop
     } 
   }
}

void loop(){
  
 switchState = digitalRead(SWITCHER); // Check status of switch

if(switchState == LOW){               // If person sits
    if(sitTime >= timeToStand) {      // If person sits too long:
      alarm();                        // Start Alarm function
    }else{                            // If person is allowed to still sit:
      delay(1000);                    // Wait 1sec
      sitTime++;                      // Increase sit time by 1
      Serial.println("Sit time: ");       // print junk to pc screen
      Serial.println(sitTime);        // print junk to pc screen
    }
}else{                                // If nobody sit:
   afk();                             // Start afk function
   digitalWrite(GREEN_LED, HIGH);     // Turns green led on
 }
}
