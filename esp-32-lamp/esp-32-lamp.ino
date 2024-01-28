#include <FastLED.h>
#include "GyverButton.h"

#define PIN 15
#define LEDSTRIP_PIN 2
#define NUM_LEDS 30
#define FADE_SPEED  2000
#define LED 2

CRGB leds[NUM_LEDS];
GButton butt1(PIN);
CRGB warmColor = CRGB(252, 74, 3);
CRGB coldColor = CRGB(255, 255, 255);

uint8_t value = 0;
bool isLightActive = false;
uint8_t brightness = 0;
bool incrementing = true;
bool isClickAndHold = false;
bool lastWasSingle = false;
uint32_t lastWasSingleTime = 0;


void setup() {
  pinMode(PIN, INPUT);
  pinMode(LEDSTRIP_PIN, OUTPUT);

  //  butt1.setDebounce(1);        // настройка антидребезга (по умолчанию 80 мс)
  // butt1.setTimeout(3);        // настройка таймаута на удержание (по умолчанию 500 мс)
  // butt1.setClickTimeout(6);   // настройка таймаута между кликами (по умолчанию 300 мс)

  // HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC (PIN --- КНОПКА --- GND)
  // LOW_PULL  - кнопка подключена к VCC, пин подтянут к GND
  // butt1.setType(HIGH_PULL);

  // // NORM_OPEN - нормально-разомкнутая кнопка
  // // NORM_CLOSE - нормально-замкнутая кнопка
  // butt1.setDirection(NORM_CLOSE);

  FastLED.addLeds<NEOPIXEL, LEDSTRIP_PIN>(leds, NUM_LEDS);
}

void loop() {
  butt1.tick();  // обязательная функция отработки. Должна постоянно опрашиваться

  // if ()

  if (butt1.isSingle()) {
    // toggleLeds();
    // // lastWasSingle = true;
    // // lastWasSingleTime = millis();
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
  }

   if (butt1.isHold()) {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);

    // toggleLeds();
    // lastWasSingle = true;
    // lastWasSingleTime = millis();
  }

  // if ((lastWasSingle || lastWasHold) && (millis() - lastWasSingleTime < 50) && butt1.isHold()) {
  //   setBrightness();
  // }

  // if (butt1.isHold()) {
  //   lastWasHold = true;
  // } 
  // else {}

  // if (butt1.isHold()) {
    setBrightness();
  // }
  // Transition from warm to cold
  

  // if (butt1.isDouble()) {
  //   for (int i = 0; i < 2; i++) {
  //     lightLeds();
  //   }
  // }
}

void toggleLeds() {
  if (isLightActive) {
    isLightActive = !isLightActive;

    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::White; 
    }
    FastLED.show();
  }
  else {
    isLightActive = !isLightActive;

    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black; 
    }
    FastLED.show();
  }
}

void setBrightness() {
 if (incrementing) {
    brightness++;
    if (brightness == 255) {
      incrementing = false;
      delay(100);
    }
  } else {
    brightness--;
    if (brightness == 0) {
      incrementing = true;
      delay(100);
    }
  }
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB( brightness, brightness, brightness );
  }

  FastLED.show();
  delay(5);
}

void transitionColor(CRGB startColor, CRGB endColor) {
  unsigned long startTime = millis();

  while (millis() - startTime <= FADE_SPEED) {
    float percentComplete = (millis() - startTime) / (float)FADE_SPEED;

    // Interpolate colors
    CRGB currentColor = CRGB(
      startColor.r + (endColor.r - startColor.r) * percentComplete,
      startColor.g + (endColor.g - startColor.g) * percentComplete,
      startColor.b + (endColor.b - startColor.b) * percentComplete
    );

    // Set LED colors
    fill_solid(leds, NUM_LEDS, currentColor);
    FastLED.show();
  }
}