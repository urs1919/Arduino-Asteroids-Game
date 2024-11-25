#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 

// Pin Declaration
const int potXPin = A0;
const int potYPin = A1;

int potX = 0;
int potY = 0;

int screenX = 0;
int screenY = 0;

// Enemy values
bool alive[5] = {true, true, true, true, true};
int positions[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Shooting functionality
const int pinUp = 7;
uint8_t  stateUp = 0;

const int pinLeft = 6;
uint8_t  stateLeft = 0;

const int pinRight = 5;
uint8_t  stateRight = 0;

const int pinDown = 4;
uint8_t  stateDown = 0;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A5));

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);

  for(int i = 0; i<5; ++i) {
    int x = random(0,129);
    int y = random(0, 65);
    positions[i*2] = x;
    positions[i*2+1] = y;
    
  }
  
}

void loop() {
  display.clearDisplay();

  movement();
  drawEnemies();

  shoot();

  winScreen();


  display.display();
  delay(25);
}

inline void drawCross(int x, int y, int size) {
  display.drawLine(x-size, y, x+size, y, SSD1306_WHITE);
  display.drawLine(x, y-size, x, y+size, SSD1306_WHITE);
}

inline void movement() {
  potX = analogRead(potXPin);
  potY = analogRead(potYPin);

  screenX = map(potX, 0, 1024, 0, 128);
  screenY = map(potY, 0, 1024, 0, 64);

  drawCross(screenX, screenY, 5);
}

void drawEnemies() {
  for(int i = 0; i<5; ++i) {
    if(alive[i]) {
      display.fillRect(positions[i*2], positions[i*2+1], 10, 10, SSD1306_WHITE);
      
    }
  }
}

void winScreen() {
  while(!alive[0] && !alive[1] && !alive[2] && !alive[3] && !alive[4]) {
    display.clearDisplay();
    
    display.setTextSize(2);      // Text size (1 = normal, 2 = double, etc.)
    display.setTextColor(SSD1306_WHITE);  // Color of the text
    display.setCursor(16, 32);
    
    display.println("You Win!");
    display.display();
    delay(500);
  }
}


void shoot() {
  stateUp = digitalRead(pinUp);
  stateLeft = digitalRead(pinLeft);
  stateRight = digitalRead(pinRight);
  stateDown = digitalRead(pinDown);
  if(!stateUp) {
    display.drawLine(screenX, screenY, screenX, 0, SSD1306_WHITE);
    for(int i = 0; i<5; ++i) {
      if(screenX >= positions[i*2] && screenX <= (positions[i*2]+10) && positions[i*2+1] <= screenY && alive[i]) {
        alive[i] = false;
      }
    }
  }
  if(!stateLeft) {
    display.drawLine(screenX, screenY, 0, screenY, SSD1306_WHITE);
    for(int i = 0; i<5; ++i) {
      if(screenY >= positions[i*2+1] && screenY <= (positions[i*2+1]+10) && positions[i*2] <= screenX && alive[i]) {
        alive[i] = false;
      }
    }
  }
  if(!stateRight) {
    display.drawLine(screenX, screenY, SCREEN_WIDTH, screenY, SSD1306_WHITE);
    for(int i = 0; i<5; ++i) {
      if(screenY >= positions[i*2+1] && screenY <= (positions[i*2+1]+10) && positions[i*2] >= screenX && alive[i]) {
        alive[i] = false;
      }
    }
  }
  if(!stateDown) {
    display.drawLine(screenX, screenY, screenX, SCREEN_HEIGHT, SSD1306_WHITE);
    for(int i = 0; i<5; ++i) {
      if(screenX >= positions[i*2] && screenX <= (positions[i*2]+10) && positions[i*2+1] >= screenY && alive[i]) {
        alive[i] = false;
      }
    }
  }
}

