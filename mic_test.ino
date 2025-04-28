/*
Connections 

SD Card Module       ESP32 
GND                  GND
Vcc                  VIn
MISO                 D19 
MOSI                 D23 
SCK                  D18 
CS                   D5

I2S MIC (INMP441)    ESP32
GND                  GND
VDD                  3.3V
SD                   D35                  
SCK                  D33
WS                   D22
L/R                  3.3V

Button               ESP32
Button (one leg)     D26
Button (other leg)   GND
*/

// *** HINT: in case of an 'Sketch too Large' Compiler Warning/ERROR in Arduino IDE (ESP32 Dev Module):
// -> select a larger 'Partition Scheme' via menu > tools: e.g. using 'No OTA (2MB APP / 2MB SPIFFS) ***

#define VERSION "\nak esp32 INMP441"

#include <SD.h>    // needed for SD card operations

#define SD_CS_PIN 5 
#define LED 2
#define RECORD_BTN 26

// Function declarations (from KALO library)
bool I2S_Record_Init();
bool Record_Start(String filename);
bool Record_Available(String filename, float* audiolength_sec);

// Global variables for button, recording state, and file naming
bool isRecording = false;
bool lastButtonState = HIGH;
String currentAudioFile = "";

// Generate next audio filename (audio1.wav, audio2.wav, etc.)
String getNextAudioFile() {
  int fileNum = 1;
  String fileName;
  do {
    fileName = "/audio" + String(fileNum) + ".wav";
    fileNum++;
  } while (SD.exists(fileName));
  return fileName;
}

// ------------------------------------------------------------------------------------------------------------------------------
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.setTimeout(100);  // Faster reaction after CR entered
  Serial.println("[DEBUG] Starting setup...");

  // Pin assignments
  pinMode(LED, OUTPUT);
  pinMode(RECORD_BTN, INPUT_PULLUP);  // Tactile button with internal pull-up
  Serial.println("[DEBUG] Pins initialized: LED (2), Button (26)");

  // Hello World
  Serial.println(VERSION);

  // Initialize SD card
  Serial.println("[DEBUG] Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("[ERROR] SD Card initialization failed!");
    return;
  }
  Serial.println("[DEBUG] SD Card initialized successfully");

  // Initialize I2S Recording Services
  Serial.println("[DEBUG] Initializing I2S recording...");
  if (!I2S_Record_Init()) {
    Serial.println("[ERROR] I2S recording initialization failed!");
    return;
  }
  Serial.println("[DEBUG] I2S recording initialized");

  // INIT done
  Serial.println("[DEBUG] Setup complete. Waiting for button press to start/stop recording...");
}

// ------------------------------------------------------------------------------------------------------------------------------
void loop() {
  bool currentButtonState = digitalRead(RECORD_BTN);

  // Detect button press (falling edge)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    Serial.println("[DEBUG] Button pressed!");
    
    if (!isRecording) {
      // Start recording with a new file
      currentAudioFile = getNextAudioFile();
      Serial.println("[DEBUG] Starting recording to " + currentAudioFile + "...");
      digitalWrite(LED, HIGH);
      isRecording = true;
      if (!Record_Start(currentAudioFile)) {
        Serial.println("[ERROR] Failed to start recording!");
        isRecording = false;
        digitalWrite(LED, LOW);
        return;
      }
      Serial.println("[DEBUG] Recording started successfully");
    } else {
      // Stop recording
      Serial.println("[DEBUG] Stopping recording...");
      isRecording = false;
      digitalWrite(LED, LOW);
      float recorded_seconds;
      if (Record_Available(currentAudioFile, &recorded_seconds)) {
        Serial.printf("[DEBUG] Recording saved to %s. Duration: %.2f seconds\n", currentAudioFile.c_str(), recorded_seconds);
        if (recorded_seconds <= 0.4) {
          Serial.println("[DEBUG] Recording too short (<=0.4s), ignoring");
        }
      } else {
        Serial.println("[ERROR] Recording not available or failed to save!");
      }
      currentAudioFile = "";  // Reset for next recording
    }
    delay(50);  // Simple debounce
  }

  // Continue capturing I2S data if recording
  if (isRecording) {
    if (!Record_Start(currentAudioFile)) {
      Serial.println("[ERROR] Failed to append I2S data!");
      isRecording = false;
      digitalWrite(LED, LOW);
    }
  }

  lastButtonState = currentButtonState;  // Update button state
}