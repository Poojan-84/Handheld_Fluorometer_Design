#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// ---------------- TFT PINS ----------------
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// --------------- ADS1115 -------------------
Adafruit_ADS1115 ads;
const adsGain_t gainSetting = GAIN_TWO;

// --------------- Graph variables ----------------
const int graphX = 10;
const int graphY = 140;       // FIXED: moved up for safe zone
const int graphW = 300;
const int graphH = 90;        // FIXED: must keep graph inside screen

int graphPos = 0;
float lastY = -1;

float minVal = 9999;
float maxVal = -9999;

void drawGraphFrame() {
  tft.drawRect(graphX, graphY, graphW, graphH, ILI9341_WHITE);
  tft.setCursor(graphX, graphY - 20);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  //tft.print("Signal (Volts)");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // I2C for ADS1115
  Wire.begin(21, 22);

  // Initialize ADS1115
  ads.begin();
  ads.setGain(gainSetting);
  ads.setDataRate(RATE_ADS1115_128SPS);
  delay(100);

  // Initialize TFT
  tft.begin();
  tft.setRotation(1);          // landscape
  tft.fillScreen(ILI9341_BLACK);

  // Title
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(20, 10);
  tft.print("Fluorometer");

  // Labels
  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.print("Voltage (V):");

  tft.setCursor(10, 90);
  tft.print("Current (nA):");

  tft.setCursor(10, 120);
  tft.print("Min/Max (V):");

  drawGraphFrame();

  // Safety clear area at bottom of screen (prevents grey noise)
  tft.fillRect(0, graphY + graphH, 320, 20, ILI9341_BLACK);

  Serial.println("ADS1115 + TFT Ready.");
}

void loop() {

  // ---------- Sensing Code (UNCHANGED) ----------
  int16_t raw = ads.readADC_SingleEnded(0);
  float volts = ads.computeVolts(raw);

  float current_A = volts / 1000000.0;
  float current_nA = current_A * 1e9;

  Serial.printf("ADC: %d | %.6f V | %.3f nA\n",
                raw, volts, current_nA);
  // ------------------------------------------------


  // -------- Update Min/Max --------
  if (volts < minVal) minVal = volts;
  if (volts > maxVal) maxVal = volts;


  // --------- DISPLAY TEXT REFRESH ----------

  // Voltage
  tft.fillRect(150, 60, 150, 25, ILI9341_BLACK);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.setCursor(150, 60);
  tft.print(volts, 6);

  // Current
  tft.fillRect(150, 90, 150, 25, ILI9341_BLACK);
  tft.setCursor(150, 90);
  tft.print(current_nA, 3);

// Min/Max (moved left + wider clear box)
tft.fillRect(100, 120, 200, 25, ILI9341_BLACK); 
tft.setCursor(100, 120);   
tft.setTextColor(ILI9341_MAGENTA);
tft.print(minVal, 3);
tft.print("/");
tft.print(maxVal, 3);



  // --------- GRAPH DRAWING ---------

  // Erase previous column
  tft.drawFastVLine(graphX + graphPos, graphY + 1, graphH - 2, ILI9341_BLACK);

  // Map voltage to graph height (0–2.048V)
  float norm = volts / 2.048;
  if (norm < 0) norm = 0;
  if (norm > 1) norm = 1;

  int y = graphY + graphH - (norm * (graphH - 2));

  // Draw continuous line
  if (lastY < 0) lastY = y;
  tft.drawLine(graphX + graphPos - 1, lastY, graphX + graphPos, y, ILI9341_YELLOW);
  lastY = y;

  // Move graph forward
  graphPos++;
  if (graphPos >= graphW) {
    graphPos = 1;
    lastY = -1;

    // Clear graph area for next full sweep
    tft.fillRect(graphX + 1, graphY + 1, graphW - 2, graphH - 2, ILI9341_BLACK);
    tft.fillRect(0, graphY + graphH, 320, 20, ILI9341_BLACK);  // bottom safety bar
  }

  delay(150);
}
