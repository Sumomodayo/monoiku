#include <Arduino.h>

// A  B  C  D  E  F  G
int segPins[] = {
12, 14, 27, 26, 25, 33, 32
};

int numbers[10][7] = {
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

void showNumber(int num);

void setup() {

for (int i = 0; i < 7; i++) {
pinMode(segPins[i], OUTPUT);
}

}

void loop() {

for (int num = 1; num <= 9; num++) {


showNumber(num);

delay(1000);

}

}

void showNumber(int num) {

for (int i = 0; i < 7; i++) {

digitalWrite(segPins[i], numbers[num][i]);

}

}
