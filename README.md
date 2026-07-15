# ESP32 WiFi & Bluetooth Jammer 📡

A comprehensive RF interference system built on the ESP32 microcontroller with dual NRF24L01 modules for jamming WiFi, Bluetooth, and RC drone communications.

---

## ⚠️ **LEGAL DISCLAIMER**

This project is for **educational and research purposes only**. Unauthorized jamming of radio frequencies is **illegal** in most countries and violates:
- FCC regulations (USA)
- OFCOM rules (UK)
- Local radio communication laws

**Use only in controlled environments with proper authorization.**

---

## 📋 **Features**

✅ **Triple-Mode Jamming:**
- WiFi 2.4GHz jamming (11 channels)
- Bluetooth jamming (79 channels)
- RC Drone frequency jamming (8 channels)

✅ **User Interface:**
- 16×2 LCD display with I2C interface
- Power ON/OFF button with status feedback
- Mode selection button for switching between jamming types
- Real-time battery percentage display

✅ **Battery Management:**
- ADC-based battery voltage monitoring
- Calculates percentage (0-100%)
- Displays voltage and percentage on LCD
- Low-power idle mode

✅ **Audio Feedback:**
- 5V active buzzer
- 1-second activation beep
- Mode change notification beep

✅ **Dual RF24 Modules:**
- Independent control of two NRF24L01+ transceivers
- High-power PA level transmission
- Optimized data rates for each frequency band

---

## 🔌 **Hardware Requirements**

### Core Components
| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 DevKit V1 | 1 | Main microcontroller |
| NRF24L01+ | 2 | RF transmission modules |
| 16×2 LCD (I2C) | 1 | Display interface |
| 5V Active Buzzer | 1 | Audio feedback |
| Push Button (6mm) | 2 | Power & Mode controls |
| 3.7V Li-Ion Battery | 1 | 1000mAh or higher |
| TP4056 Module | 1 | Battery charging |

### Support Components
| Component | Value/Type | Purpose |
|-----------|-----------|---------|
| Capacitor | 10µF | NRF24 power stabilization |
| Resistor | 100Ω | Buzzer current limiting |
| Resistor | 100kΩ | Voltage divider (×2) |
| Jumper Wires | Various | Connections |
| Breadboard | 830 holes | Component mounting |

---

## 🔗 **Pin Connections**

### NRF24L01 Module 1 (WiFi Jamming)
NRF24 #1 → ESP32 VCC → 3.3V GND → GND CE → GPIO 22 CSN → GPIO 21 MOSI → GPIO 23 (SPI) MISO → GPIO 19 (SPI) SCK → GPIO 18 (SPI)

Code

### NRF24L01 Module 2 (Bluetooth Jamming)
NRF24 #2 → ESP32 VCC → 3.3V (with 10µF cap) GND → GND CE → GPIO 16 CSN → GPIO 15 MOSI → GPIO 23 (SPI - Shared) MISO → GPIO 19 (SPI - Shared) SCK → GPIO 18 (SPI - Shared)

Code

### Control Interface
Component → ESP32 Power Button → GPIO 4 (with pull-up) Mode Button → GPIO 5 (with pull-up) Buzzer (+) → GPIO 12 (via 100Ω resistor) Buzzer (GND) → GND Battery ADC → GPIO 35 (via voltage divider)

Code

### LCD (I2C)
LCD I2C Module → ESP32 SDA → GPIO 32 SCL → GPIO 33 VCC → 5V GND → GND

Code

### Battery Connection
Battery → TP4056 Module → ESP32 Li-Ion + → IN+ → (Charger) Li-Ion - → IN- → GND → OUT+ → VBAT (with divider) → OUT- → GND

Code

---

## 📦 **Installation**

### Step 1: Install Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software)

### Step 2: Add ESP32 Board Support
1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add this URL to "Additional Board Manager URLs":
https://dl.espressif.com/dl/package_esp32_index.json

Code
4. Go to **Tools → Board → Boards Manager**
5. Search for "esp32" and install by Espressif Systems

### Step 3: Install Required Libraries
In Arduino IDE, go to **Sketch → Include Library → Manage Libraries** and install:

- **RF24** by TMRh20 (v1.4.7 or higher)
- **LiquidCrystal_I2C** by Frank de Brabander
- 
# Alternative: Install via command line
# Replace [Library_Name] with the library name above
Step 4: Configure Board Settings
In Arduino IDE:

Tools → Board → Select "ESP32 Dev Module"
Tools → Flash Size → 4MB
Tools → Upload Speed → 115200
Tools → Port → Select your COM port
Step 5: Upload Code
Copy the complete code from ESP32_Jammer_Complete.ino
Paste into Arduino IDE
Click Upload button (⬆️)
Wait for "Upload Complete" message
💻 Code Overview
Main Components
Pin Definitions
C++
// NRF24 Module 1 (WiFi)
#define CE_PIN1 22
#define CSN_PIN1 21

// NRF24 Module 2 (Bluetooth)
#define CE_PIN2 16
#define CSN_PIN2 15

// Control Interface
#define POWER_BUTTON_PIN 4
#define MODE_BUTTON_PIN 5
#define BUZZER_PIN 12
#define BATTERY_ADC_PIN 35
Operating Modes
C++
enum Mode {
  IDLE = 0,
  WIFI_JAMMING = 1,
  BT_JAMMING = 2,
  RC_DRONE_JAMMING = 3
};
Channel Definitions
WiFi Channels (11): 1, 6, 11, 2, 7, 3, 8, 4, 9, 5, 10

Bluetooth Channels (79): 2-78 (all Bluetooth LE channels)

RC Drone Channels (8): 2, 4, 6, 8, 10, 12, 14, 16

🎮 Usage Guide
Power Control
Press Power Button → System activates with 1-second beep
LCD displays → "ACTIVATED" message
Press Again → System deactivates with 1-second beep
LCD displays → "DEACTIVATED" message
Mode Selection
(Only works when system is ON)

Press Mode Button → Cycles through modes with beep notification
Mode 1 → WiFi 2.4GHz jamming
Mode 2 → Bluetooth 2.4GHz jamming
Mode 3 → RC Drone frequency jamming
Battery Monitoring
When OFF → LCD shows battery percentage and voltage
Updates every 5 seconds
Voltage range → 2.8V (0%) to 4.2V (100%)
Current reading displayed on second line
LCD Display Modes
Power OFF State:

Code
Battery: 85%
V: 3.85V
Active Jamming:

Code
Mode: WiFi
Jamming Active
🔧 Troubleshooting
Issue: LCD Not Displaying
Solution:

Check I2C address (default: 0x27, alternative: 0x3F)
Verify SDA (GPIO 32) and SCL (GPIO 33) connections
Use I2C scanner code to find correct address
Issue: NRF24 Modules Not Detected
Solution:

Verify 10µF capacitors are connected to VCC
Check CE and CSN pin connections
Ensure SPI pins (18, 19, 23) are not conflicts
Test with RF24 example code
Issue: Battery Reading Shows 0%
Solution:

Check voltage divider resistor values (100kΩ + 100kΩ)
Verify connection to GPIO 35
Ensure battery is charged to 3.7V+
Issue: Buzzer Not Beeping
Solution:

Check GPIO 12 connection
Verify 100Ω current-limiting resistor
Test with digitalWrite(12, HIGH/LOW)
Buzzer polarity: + to GPIO 12, - to GND
Issue: Button Presses Not Registering
Solution:

Confirm pull-up resistors enabled in code
Check button connections to GND
Test with digitalRead() in Serial Monitor
Increase DEBOUNCE_DELAY if needed
📊 Performance Specifications
Parameter	Value
RF Power Output	0dBm (1mW)
Data Rate (WiFi)	2 Mbps
Data Rate (Bluetooth)	1 Mbps
Channel Switching Speed	5ms
LCD Refresh Rate	0.2Hz (off), varies when on
Battery Runtime	~4-6 hours (1000mAh)
Operating Temperature	0°C to 40°C
📝 Code Example: Custom Jamming
To add custom jamming frequencies:

C++
const uint8_t customChannels[5] = {1, 5, 9, 13, 17};

void jamCustom() {
  for (int i = 0; i < 5; i++) {
    if (powerState == OFF) break;
    wifiJammer.setChannel(customChannels[i]);
    wifiJammer.write(&interferencePayload, sizeof(interferencePayload));
    delay(channelDelay);
  }
}
🚀 Advanced Modifications
Increase Transmission Power
C++
wifiJammer.setPALevel(RF24_PA_MAX); // Maximum power (0 dBm)
Faster Channel Hopping
C++
const unsigned long channelDelay = 2; // Reduce from 5ms
Battery Low Warning
C++
if (batteryPercentage < 20) {
  beepBuzzer(200); // Short warning beep
  beepBuzzer(200);
}
📚 References
NRF24L01+ Datasheet
ESP32 Documentation
RF24 Library GitHub
2.4GHz Frequency Bands
📄 License
This project is provided for educational purposes only. Users are responsible for ensuring compliance with local radio frequency regulations.

🤝 Contributing
Contributions are welcome! Please:

Fork the repository
Create a feature branch
Submit a pull request with improvements
✉️ Support
For issues, questions, or suggestions, please open a GitHub issue or contact the maintainer.

Last Updated: January 2025
Version: 1.0.0
Status: Stable ✅

⚡ Quick Start Checklist
 Hardware assembled and wired correctly
 Arduino IDE installed with ESP32 board support
 RF24 and LiquidCrystal_I2C libraries installed
 Code uploaded successfully
 Power button toggles system on/off
 Mode button cycles between jamming modes
 LCD displays battery information
 Buzzer beeps on power toggle
 Both NRF24 modules detected in Serial Monitor
 System ready for testing
Happy Jamming! 🎯 (Responsibly and Legally)
