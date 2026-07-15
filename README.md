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
