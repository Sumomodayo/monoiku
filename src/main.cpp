#include <Arduino.h>
#include <DHT.h>

const int DHT_PIN = 21;

#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

const int SWITCH_PIN = 34;

//
//       A
//      ---
//   F |   | B
//      -G-
//   E |   | C
//      ---
//       D


const int segPins2[7] = {
    4,  // A
    16,  // B
    17,  // C
    18,  // D
    19,  // E
    22,   // F
    23   // G
};

const int segPins1[7] = {
    32,   // A
    33,   // B
    27,  // C
    14,  // D
    13,  // E
    25,   // F
    26   // G
};

const int numbers[10][7] = {

    // 0
    {1, 1, 1, 1, 1, 1, 0},

    // 1
    {0, 1, 1, 0, 0, 0, 0},

    // 2
    {1, 1, 0, 1, 1, 0, 1},

    // 3
    {1, 1, 1, 1, 0, 0, 1},

    // 4
    {0, 1, 1, 0, 0, 1, 1},

    // 5
    {1, 0, 1, 1, 0, 1, 1},

    // 6
    {1, 0, 1, 1, 1, 1, 1},

    // 7
    {1, 1, 1, 0, 0, 0, 0},

    // 8
    {1, 1, 1, 1, 1, 1, 1},

    // 9
    {1, 1, 1, 1, 0, 1, 1}
};


// 消灯

void clearDisplay(const int pins[7])
{
    for (int i = 0; i < 7; i++)
    {
        digitalWrite(pins[i], HIGH);
    }
}

void showNumber(int number, const int pins[7])
{
    if (number < 0 || number > 9)
    {
        clearDisplay(pins);
        return;
    }

    for (int i = 0; i < 7; i++)
    {
        digitalWrite(pins[i], numbers[number][i]);
    }
}

void showTwoDigits(int value)
{
    if (value < 0 || value > 99)
    {
        value = 0;
    }

    int tens = value / 10;
    int ones = value % 10;

    showNumber(tens, segPins2);

    showNumber(ones, segPins1);
}

void setup()
{

    Serial.begin(115200);

    delay(1000);

    for (int i = 0; i < 7; i++)
    {
        pinMode(segPins1[i], OUTPUT);
        digitalWrite(segPins1[i], HIGH);
    }

    for (int i = 0; i < 7; i++)
    {
        pinMode(segPins2[i], OUTPUT);
        digitalWrite(segPins2[i], HIGH);
    }

    pinMode(SWITCH_PIN, INPUT_PULLUP);

    dht.begin();

    showTwoDigits(0);

    Serial.println("温湿度計スタート");
    Serial.println("Switch:");
    Serial.println("  HIGH = 温度");
    Serial.println("  LOW  = 湿度");
    Serial.println();
}

void loop()
{
    static unsigned long lastReadTime = 0;
    static float temperature = NAN;
    static float humidity = NAN;

    unsigned long currentTime = millis();

    if (currentTime - lastReadTime >= 2000)
    {
        lastReadTime = currentTime;

        temperature = dht.readTemperature();
        humidity = dht.readHumidity();

        // ログ表示
        if (isnan(temperature) || isnan(humidity))
        {
            Serial.println("error.");
        }
        else
        {
            Serial.printf("温度: %.1f °C\n", temperature);
            Serial.printf("湿度: %.1f %%\n", humidity);
            Serial.println();
        }
    }

    int switchState = digitalRead(SWITCH_PIN);
    float value = (switchState == HIGH) ? temperature : humidity;

    if (isnan(value))
    {
        showTwoDigits(0);
    }
    else
    {
        int displayValue = round(value);

        // 2桁に収める（湿度100%は99で表示）
        if (displayValue < 0)  displayValue = 0;
        if (displayValue > 99) displayValue = 99;

        showTwoDigits(displayValue);
    }

    delay(50);
}