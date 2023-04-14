#include <ArduinoJson.h>
#include <Stepper.h>
#include <SD.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <TMRpcm.h>

// Constants
const int SD_ChipSelectPin = 10;
const int pin1 = 2;
const int pin2 = 3;
const int pin3 = 4;
const int pin4 = 5;
const int delaytime = 5;
const int counts_per_pill = 100;
const int hours = 0;
const int minutes = 1;
const int AM_PM = 2;
const int connecting = 0;
const int disconnecting = 1;
const int cancel = 2;
const int set = 3;
const int Exit = 4;
const char* pill_1 = "Pill 1";
const char* pill_2 = "Pill 2";
const char* pill_3 = "Pill 3";
const char* pill_4 = "Pill 4";
const char* pill_5 = "Pill 5";
const char* pill_6 = "Pill 6";
const char* pill_7 = "Pill 7";
const char* pill_8 = "Pill 8";
const char* pill_9 = "Pill 9";
const char* pill_10 = "Pill 10";
const char* pill_11 = "Pill 11";
const char* pill_12 = "Pill 12";
const char* pill_13 = "Pill 13";
const char* pill_14 = "Pill 14";
const char* empty = "empty";
const char* setted = "setted";
const int app_state_disconnected = 0;
const int app_state_connected = 1;

// Global variables
TMRpcm tmrpcm;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
volatile int counter = 0;
volatile int Minutes = 0;
volatile int Hours = 0;
volatile int carry = 0;
volatile int ampm = 0;
byte memory[100];
int app_state = app_state_disconnected;

// Stepper motor configuration
Stepper stepper(200, pin1, pin3, pin2, pin4);

// WiFi and Web server configurations
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
ESP8266WebServer server(80);
const int httpPort = 80;
String serverPath = "/json";
String serverHost = "your_SERVER_HOST";
WiFiClient client;

// Function prototypes
void dispensePill(int pillNumber);
void loadMemory();
void saveMemory();
void printTime();
void printPillStates();
void playSound();
void connectToWiFi();
void handleRequest();

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Connect to Wi-Fi network
  connectToWiFi();
  
  // Initialize SD card module
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  
  // Initialize EEPROM
  EEPROM.begin(100);
  
  // Load memory from EEPROM
  loadMemory();
  
  // Initialize LCD screen
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Pill Disp");
            lcd.print(pillName);
          lcd.print(" set");
        }
      } else {
        Serial.println("Error: Failed to retrieve JSON file.");
      }
      break;

    case Exit:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pill Dispenser");
      lcd.setCursor(0, 1);
      lcd.print("Exiting...");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Goodbye!");
      delay(1000);
      break;
    }
  }

  // Check if it's time to dispense a pill
  if (counter == 60) {
    counter = 0;

    for (int i = 1; i <= 14; i++) {
      int hour = memory[i * 3];
      int minute = memory[i * 3 + 1];
      int amPm = memory[i * 3 + 2];
      bool dispensed = memory[i * 3 + 3];

      if (!dispensed && Hours == hour && Minutes == minute && ampm == amPm) {
        dispensePill(i);
        memory[i * 3 + 3] = 1;
        saveMemory();
      }
    }
  }
}

void dispensePill(int pillNumber) {
  // Rotate stepper motor to dispense pill
  stepper.step(counts_per_pill);

  // Print message to LCD screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing Pill");
  lcd.setCursor(0, 1);
  lcd.print("Pill ");
  lcd.print(pillNumber);
  lcd.print(" Dispensed");

  // Play sound
  playSound();

  // Wait for 5 seconds
  delay(5000);
}

void loadMemory() {
  for (int i = 0; i < 100; i++) {
    memory[i] = EEPROM.read(i);
  }
}

void saveMemory() {
  for (int i = 0; i < 100; i++) {
    EEPROM.write(i, memory[i]);
  }
}

void printTime() {
  // Print current time to LCD screen
  lcd.setCursor(0, 0);
  if (Hours < 10) {
    lcd.print("0");
  }
  lcd.print(Hours);
  lcd.print(":");
  if (Minutes < 10) {
    lcd.print("0");
  }
  lcd.print(Minutes);
  lcd.print(" ");
  if (ampm == 0) {
    lcd.print("AM");
  } else {
    lcd.print("PM");
  }
}

void printPillStates() {
  // Print pill states to LCD screen
  lcd.setCursor(0, 1);
  for (int i = 1; i <= 14; i++) {
    if (memory[i * 3 + 3] == 0) {
      lcd.print("O");
    } else {
      lcd.print("X");
    }
  }
}

void playSound() {
  // Play sound using speaker
  tmrpcm.play("sound.wav");
}

ISR(TIMER1_COMPA_vect) {
  // Increment time variables
  counter++;
  Minutes++;
  if (Minutes == 60) {
    Minutes = 0;
    Hours++;
    if (Hours == 12) {
      carry++;
    }
    if (Hours == 13) {
      Hours = 1;
    }
  }
  if (carry % 2 == 0) {
    ampm = 0
  } else {
    ampm = 1;
  }
  
  // Print current time and pill states to LCD screen
  printTime();
  printPillStates();
}

void dispensePill(int pillNumber) {
  // Rotate stepper motor to dispense pill
  stepper.step(counts_per_pill);
  
  // Print message to LCD screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing Pill");
  lcd.setCursor(0, 1);
  lcd.print("Pill ");
  lcd.print(pillNumber);
  lcd.print(" Dispensed");
  
  // Play sound
  playSound();
  
  // Wait for 5 seconds
  delay(5000);
}

void loadMemory() {
  for (int i = 0; i < 100; i++) {
    memory[i] = EEPROM.read(i);
  }
}

void saveMemory() {
  for (int i = 0; i < 100; i++) {
    EEPROM.write(i, memory[i]);
  }
}

void printTime() {
  // Print current time to LCD screen
  lcd.setCursor(0, 0);
  if (Hours < 10) {
    lcd.print("0");
  }
  lcd.print(Hours);
  lcd.print(":");
  if (Minutes < 10) {
    lcd.print("0");
  }
  lcd.print(Minutes);
  lcd.print(" ");
  if (ampm == 0) {
    lcd.print("AM");
  } else {
    lcd.print("PM");
  }
}

void printPillStates() {
  // Print pill states to LCD screen
  lcd.setCursor(0, 1);
  for (int i = 1; i <= 14; i++) {
    if (memory[i * 3 + 3] == 0) {
      lcd.print("O");
    } else {
      lcd.print("X");
    }
  }
}

void playSound() {
  // Play sound using speaker
  tmrpcm.play("sound.wav");
}

ISR(TIMER1_COMPA_vect) {
  // Increment time variables
  counter++;
  Minutes++;
  if (Minutes == 60) {
    Minutes = 0;
    Hours++;
    if (Hours == 12) {
      carry++;
    }
    if (Hours == 13) {
      Hours = 1;
    }
  }
  if (carry % 2 == 0) {
    ampm = 0;
  } else {
    ampm = 1;
  }
  
  // Print current time and pill states to LCD screen
  printTime();
  printPillStates();
}
