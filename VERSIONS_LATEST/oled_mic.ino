#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

// === OLED Setup ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 26
#define OLED_SCL 25
TwoWire myI2C(1);  // I2C bus 1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myI2C, -1);

// === Pin Setup ===
#define SD_CS_PIN 5 
#define LED 2
#define RECORD_BTN 4

// === Function declarations (external mic lib) ===
bool I2S_Record_Init();
bool Record_Start(String filename);
bool Record_Available(String filename, float* audiolength_sec);

// === Global State ===
bool isRecording = false;
bool lastButtonState = HIGH;
String currentAudioFile = "";
int graphX = 0;  // For waveform animation

// === Get next audio filename ===
String getNextAudioFile() {
  int fileNum = 1;
  String fileName;
  do {
    fileName = "/audio" + String(fileNum) + ".wav";
    fileNum++;
  } while (SD.exists(fileName));
  return fileName;
}

// === Debug Print (Serial + OLED) ===
void debugMessage(String msg, bool clear = true) {
  Serial.println(msg);
  if (clear) display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(msg);
  display.display();
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  pinMode(LED, OUTPUT);
  pinMode(RECORD_BTN, INPUT_PULLUP);

  myI2C.begin(OLED_SDA, OLED_SCL, 400000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[ERROR] OLED not found!");
    while (1);
  }
  display.clearDisplay();

  debugMessage("ESP32 Mic Recorder\nInit SD...", true);

  if (!SD.begin(SD_CS_PIN)) {
    debugMessage("[ERROR] SD FAIL!");
    return;
  }
  debugMessage("SD OK\nInit I2S...", true);

  if (!I2S_Record_Init()) {
    debugMessage("[ERROR] I2S FAIL!");
    return;
  }
  debugMessage("Ready.\nPress button.", true);
}

// === Simulated waveform for OLED ===
void drawWaveform() {
  static int lastY = SCREEN_HEIGHT / 2;
  int newY = random(10, SCREEN_HEIGHT - 10);
  display.drawLine(graphX, lastY, graphX + 1, newY, SSD1306_WHITE);
  lastY = newY;
  graphX += 2;
  if (graphX >= SCREEN_WIDTH) {
    graphX = 0;
    display.clearDisplay();
  }
  display.display();
}

// === Main Loop ===
void loop() {
  bool currentButtonState = digitalRead(RECORD_BTN);

  if (currentButtonState == LOW && lastButtonState == HIGH) {
    Serial.println("[DEBUG] Button pressed!");

    if (!isRecording) {
      currentAudioFile = getNextAudioFile();
      debugMessage("Recording:\n" + currentAudioFile, true);
      digitalWrite(LED, HIGH);
      isRecording = true;

      if (!Record_Start(currentAudioFile)) {
        debugMessage("[ERROR] Record Fail!");
        isRecording = false;
        digitalWrite(LED, LOW);
        return;
      }
    } else {
      debugMessage("Stopping...", true);
      isRecording = false;
      digitalWrite(LED, LOW);

      float recorded_seconds;
      if (Record_Available(currentAudioFile, &recorded_seconds)) {
        String msg = "Saved:\n" + currentAudioFile + "\nDuration: " + String(recorded_seconds, 2) + "s";
        debugMessage(msg, true);
        if (recorded_seconds <= 0.4) {
          debugMessage("Too short,\nignored", true);
        }
      } else {
        debugMessage("Save failed!", true);
      }
      currentAudioFile = "";
      graphX = 0;
      delay(2000);
      debugMessage("Ready.\nPress button.");
    }
    delay(50);  // Debounce
  }

  // If recording, update waveform
  if (isRecording) {
    drawWaveform();  // Visual zig-zag
    Record_Start(currentAudioFile);  // Feed I2S mic
  }

  lastButtonState = currentButtonState;
}
