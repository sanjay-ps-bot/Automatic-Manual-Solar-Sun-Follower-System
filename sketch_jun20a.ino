/**************************************************
 * ESP32 SMART SOLAR TRACKER PRO V2.0
 * PART 1/6
 **************************************************/

// ============ LIBRARIES ============
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ============ OLED SETTINGS ============
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);


// ============ PIN DEFINITIONS ============
#define LDR_LEFT   34
#define LDR_RIGHT  35
#define SERVO_PIN  18
#define LED_PIN    2


// ============ WIFI HOTSPOT ============
const char* ssid = "SOLAR_TRACKER_PRO";
const char* password = "12345678";

WebServer server(80);
Servo solarServo;


// ============ SYSTEM VARIABLES ============
int servoAngle = 90;
bool autoMode = true;

int leftLDR = 0;
int rightLDR = 0;

int threshold = 120;


// OLED animation variables
int sunX = 64;
int cloudX = -30;


// ============ WEB PAGE ============
String createWebPage()
{
  String page = "";

  page += "<!DOCTYPE html>";
  page += "<html>";
  page += "<head>";

  page += "<meta name='viewport' content='width=device-width,initial-scale=1'>";

  page += "<title>Solar Tracker Pro</title>";

  page += "<style>";

  // Body style
  page += "body{";
  page += "margin:0;";
  page += "font-family:Arial;";
  page += "text-align:center;";
  page += "background:linear-gradient(180deg,#020111,#091540,#ff8c00);";
  page += "color:white;";
  page += "}";


  // Animated title
  page += "h1{";
  page += "font-size:34px;";
  page += "animation:glow 2s infinite;";
  page += "}";

  page += "@keyframes glow{";
  page += "0%{text-shadow:0 0 10px yellow;}";
  page += "50%{text-shadow:0 0 30px orange;}";
  page += "100%{text-shadow:0 0 10px yellow;}";
  page += "}";


  // Cards
  page += ".card{";
  page += "margin:15px;";
  page += "padding:20px;";
  page += "border-radius:25px;";
  page += "background:rgba(255,255,255,0.2);";
  page += "}";


  // Buttons
  page += "button{";
  page += "width:130px;";
  page += "height:50px;";
  page += "font-size:18px;";
  page += "margin:6px;";
  page += "border:none;";
  page += "border-radius:30px;";
  page += "background:#ffd700;";
  page += "cursor:pointer;";
  page += "}";


  // Slider
  page += "input[type=range]{";
  page += "width:80%;";
  page += "}";

  page += "</style>";
  page += "</head>";
  page += "<body>";


  // Heading
  page += "<h1>☀ SOLAR TRACKER PRO</h1>";


  // Mode
  page += "<div class='card'>";
  page += "<h2>MODE : ";

  if(autoMode)
    page += "AUTO ☀";
  else
    page += "MANUAL 🎮";

  page += "</h2>";

  page += "<a href='/auto'>";
  page += "<button>AUTO</button>";
  page += "</a>";

  page += "<a href='/manual'>";
  page += "<button>MANUAL</button>";
  page += "</a>";

  page += "</div>";


  // Servo
  page += "<div class='card'>";

  page += "<h2>SERVO ANGLE</h2>";

  page += "<h3>";
  page += String(servoAngle);
  page += "°</h3>";

  page += "<input type='range' min='0' max='180' value='";
  page += String(servoAngle);
  page += "' onchange=\"location='/servo?angle='+this.value\">";

  page += "</div>";


  // LDR Data
  page += "<div class='card'>";

  page += "<h3>☀ LEFT LDR : ";
  page += String(leftLDR);
  page += "</h3>";

  page += "<h3>☀ RIGHT LDR : ";
  page += String(rightLDR);
  page += "</h3>";

  page += "</div>";


  // Footer
  page += "<h3>ESP32 WiFi Solar Tracking System 🌍</h3>";

  page += "</body>";
  page += "</html>";

  return page;
}
/**************************************************
 * PART 2/6
 * Web Server & Servo Control Functions
 **************************************************/

// ---------- Home Page ----------
void handleRoot()
{
  server.send(200, "text/html", createWebPage());
}


// ---------- AUTO Mode ----------
void handleAuto()
{
  autoMode = true;

  server.sendHeader("Location", "/");
  server.send(303);
}


// ---------- MANUAL Mode ----------
void handleManual()
{
  autoMode = false;

  server.sendHeader("Location", "/");
  server.send(303);
}


// ---------- Manual Servo Control ----------
void handleServo()
{
  // Servo can be controlled only in MANUAL mode
  if (!autoMode)
  {
    if (server.hasArg("angle"))
    {
      int newAngle = server.arg("angle").toInt();

      // Limit angle
      newAngle = constrain(newAngle, 0, 180);

      // Smooth movement
      moveServoSmooth(newAngle);
    }
  }

  server.sendHeader("Location", "/");
  server.send(303);
}


// ---------- Smooth Servo Function ----------
void moveServoSmooth(int targetAngle)
{
  targetAngle = constrain(targetAngle, 0, 180);


  if (targetAngle > servoAngle)
  {
    for (int i = servoAngle; i <= targetAngle; i++)
    {
      solarServo.write(i);
      delay(8);
    }
  }

  else if (targetAngle < servoAngle)
  {
    for (int i = servoAngle; i >= targetAngle; i--)
    {
      solarServo.write(i);
      delay(8);
    }
  }


  servoAngle = targetAngle;
}


// ---------- Start Web Server ----------
void startWebServer()
{
  // Website routes
  server.on("/", handleRoot);

  server.on("/auto", handleAuto);

  server.on("/manual", handleManual);

  server.on("/servo", handleServo);


  // Error page
  server.onNotFound([]()
  {
    String errorPage = "";

    errorPage += "<html>";
    errorPage += "<body style='background:black;color:white;text-align:center;font-family:Arial;'>";

    errorPage += "<h1>404</h1>";
    errorPage += "<h2>Page Not Found</h2>";

    errorPage += "<a href='/'>";
    errorPage += "<button style='padding:10px;'>HOME</button>";
    errorPage += "</a>";

    errorPage += "</body>";
    errorPage += "</html>";

    server.send(404, "text/html", errorPage);
  });


  server.begin();


  Serial.println("================================");
  Serial.println(" SOLAR TRACKER WEB SERVER READY ");
  Serial.println("================================");

  Serial.print("WiFi Name : ");
  Serial.println(ssid);

  Serial.print("Password  : ");
  Serial.println(password);

  Serial.println("Open Browser:");
  Serial.println("http://192.168.4.1");

  Serial.println("================================");
}

/**************************************************
 * PART 3/6
 * OLED Animation Functions
 **************************************************/

// ---------- Draw Cloud ----------
void drawCloud(int x, int y)
{
  display.fillCircle(x, y, 6, WHITE);
  display.fillCircle(x + 10, y - 4, 8, WHITE);
  display.fillCircle(x + 20, y, 6, WHITE);

  display.fillRect(x - 5, y, 30, 8, WHITE);
}


// ---------- Draw Sun With Rays ----------
void drawSun(int x, int y)
{
  // Sun body
  display.fillCircle(x, y, 8, WHITE);


  // Sun rays
  for(int angle = 0; angle < 360; angle += 45)
  {
    float rad = angle * PI / 180.0;

    int x1 = x + cos(rad) * 12;
    int y1 = y + sin(rad) * 12;

    int x2 = x + cos(rad) * 18;
    int y2 = y + sin(rad) * 18;

    display.drawLine(x1, y1, x2, y2, WHITE);
  }
}


// ---------- Draw Ground ----------
void drawGround()
{
  // Ground line
  display.drawLine(0, 54, 127, 54, WHITE);


  // Grass effect
  for(int i = 0; i < 128; i += 5)
  {
    display.drawLine(i, 54, i + 2, 50, WHITE);
  }
}


// ---------- Draw Solar Panel ----------
void drawSolarPanel()
{
  // Panel stand
  display.drawLine(64, 45, 64, 54, WHITE);


  // Panel body
  display.drawRect(45, 33, 38, 12, WHITE);


  // Solar cells
  display.drawLine(54, 33, 54, 45, WHITE);
  display.drawLine(64, 33, 64, 45, WHITE);
  display.drawLine(74, 33, 74, 45, WHITE);

  display.drawLine(45, 39, 83, 39, WHITE);
}


// ---------- Move Sun According To LDR ----------
void updateSunPosition()
{
  int difference = leftLDR - rightLDR;


  if(difference > threshold)
  {
    // Light is stronger on left
    sunX -= 2;
  }

  else if(difference < -threshold)
  {
    // Light is stronger on right
    sunX += 2;
  }


  // Keep sun inside screen
  sunX = constrain(sunX, 15, 113);
}


// ---------- Animate Cloud ----------
void animateCloud()
{
  cloudX++;


  if(cloudX > 140)
  {
    cloudX = -35;
  }
}


// ---------- OLED Main Screen ----------
void updateOLED()
{
  display.clearDisplay();


  // Animate sky
  animateCloud();

  drawCloud(cloudX, 12);


  // Move and draw sun
  updateSunPosition();

  drawSun(sunX, 18);


  // Draw solar tracker scene
  drawSolarPanel();

  drawGround();


  // Status text
  display.setTextSize(1);
  display.setTextColor(WHITE);


  // Mode
  display.setCursor(0, 0);

  if(autoMode)
  {
    display.print("AUTO");
  }
  else
  {
    display.print("MAN");
  }


  // Servo angle
  display.setCursor(85, 0);

  display.print(servoAngle);
  display.print((char)247);


  // LDR values
  display.setCursor(0, 56);

  display.print("L:");
  display.print(leftLDR / 100);

  display.print(" R:");
  display.print(rightLDR / 100);


  display.display();
}
/**************************************************
 * PART 4/6
 * Setup, OLED Startup & WiFi Hotspot
 **************************************************/

// ---------- OLED Startup Animation ----------
void startupAnimation()
{
  // Sun enters from left side
  for (int x = -20; x <= 64; x += 4)
  {
    display.clearDisplay();

    // Draw moving sun
    drawSun(x, 20);

    // Show title
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(18, 50);
    display.print("SOLAR TRACKER");

    display.display();

    delay(40);
  }


  // Flash effect
  for (int i = 0; i < 3; i++)
  {
    display.invertDisplay(true);
    delay(150);

    display.invertDisplay(false);
    delay(150);
  }
}


// ---------- Setup Function ----------
void setup()
{
  // Start Serial Monitor
  Serial.begin(115200);


  // Configure pins
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);


  // Start I2C OLED
  Wire.begin(21, 22);


  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    Serial.println("OLED NOT FOUND!");

    while (true)
    {
      delay(100);
    }
  }


  // OLED startup animation
  startupAnimation();


  // Initialize Servo
  solarServo.attach(SERVO_PIN);
  solarServo.write(servoAngle);


  // Create ESP32 WiFi Hotspot
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);


  IPAddress IP = WiFi.softAPIP();


  // Show WiFi information on OLED
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);


  display.setCursor(0, 0);
  display.println("SOLAR TRACKER");


  display.setCursor(0, 15);
  display.println("WiFi HOTSPOT ON");


  display.setCursor(0, 30);
  display.print("SSID:");
  display.println(ssid);


  display.setCursor(0, 45);
  display.print("IP:");
  display.println(IP);


  display.display();


  // Give user time to read
  delay(5000);


  // Start website server
  startWebServer();


  // Serial information
  Serial.println("================================");
  Serial.println(" SOLAR TRACKER READY ");
  Serial.println("================================");

  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.print("PASSWORD: ");
  Serial.println(password);

  Serial.println("OPEN IN BROWSER:");
  Serial.println("http://192.168.4.1");

  Serial.println("================================");
}
/**************************************************
 * PART 5/6
 * Main System Loop and Solar Tracking
 **************************************************/

// ---------- Automatic Solar Tracking ----------
void autoTracking()
{
  int difference = leftLDR - rightLDR;


  // Left sensor receives more sunlight
  if (difference > threshold)
  {
    servoAngle -= 1;
  }


  // Right sensor receives more sunlight
  else if (difference < -threshold)
  {
    servoAngle += 1;
  }


  // Protect servo limits
  servoAngle = constrain(servoAngle, 0, 180);


  // Smooth servo movement
  solarServo.write(servoAngle);
}


// ---------- LED Light Indicator ----------
void updateLED()
{
  // LED ON when strong sunlight detected
  if (leftLDR > 1800 || rightLDR > 1800)
  {
    digitalWri    bbb    bte(LED_PIN, HIGH);
  
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}


// ---------- Serial Monitor Information ----------
void serialOutput()
{
  Serial.print("LEFT LDR: ");
  Serial.print(leftLDR);

  Serial.print("  RIGHT LDR: ");
  Serial.print(rightLDR);

  Serial.print("  SERVO: ");
  Serial.print(servoAngle);

  Serial.print("  MODE: ");

  if (autoMode)
  {
    Serial.println("AUTO");
  }
  else
  {
    Serial.println("MANUAL");
  }
}


// ---------- Main Loop ----------
void loop()
{
  // Handle website requests
  server.handleClient();


  // Read LDR values
  leftLDR = analogRead(LDR_LEFT);
  rightLDR = analogRead(LDR_RIGHT);


  // Automatic tracking mode
  if (autoMode)
  {
    autoTracking();
  }


  // LED status
  updateLED();


  // Update OLED animation
  updateOLED();


  // Print data to serial monitor
  serialOutput();


  // Smooth operation delay
  delay(30);
}

