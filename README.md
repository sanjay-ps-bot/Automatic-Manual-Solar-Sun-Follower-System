🌞 ESP32 Advanced Smart Solar Tracker
🚀 Project Overview

ESP32 Advanced Smart Solar Tracker is an intelligent IoT-based solar tracking system that automatically follows the direction of maximum sunlight using dual LDR sensors and a servo motor. It also provides a modern Wi-Fi web dashboard, a realistic animated OLED display, and manual control options.

The ESP32 creates its own Wi-Fi hotspot, allowing any mobile device to connect and control the system without requiring an internet connection.

✨ Features
☀️ Automatic Sun Tracking
Uses two LDR sensors (Left & Right) to detect sunlight direction.
Automatically rotates the servo motor toward the brightest light source.
Smart dead-zone algorithm reduces unnecessary servo movement and shaking.
🎮 Manual Servo Control
Control the solar panel angle manually using a web dashboard.
Smooth angle adjustment from 0° to 180°.
Instant response from the ESP32 web server.
🌐 Wi-Fi Hotspot Web Dashboard

The ESP32 creates a private Wi-Fi network:

SSID: SUN_TRACKER_ESP32
Password: 12345678
IP Address: 192.168.4.1

Dashboard Features:

🌌 Space-themed animated background
🌞 Glowing sun animation
📱 Mobile-friendly responsive interface
🎚️ Servo angle slider
🤖 AUTO / MANUAL mode switching
📊 Live LDR sensor values
📐 Real-time servo angle display
💡 LED status indicator
🖥️ OLED Realistic Animation

The OLED (128×64 SSD1306) displays a miniature solar environment:

       ☁️      ☁️

          ☀️
       \  |  /
        \ | /
----------------
          🔋
       Solar Panel
Animation Elements
🌞 Moving sun based on LDR readings
☁️ Animated moving clouds
🌅 Horizon/ground line
🔋 Solar panel graphic
🧭 Direction indication
📊 System status display
🔧 Components Required
Component	Quantity
ESP32 Development Board	1
LDR Module	2
SG90/MG90 Servo Motor	1
SSD1306 128×64 OLED Display	1
LED	1
220Ω Resistor	1
Breadboard	1
Jumper Wires	As required
External 5V Power Supply	1
🔌 Circuit Connections
LDR Sensors
LDR	ESP32
Left AO	GPIO 34
Right AO	GPIO 35
VCC	3.3V
GND	GND
Servo Motor
Servo Wire	Connection
Signal	GPIO 18
VCC	External 5V
GND	Common GND with ESP32

⚠️ Important: Do not power the servo directly from the ESP32. Use a separate 5V supply and connect the grounds together.

OLED Display (I2C)
OLED Pin	ESP32
SDA	GPIO 21
SCL	GPIO 22
VCC	3.3V
GND	GND
LED Indicator
LED	ESP32
Anode (+)	GPIO 2 through 220Ω resistor
Cathode (-)	GND
🧠 System Working
The ESP32 reads both LDR sensor values.
It compares the light intensity from the left and right sensors.
The servo rotates toward the brighter side.
The OLED displays a moving animated sun that represents the tracking direction.
The LED turns ON when sufficient sunlight is detected.
Users can switch between AUTO and MANUAL modes using the Wi-Fi dashboard.
📚 Required Arduino Libraries

Install these libraries from Arduino IDE Library Manager:

ESP32Servo
Adafruit SSD1306
Adafruit GFX
WiFi (ESP32 Built-in)
WebServer (ESP32 Built-in)
Wire (ESP32 Built-in)
📱 How to Use
1. Upload the Code

Upload the program to the ESP32 using Arduino IDE.

2. Power the System
Power ESP32 via USB.
Power the servo using an external 5V source.
3. Connect to ESP32 Wi-Fi
Wi-Fi Name: SUN_TRACKER_ESP32
Password: 12345678
4. Open the Dashboard

Open your mobile browser and visit:

http://192.168.4.1
5. Control the System
AUTO Mode → Solar panel follows sunlight automatically.
MANUAL Mode → Adjust the servo angle using the slider.
📂 Project Structure
ESP32_Advanced_Solar_Tracker/
│
├── ESP32_Advanced_Solar_Tracker.ino
├── README.md
│
├── Hardware
│   ├── ESP32
│   ├── Dual LDR Sensors
│   ├── Servo Motor
│   ├── SSD1306 OLED
│   └── LED Indicator
│
└── Features
    ├── Wi-Fi Hotspot Dashboard
    ├── Automatic Sun Tracking
    ├── Manual Servo Control
    ├── OLED Animation
    └── Real-time Monitoring
🌟 Future Improvements
🔋 Real solar panel integration
🌦️ Weather monitoring sensors
📡 Remote monitoring over the Internet
📈 Data logging and analytics
🌙 Automatic night parking mode
⚡ Battery charging management
🛠️ Developed By

Sanjay PS
Electronics • IoT • Embedded Systems • Space Technology Enthusiast

⭐ If you like this project

Give this repository a ⭐ on GitHub and support more IoT and space-inspired projects!
