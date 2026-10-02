#include <Arduino.h>
#include <math.h>

// 一の位 7セグメントLED
//
//       A
//      ---
//   F |   | B
//      -G-
//   E |   | C
//      ---
//       D
//
// A B C D E F G

const int segPins1[7] = {
  18,  // A
  19,  // B
  33,  // C
  25,  // D
  26,  // E
  5,   // F
  17   // G
};

// 十の位 7セグメントLED
//
// A B C D E F G

const int segPins2[7] = {
  0,   // A
  4,   // B
  27,  // C
  14,  // D
  12,  // E
  2,   // F
  15   // G
};

// NTCサーミスタ

const int TEMP_PIN = 34;

// NTCの25℃時の抵抗値
// 5KΩ
const float R0 = 5000.0;

// 基準温度
// 25℃
const float TEMP_REFERENCE = 25.0 + 273.15;

// B定数
// NTCの仕様が不明なため3950を使用
const float B = 3950.0;

// 固定抵抗
// 5.1kΩ
const float FIXED_RESISTOR = 5100.0;

// ADC設定

const float ADC_MAX = 4095.0;
const float ADC_VOLTAGE = 3.3;

// 7セグメント数字パターン
//
// A B C D E F G
//
// 1 = 点灯
// 0 = 消灯
//
// コモンカソード用

const int numbers[10][7] = {

  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9

};

// 7セグメントLEDを消灯

void clearDisplay(const int segPins[]) {

  for (int i = 0; i < 7; i++) {

    digitalWrite(
      segPins[i],
      LOW
    );

  }

}

void showNumber(
  int num,
  const int segPins[]
) {

  if (num < 0 || num > 9) {

    return;

  }

  for (int i = 0; i < 7; i++) {

    digitalWrite(
      segPins[i],
      numbers[num][i]
    );

  }

}

// 温度を7セグメントLEDに表示
// 0～99℃ → 温度を表示
// 取得失敗 → 00

void showTemperature(int temperature) {

  if (
    temperature < 0 ||
    temperature > 99
  ) {

    showNumber(
      0,
      segPins2
    );

    showNumber(
      0,
      segPins1
    );

    return;

  }

  int tens =
    temperature / 10;

    int ones =
    temperature % 10;

  showNumber(
    tens,
    segPins2
  );

  showNumber(
    ones,
    segPins1
  );

}

// NTCから温度を取得
//
// 配線:
//
// 3.3V
//  |
// 5.1kΩ
//  |
//  +-------- GPIO34
//  |
// NTC 5KΩ
//  |
// GND
//
// この配線では、温度が上がると
// NTCの抵抗値が下がる。

float getTemperature() {

  // ADCを複数回読み取る

  const int samples = 10;

  long total = 0;


  for (int i = 0; i < samples; i++) {

    total += analogRead(
      TEMP_PIN
    );

    delay(2);

  }

  // ADC平均値

  float adcValue =
    (float)total / samples;


  // ADC値を電圧に変換

  float voltage =
    (adcValue / ADC_MAX) *
    ADC_VOLTAGE;


  // 電圧異常チェック

  if (
    voltage <= 0.0 ||
    voltage >= ADC_VOLTAGE
  ) {

    return -999.0;

  }

  // NTC抵抗値を計算
  //
  // 配線:
  //
  // 3.3V
  //  |
  // 5.1kΩ
  //  |
  //  +------ GPIO34
  //  |
  // NTC
  //  |
  // GND
  //
  // R_NTC =
  //
  // 5.1kΩ × (3.3V - Vout)
  // ---------------------
  //          Vout

  float resistance =
    FIXED_RESISTOR *
    (ADC_VOLTAGE - voltage) /
    voltage;

  // 抵抗値異常チェック

  if (resistance <= 0.0) {

    return -999.0;

  }

  // Beta式

  float temperatureK =
    1.0 /
    (
      (1.0 / TEMP_REFERENCE) +
      (
        log(resistance / R0) / B
      )
    );

  // ケルビン → 摂氏

  float temperatureC =
    temperatureK - 273.15;


  return temperatureC;

}

void setup() {

  // シリアル通信

  Serial.begin(
    115200
  );

  // 一の位の7セグメントLED

  for (int i = 0; i < 7; i++) {

    pinMode(
      segPins1[i],
      OUTPUT
    );


    digitalWrite(
      segPins1[i],
      LOW
    );

  }

  // 十の位の7セグメントLED

  for (int i = 0; i < 7; i++) {

    pinMode(
      segPins2[i],
      OUTPUT
    );

    digitalWrite(
      segPins2[i],
      LOW
    );

  }

  // NTC

  pinMode(
    TEMP_PIN,
    INPUT
  );

  // ADC減衰設定

  analogSetPinAttenuation(
    TEMP_PIN,
    ADC_11db
  );


  // 初期表示
  // 温度取得前 → 00

  showNumber(
    0,
    segPins2
  );


  showNumber(
    0,
    segPins1
  );

  // シリアルモニター開始メッセージ

  Serial.println();

  Serial.println(
    "NTC Temperature Sensor"
  );

  Serial.println(
    "----------------------"
  );

}

void loop() {

  // 温度を取得

  float temperature =
    getTemperature();

  // シリアルモニターに表示

  if (temperature == -999.0) {

    Serial.println(
      "Temperature: ERROR"
    );

  }
  else {

    Serial.print(
      "Temperature: "
    );

    Serial.print(
      temperature,
      2
    );

    Serial.println(
      " C"
    );

  }
  // 温度を整数にする

  int temp =
    (int)round(
      temperature
    );
  // 7セグメントLEDに表示

  showTemperature(
    temp
  );
  // 1秒待機

  delay(
    1000
  );

}