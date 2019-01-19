#include <Wire.h>
#include "rgb_lcd.h"

int soundPin = 6;
int movePin = 5;

int vals[6];
int ptr = 0;

int melody[] = {
  2500, 2000, 2500
};
int noteDurations[] = {
  4, 3, 4
};

rgb_lcd lcd;


// Set Up and Loop:
void setup()
{
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0); //set to green at beginning
  pinMode(6, OUTPUT); //D6 as piezobuzzer
  pinMode(2, INPUT); //D2 as PIR sensor

  delay(1000);
}



// WHAT HAPPENS TO THE DAMN THING

void loop() {
  Serial.println("Start first loop");
  while (!reserved()) {
    delay(500);
  }
  Serial.println("Reserved");

  reserve();
  Serial.println("Reserve implemented");
  bool armed = true;

  // reserve seat until disarmed
  // has time limit dependent on time to check disarmed/trespass
  // will alarm if trespassed, giving extra time
  for (int i = 0; i < 50; i++) {
    if (disarmed()) {
      Serial.println("Disarmed");
      armed = false;
      break;
    }
    if (trespassed()) {
      Serial.println("Trespassed");
      fuckyou();
    }
  }
  if (!armed) {
    bool taken = false;
    for(int i = 0; i < 10; i++){
      if (trespassed()) {
        taken = true;
        Serial.println("Occupied");
        break;
      }
    }
    
    
    if(taken == true) {
      welcome();
      Serial.println("Welcomed");
      for (int i = 0; i < 5; i++) {
        if (vacated()) {
          Serial.println("Vacated");
          taken = false;
          break;
        }
        if ((i == 4000) || (i == 4500) || (i == 4750)) {
          Serial.println("Taking long time");
          warn();
        }
      }
    }
    
    if(taken == true){
      Serial.println("Took too long");
      fuckyou();
    }
  }
  Serial.println("Resetting");
  
  reset();
}




// INPUT ADJUSTMENTS

// Telegram inputs - TODO!!!!!!!!
bool reserved() {
  delay(10);
  return true;
}

bool disarmed() {
  delay(10);
  return false;
}

// Sensor inputs
int movement() {
  int c = 0;
  for(int i = 0; i < 350; i++) {
    c = c + digitalRead(movePin);
    delay(7);
  }
  Serial.println(c);
  return c;
}

bool vacated() {
  if (movement() < 200) {
    return (movement() < 100);
  }
  return false;
}

bool trespassed() {
  return (movement() > 200);
}


// OUTPUT ADJUSTMENTS

void alert() {
  for (int thisNote = 0; thisNote < 3; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(soundPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(soundPin);
  }
}

// nobody seated
void reset()
{
  alert();
  lcd.setRGB(0, 255, 0);
  lcd.print("This seat is vacant");
  lcd.setCursor(0, 1);
  lcd.print("Reserve me on telegram or press the button");
}

// reserved
void reserve()
{
  alert();
  lcd.setRGB(255, 137, 0);
  lcd.print("This seat is reserved");
  lcd.setCursor(0, 1);
  lcd.print("Please find another location.");
}

// people seated
void welcome()
{
  alert();
  lcd.setRGB(0, 0, 255);
  lcd.print("Welcome to Food Court");
  lcd.setCursor(0, 1);
  lcd.print("Please enjoy your meal!");
}

// people taking up too much time
void warn()
{
  alert();
  lcd.setRGB(0, 0, 255);
  lcd.print("Your time is almost up");
  lcd.setCursor(0, 1);
  lcd.print("Please give up your seats");
}

// tresspassers seated
void alarm() {
  Serial.println("alarm");
  int i = 0;
  // while ((!vacated()) && (i > 300)) {
  while(!vacated() && (i < 300)) {
    Serial.println("here");
    // tone(soundPin, 3100, 500);
    delay(500);
    noTone(soundPin);
    delay(100);
    i = i + 1;
  }
}

void fuckyou()
{
  lcd.setRGB(255, 0, 0);
  lcd.print("Please vacate the seat");
  lcd.setCursor(0, 1);
  lcd.print("This seat has been reserved by another user");

  alarm();
}





