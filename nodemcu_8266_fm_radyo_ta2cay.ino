
//TA2CAY-- ESP8266 İLE HAFIZALI FM RADYO ALICISI PROJESİ--TA2CAY
#include <Wire.h>
#include <U8g2lib.h>
#include <TEA5767Radio.h>
#include <TimeLib.h>
#include <DHT.h>

// DHT Sensor Definitions
#define DHTPIN D4
#define DHTTYPE DHT11

TEA5767Radio radio;
DHT dht(DHTPIN, DHTTYPE);

#define MIN_FREQUENCY 87.5
#define MAX_FREQUENCY 108.0
#define POT_PIN A0

// Favori İstasyonlar
const int FAVORITE_BUTTON_PIN = D5;
const int NUM_FAVORITES = 7; // Favori kanal sayısı 7'ye çıkarıldı
float favoriteFrequencies[NUM_FAVORITES] = {93.4, 91.8, 104.9, 96.4, 106.9, 99.2, 100.5}; 
int currentFavoriteIndex = 0;

float currentFrequency = 93.4;
const int POT_READ_INTERVAL = 50;
int lastPotValue = -1;
const int POT_THRESHOLD = 10;
int signalLevel = 50;

const int numChannels = 9;
const float thresholds[numChannels] = {99.2, 91.8, 93.4, 104.9, 96.4, 106.9, 93.0, 91.5, 102.4};
const char* channelNames[numChannels] = {"TRT 3", "TRT HABER", "TRT TURKU", "RADYO 1", "TRT NAGME", "TRT FM", "YEREL RADYO", "METEOR FM", "TRT TURKU"};

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, D1, D2, U8X8_PIN_NONE);

// Marquee Değişkenleri
int scrollX = 0;
const int scrollSpeed = 1; // Kayma hızı
String currentChannelName = "";

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(FAVORITE_BUTTON_PIN, INPUT_PULLUP);

  u8g2.begin();
  u8g2.enableUTF8Print();

  dht.begin();

  radio.setFrequency(currentFrequency);
  updateDisplay(currentFrequency);
  Serial.println("FM Radyo Test Başladı");

  showStartingMessage();
  lastPotValue = analogRead(POT_PIN);
    setTime(00, 00, 00, 1, 1, 2025); 
    // Saat 00:00:00, 1 Ocak 2025 Manuel değiştirilebilir.
}

void loop() {
    static int previousMinute = -1;

    if (minute() != previousMinute) {
        previousMinute = minute();
        updateDisplay(currentFrequency);
    }

  handlePotentiometer();
  handleSignalLevel();
  handleFavoriteButton();
}

void updateDisplay(float frequency) {
    u8g2.clearBuffer();
    int yOffset = 1; // Yazıları 1 piksel aşağı kaydırmak için

    // Sıcaklık
    u8g2.setFont(u8g2_font_helvB08_tf);
    float temperature = dht.readTemperature();
    char tempStr[10];
    dtostrf(temperature, 4, 1, tempStr);
    strcat(tempStr, "C");
    u8g2.drawStr(5, 10 + yOffset, tempStr);

    // Nem
    u8g2.setFont(u8g2_font_helvB08_tf);
    float humidity = dht.readHumidity();
    int humidityInt = round(humidity);
    char humStr[10];
    sprintf(humStr, "%%%d", humidityInt);
    int humWidth = u8g2.getStrWidth(humStr);
    u8g2.drawStr(128 - humWidth - 5, 10 + yOffset, humStr);

    // Saat (Sıcaklık ve Nemin Ortası)
    u8g2.setFont(u8g2_font_helvB08_tf);
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", hour(), minute());
    int timeWidth = u8g2.getStrWidth(timeStr);
    u8g2.drawStr((128 - timeWidth) / 2, 10 + yOffset, timeStr);

    // Frekans
    u8g2.setFont(u8g2_font_logisoso22_tf);
    char freqStr[15];
    sprintf(freqStr, "%.1f MHZ", frequency);
    int xPos = (128 - u8g2.getStrWidth(freqStr)) / 2;
    u8g2.drawStr(xPos, 40 + yOffset, freqStr);

    // Kanal İsmi (Marquee)
    u8g2.setFont(u8g2_font_helvB10_tf);
    bool foundChannel = false;
    for (int i = 0; i < numChannels; i++) {
        if (abs(frequency - thresholds[i]) < 0.1) {
           currentChannelName = String(channelNames[i]);
            foundChannel = true;
            break;
        }
    }

    if(!foundChannel){
        currentChannelName ="";
    }

    int textWidth = u8g2.getStrWidth(currentChannelName.c_str());
     if (textWidth > 128) { // Eğer metin ekrana sığmıyorsa kaydır
         u8g2.drawUTF8(128 - scrollX , 63 + yOffset, currentChannelName.c_str());
         scrollX += scrollSpeed;
         if (scrollX > textWidth + 128) {
           scrollX = 0;
         }

     }else{
        int x = (128-textWidth) / 2;
        u8g2.drawUTF8(x , 63 + yOffset, currentChannelName.c_str());
       }

    // Kanal Bulunamazsa Sinyal Seviyesi
    if (!foundChannel) {
        u8g2.drawFrame(10, 55 + yOffset, 108, 8);
        u8g2.drawBox(10, 55 + yOffset, map(signalLevel, 0, 100, 0, 108), 8);
    }
    u8g2.sendBuffer();
}

void handlePotentiometer() {
    static unsigned long lastPotReadTime = 0;

    if (millis() - lastPotReadTime >= POT_READ_INTERVAL) {
        int potValue = analogRead(POT_PIN);
        if (abs(potValue - lastPotValue) > POT_THRESHOLD) {
            float newFrequency = map(potValue, 0, 1023, MIN_FREQUENCY * 10, MAX_FREQUENCY * 10) / 10.0;
            newFrequency = round(newFrequency * 10.0) / 10.0;

            if (abs(newFrequency - currentFrequency) >= 0.1) {
                currentFrequency = newFrequency;
                radio.setFrequency(currentFrequency);
                updateDisplay(currentFrequency);
                Serial.print("Frekans Değişti: ");
                Serial.println(currentFrequency);
            }
            lastPotValue = potValue;
        }
        lastPotReadTime = millis();
    }
}

void handleSignalLevel() {
    static unsigned long lastSignalUpdateTime = 0;

    if (millis() - lastSignalUpdateTime > 500) {
        signalLevel += random(-10, 11);
        signalLevel = constrain(signalLevel, 10, 90);
        lastSignalUpdateTime = millis();
    }
}


void handleFavoriteButton() {
    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(FAVORITE_BUTTON_PIN);

    // Butona basılıp bırakıldığında (düşen kenar)
    if (lastButtonState == HIGH && currentButtonState == LOW) {
        currentFavoriteIndex = (currentFavoriteIndex + 1) % NUM_FAVORITES;
        currentFrequency = favoriteFrequencies[currentFavoriteIndex];
        radio.setFrequency(currentFrequency);
        updateDisplay(currentFrequency);
        Serial.print("Favori İstasyon Değişti: ");
        Serial.println(currentFrequency);
    }

    lastButtonState = currentButtonState;
}


void showStartingMessage() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawBox(10, 10, 108, 44);
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_logisoso22_tf);

  // Metni ortala
  int textWidth = u8g2.getStrWidth("TA2CAY");
  int xPos = (128 - textWidth) / 2;

  u8g2.drawStr(xPos, 40, "TA2CAY");
  u8g2.sendBuffer();
  delay(2000);
  u8g2.setDrawColor(1);
}