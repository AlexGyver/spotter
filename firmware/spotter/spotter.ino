#define DELAY_DEB 50      // дребезг кнопки, мс
#define DELAY_PULSE 50    // импульс для создания контакта, мс
#define DELAY_PAUSE 200   // пауза после этого импульса, мс
#define DELAY_AFTER 500   // пауза после сварки
#define POT_MIN 100       // мин. время сварки, мс
#define POT_MAX 5000      // макс. время сварки, мс

#define PIN_BTN 0   // кнопка
#define PIN_OUT 1   // транс
#define ADC_POT 1   // крутилка (номер АЦП)

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_OUT, OUTPUT);
}

void loop() {
  spotter();
}

void spotter() {
  if (!digitalRead(PIN_BTN)) {              // кнопка нажата
    delay(DELAY_DEB);                       // дебаунс
    digitalWrite(PIN_OUT, 1);               // включаем транс
    if (!smartDelay(DELAY_PULSE)) return;   // ждём импульс
    digitalWrite(PIN_OUT, 0);               // выключаем транс
    if (!smartDelay(DELAY_PAUSE)) return;   // ждём паузу
    digitalWrite(PIN_OUT, 1);               // включаем транс
    int thisDelay = map(analogRead(ADC_POT), 0, 1023, POT_MIN, POT_MAX);
    if (!smartDelay(thisDelay)) return;     // ждём сварку
    digitalWrite(PIN_OUT, 0);               // выключаем транс
    while (!digitalRead(PIN_BTN));          // ждём отпускания кнопки
  }
}

// умная задержка, вернёт false если кнопка отпущена и отключит транс
bool smartDelay(int time) {
  uint32_t tmr = millis();
  while (millis() - tmr < time) {
    if (digitalRead(PIN_BTN)) {
      digitalWrite(PIN_OUT, 0);
      delay(DELAY_AFTER);
      return false;
    }
  }
  return true;
}
