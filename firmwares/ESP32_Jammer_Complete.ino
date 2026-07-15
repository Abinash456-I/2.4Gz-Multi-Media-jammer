
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ============= PIN DEFINITIONS =============
// NRF24L01 Module 1 Pins (for Wi-Fi jamming)
#define CE_PIN1 22
#define CSN_PIN1 21

// NRF24L01 Module 2 Pins (for Bluetooth jamming)
#define CE_PIN2 16
#define CSN_PIN2 15

// Control pins
#define POWER_BUTTON_PIN 4     // GPIO 4 for Power ON/OFF
#define MODE_BUTTON_PIN 5      // GPIO 5 for Mode selection
#define BUZZER_PIN 12          // GPIO 12 for 5V buzzer
#define BATTERY_ADC_PIN 35     // GPIO 35 (ADC) for battery voltage reading

// ============= LCD SETUP =============
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27 for 16x2 LCD (change if needed: 0x3F)

// ============= RF24 INSTANCES =============
RF24 wifiJammer(CE_PIN1, CSN_PIN1);
RF24 btJammer(CE_PIN2, CSN_PIN2);

// ============= PAYLOAD & CHANNELS =============
const char interferencePayload[32] = "NOISE_INTERFERENCE_DATA";

// Bluetooth channels (79 channels)
const uint8_t btChannels[79] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
  22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 
  42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 
  62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78
};

// Wi-Fi channels (11 channels)
const uint8_t wifiChannels[11] = {
  1, 6, 11, 2, 7, 3, 8, 4, 9, 5, 10
};

// RC Drone channels (8 channels)
const uint8_t rcDroneChannels[8] = {
  2, 4, 6, 8, 10, 12, 14, 16
};

// ============= MODE & STATE DEFINITIONS =============
enum Mode {
  IDLE = 0,
  WIFI_JAMMING = 1,
  BT_JAMMING = 2,
  RC_DRONE_JAMMING = 3
};

enum PowerState {
  OFF,
  ON
};

// ============= GLOBAL VARIABLES =============
Mode currentMode = IDLE;
PowerState powerState = OFF;

unsigned long lastPowerButtonTime = 0;
unsigned long lastModeButtonTime = 0;
unsigned long lastBatteryUpdateTime = 0;

const unsigned long DEBOUNCE_DELAY = 50;
const unsigned long STATUS_DISPLAY_DURATION = 1000;
const unsigned long BATTERY_UPDATE_INTERVAL = 5000;

// Battery variables
float batteryPercentage = 100.0;
const float MAX_BATTERY_VOLTAGE = 4.2;
const float MIN_BATTERY_VOLTAGE = 2.8;
const int ADC_MAX = 4095;

// Delays
const unsigned long channelDelay = 5;
const unsigned long loopDelay = 50;

// ============================================================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nESP32 Jammer System Starting...");

  // Initialize pins
  pinMode(POWER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize I2C LCD
  Wire.begin(32, 33); // SDA=32, SCL=33
  lcd.init();
  lcd.backlight();
  lcd.print("Jammer System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // Initialize NRF24L01 Module 1 (Wi-Fi)
  if (!wifiJammer.begin()) {
    Serial.println("Wi-Fi jammer module not detected!");
    lcd.print("WiFi Module");
    lcd.setCursor(0, 1);
    lcd.print("NOT FOUND!");
    delay(2000);
  } else {
    Serial.println("Wi-Fi jammer module initialized.");
    wifiJammer.setPALevel(RF24_PA_HIGH);
    wifiJammer.setDataRate(RF24_2MBPS);
    wifiJammer.stopListening();
  }

  // Initialize NRF24L01 Module 2 (Bluetooth)
  if (!btJammer.begin()) {
    Serial.println("Bluetooth jammer module not detected!");
    lcd.print("BT Module");
    lcd.setCursor(0, 1);
    lcd.print("NOT FOUND!");
    delay(2000);
  } else {
    Serial.println("Bluetooth jammer module initialized.");
    btJammer.setPALevel(RF24_PA_HIGH);
    btJammer.setDataRate(RF24_1MBPS);
    btJammer.stopListening();
  }

  lcd.clear();
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Battery: 100%");
  delay(2000);
  lcd.clear();

  Serial.println("Setup complete!");
}

// ============================================================================
void loop() {
  checkPowerButton();
  checkModeButton();

  if (powerState == OFF) {
    if (millis() - lastBatteryUpdateTime >= BATTERY_UPDATE_INTERVAL) {
      updateBatteryDisplay();
      lastBatteryUpdateTime = millis();
    }
  }

  if (powerState == ON) {
    executeJamming();
  }

  delay(loopDelay);
}

// ============================================================================
// BUTTON HANDLING
// ============================================================================
void checkPowerButton() {
  bool buttonState = digitalRead(POWER_BUTTON_PIN);

  if (buttonState == LOW && (millis() - lastPowerButtonTime) > DEBOUNCE_DELAY) {
    lastPowerButtonTime = millis();

    powerState = (powerState == OFF) ? ON : OFF;

    lcd.clear();
    if (powerState == ON) {
      lcd.print("ACTIVATED");
      beepBuzzer(1000); // 1 second beep
      currentMode = WIFI_JAMMING;
      Serial.println("Jammer ACTIVATED");
    } else {
      lcd.print("DEACTIVATED");
      noTone(BUZZER_PIN);
      currentMode = IDLE;
      Serial.println("Jammer DEACTIVATED");
    }

    delay(STATUS_DISPLAY_DURATION);
    lcd.clear();
    updateBatteryDisplay();
  }
}

void checkModeButton() {
  bool buttonState = digitalRead(MODE_BUTTON_PIN);

  if (buttonState == LOW && (millis() - lastModeButtonTime) > DEBOUNCE_DELAY) {
    lastModeButtonTime = millis();

    if (powerState == ON) {
      currentMode = static_cast<Mode>((currentMode + 1) % 4);
      if (currentMode == IDLE) {
        currentMode = WIFI_JAMMING;
      }

      lcd.clear();
      displayModeInfo();
      beepBuzzer(500);
      delay(1500);
      lcd.clear();
      updateBatteryDisplay();

      Serial.print("Mode changed to: ");
      Serial.println(currentMode);
    }
  }
}

// ============================================================================
// BUZZER FUNCTIONS
// ============================================================================
void beepBuzzer(unsigned long duration) {
  tone(BUZZER_PIN, 1000, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}

// ============================================================================
// LCD DISPLAY
// ============================================================================
void updateBatteryDisplay() {
  float batteryVoltage = readBatteryVoltage();
  batteryPercentage = calculateBatteryPercentage(batteryVoltage);

  lcd.clear();
  lcd.print("Battery:");
  lcd.setCursor(11, 0);
  lcd.print(batteryPercentage, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("V: ");
  lcd.print(batteryVoltage, 2);
  lcd.print("V");

  Serial.print("Battery: ");
  Serial.print(batteryPercentage, 1);
  Serial.print("% (");
  Serial.print(batteryVoltage, 2);
  Serial.println("V)");
}

void displayModeInfo() {
  lcd.print("Mode: ");
  switch (currentMode) {
    case WIFI_JAMMING:
      lcd.print("WiFi");
      break;
    case BT_JAMMING:
      lcd.print("Bluetooth");
      break;
    case RC_DRONE_JAMMING:
      lcd.print("RC Drone");
      break;
    default:
      lcd.print("Idle");
  }
  lcd.setCursor(0, 1);
  lcd.print("Jamming Active");
}

// ============================================================================
// BATTERY MONITORING
// ============================================================================
float readBatteryVoltage() {
  int adcValue = analogRead(BATTERY_ADC_PIN);
  float voltage = adcValue * (3.3 / ADC_MAX) * 2; // 2x for voltage divider
  return voltage;
}

float calculateBatteryPercentage(float voltage) {
  if (voltage >= MAX_BATTERY_VOLTAGE) {
    return 100.0;
  } else if (voltage <= MIN_BATTERY_VOLTAGE) {
    return 0.0;
  } else {
    float percentage = ((voltage - MIN_BATTERY_VOLTAGE) / (MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE)) * 100.0;
    return percentage;
  }
}

// ============================================================================
// JAMMING FUNCTIONS
// ============================================================================
void executeJamming() {
  switch (currentMode) {
    case WIFI_JAMMING:
      jamWiFi();
      break;
    case BT_JAMMING:
      jamBluetooth();
      break;
    case RC_DRONE_JAMMING:
      jamRCDrone();
      break;
    case IDLE:
      break;
  }
}

void jamWiFi() {
  for (int i = 0; i < 11; i++) {
    if (powerState == OFF) break;
    wifiJammer.setChannel(wifiChannels[i]);
    wifiJammer.write(&interferencePayload, sizeof(interferencePayload));
    delay(channelDelay);
  }
}

void jamBluetooth() {
  for (int i = 0; i < 79; i++) {
    if (powerState == OFF) break;
    btJammer.setChannel(btChannels[i]);
    btJammer.write(&interferencePayload, sizeof(interferencePayload));
    delay(channelDelay);
  }
}

void jamRCDrone() {
  for (int i = 0; i < 8; i++) {
    if (powerState == OFF) break;
    wifiJammer.setChannel(rcDroneChannels[i]);
    wifiJammer.write(&interferencePayload, sizeof(interferencePayload));
    delay(channelDelay);
  }
}